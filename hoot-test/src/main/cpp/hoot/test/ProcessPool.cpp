/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "ProcessPool.h"

#include <hoot/js/v8Engine.h>

#include <iostream>

using namespace std;


JobQueue::JobQueue()
{
}

bool JobQueue::empty()
{
  _mutex.lock();
  bool e = _jobs.empty();
//  if (e)
//  {
//    std::cout << "Job queue: " << _name.toStdString() << " empty. " << std::endl;
//  }
//  else
//  {
//    std::cout << "Job queue " << _name.toStdString() << " size: " << _jobs.size() << std::endl;
//  }
  _mutex.unlock();
  return e;
}

bool JobQueue::contains(const QString& job)
{
  _mutex.lock();
  bool c = _jobs.find(job) != _jobs.end();
  _mutex.unlock();
  return c;
}

QString JobQueue::pop()
{
  _mutex.lock();
  QString job;
  //  Don't try to erase a job from an empty queue
  if (!_jobs.empty())
  {
    job = *_jobs.begin();
    _jobs.erase(_jobs.begin());
  }
  //std::cout << "launched test: " << job.toStdString() << std::endl;
  _mutex.unlock();
  return job;
}

void JobQueue::push(const QString& job)
{
  _mutex.lock();
  _jobs.insert(job);
  _mutex.unlock();
}

ProcessThread::ProcessThread(bool showTestName,
                             bool suppressFailureDetail,
                             bool printDiff,
                             bool disableFailureRetries,
                             double waitTime,
                             QMutex* outMutex,
                             JobQueue* parallelJobs,
                             JobQueue* serialJobs)
  : _showTestName(showTestName),
    _suppressFailureDetail(suppressFailureDetail),
    _printDiff(printDiff),
    _disableFailureRetries(disableFailureRetries),
    _waitTime(waitTime),
    _outMutex(outMutex),
    _parallelJobs(parallelJobs),
    _serialJobs(serialJobs),
    _failures(0),
    _proc()
{
}

int ProcessThread::getFailures()
{
  return _failures;
}

void ProcessThread::resetProcess()
{
  //std::cout << "resetting process" << std::endl;

  //  Kill the process
  _proc->write(QString("%1\n").arg(HOOT_TEST_FINISHED).toLatin1());
  _proc->waitForFinished();
  //  Start a new process
  _proc.reset(createProcess());
}

QProcess* ProcessThread::createProcess()
{
  //std::cout << "Creating process" << std::endl;

  QProcess* proc = new QProcess();
  proc->setProcessChannelMode(QProcess::MergedChannels);
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  proc->setProcessEnvironment(env);
  QString names = (_showTestName ? "--names" : "");
  QString suppressFailureDetail = (_suppressFailureDetail ? "--suppress-failure-detail" : "");
  QString diff = (_printDiff ? "--diff" : "");

  proc->start(
    QString("HootTest %1 %2 %3 --listen %4")
      .arg(names).arg(suppressFailureDetail).arg(diff).arg((int)_waitTime));
  return proc;
}

void ProcessThread::run()
{
  //std::cout << "running process" << std::endl;

  _proc.reset(createProcess());
  if (_serialJobs != nullptr)
  {
    processJobs(_serialJobs);
    //  Reset the process between queues
    resetProcess();
  }
  processJobs(_parallelJobs);

  _proc->write(QString("%1\n").arg(HOOT_TEST_FINISHED).toLatin1());
  _proc->waitForFinished();
}

void ProcessThread::processJobs(JobQueue* queue)
{
  int restart_count = 0;
  const int MAX_RESTART = 3;
  const int READ_TIMEOUT = 500;
  bool working = true;
  while (working)
  {
    if (!queue->empty())
    {
      QString test = queue->pop();
      //  Empty strings can be ignored
      if (test.isEmpty())
        continue;
      _proc->write(QString("%1\n").arg(test).toLatin1());
      //  Read all of the output
      QString output;
      _proc->waitForReadyRead(READ_TIMEOUT);
      QString line = QString(_proc->readLine()).trimmed();
      while (line != HOOT_TEST_FINISHED)
      {
        if (_proc->state() == QProcess::NotRunning && !_disableFailureRetries)
        {
          //  Put this job that failed back on the queue
          queue->push(test);
          ++restart_count;
          //  Restart the process if there was a process failure
          if (restart_count < MAX_RESTART)
          {
            _proc.reset(createProcess());
            output.clear();
          }
          else
          {
            working = false;
            _outMutex->lock();
            cout << test.toStdString() << " failed to execute, exiting thread." << endl;
            _outMutex->unlock();
          }
          break;
        }
        else if (line == "")
        {
          if (output != "" && output != "." && !output.endsWith("\n"))
            output.append("\n");
        }
        else if ((line.contains(" ERROR ") || line.contains("Failure: ")) &&
                 !_disableFailureRetries)
        {
          ++_failures;
          line.append("\n");
          //  If the process is still running, then wait for it to finish
          if (_proc->state() == QProcess::Running)
          {
            QString next;
            while (next != HOOT_TEST_FINISHED)
            {
              if (_proc->bytesAvailable() < 1)
                _proc->waitForReadyRead(READ_TIMEOUT);
              next = QString(_proc->readLine()).trimmed();
              if (next != HOOT_TEST_FINISHED && !next.isEmpty())
                line.append(next.append("\n"));
              //  If the process ends here before writing HOOT_TEST_FINISH, break out of the loop
              if (_proc->state() != QProcess::Running)
                break;
            }
          }
          output.append(line);
          //  Reset the process
          resetProcess();
          line = HOOT_TEST_FINISHED;
          continue;
        }
        else if (line != ".")
          line.append("\n");
        output.append(line);
        if (_proc->bytesAvailable() < 1)
          _proc->waitForReadyRead(READ_TIMEOUT);
        line = QString(_proc->readLine()).trimmed();
      }
      output = output.replace("\n\n\n", "\n").replace("\n\n", "\n");
      _outMutex->lock();
      cout << output.toStdString();
      cout.flush();
      _outMutex->unlock();
    }
    else
      working = false;
  }
}

ProcessPool::ProcessPool(
  int nproc, double waitTime, bool showTestName, bool suppressFailureDetail, bool printDiff,
  bool disableFailureRetries) :
_failed(0)
{
  for (int i = 0; i < nproc; ++i)
  {
    //  First process gets the serial jobs
    JobQueue* serial = (i == 0) ? &_serialJobs : nullptr;
    ProcessThreadPtr thread =
      std::make_shared<ProcessThread>(
        showTestName, suppressFailureDetail, printDiff, disableFailureRetries, waitTime, &_mutex,
        &_parallelJobs, serial);
    _threads.push_back(thread);
  }
}

ProcessPool::~ProcessPool()
{
  //std::cout << "Shutting down process pool..." << std::endl;
  for (vector<ProcessThreadPtr>::size_type i = 0; i < _threads.size(); ++i)
  {
    _threads[i]->quit();
    _threads[i]->wait();
  }
  //std::cout << "Process pool shut down." << std::endl;
}

void ProcessPool::startProcessing()
{
  for (vector<ProcessThreadPtr>::size_type i = 0; i < _threads.size(); ++i)
  {
    //std::cout << "Starting thread: " << i << std::endl;
    _threads[i]->start();
  }
  //std::cout << "All threads started." << std::endl;
}

void ProcessPool::wait()
{
  for (vector<ProcessThreadPtr>::size_type i = 0; i < _threads.size(); ++i)
  {
    //std::cout << "Waiting for thread: " << i << std::endl;
    _threads[i]->wait();
  }
  //std::cout << "Finished waiting." << std::endl;
}

void ProcessPool::addJob(const QString& test, bool parallel)
{
  _parallelJobs.setName("parallel"); // TODO: move this somewhere else?
  _serialJobs.setName("serial");

  if (parallel && !_serialJobs.contains(test))
    _parallelJobs.push(test);
  else if (!parallel)
    _serialJobs.push(test);
}

int ProcessPool::getFailures()
{
  int failures = 0;
  for (vector<ProcessThreadPtr>::size_type i = 0; i < _threads.size(); ++i)
    failures += _threads[i]->getFailures();
  return failures;
}
