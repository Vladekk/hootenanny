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
 * @copyright Copyright (C) 2016, 2017, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
package hoot.services.job;

import javax.ws.rs.WebApplicationException;

import org.apache.commons.lang3.exception.ExceptionUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.ApplicationContext;
import org.springframework.transaction.PlatformTransactionManager;
import org.springframework.transaction.TransactionStatus;

import java.util.ArrayList;
import java.util.Arrays;

import hoot.services.ApplicationContextUtils;
import hoot.services.command.Command;
import hoot.services.command.CommandResult;
import hoot.services.utils.DbUtils;


class JobRunnable implements Runnable {
    private static final Logger logger = LoggerFactory.getLogger(JobRunnable.class);

    private final Job job;
    private final JobStatusManager jobStatusManager;


    JobRunnable(Job job, JobStatusManager jobStatusManager) {
        this.job = job;
        this.jobStatusManager = jobStatusManager;
    }

    @Override
    public void run() {
        ApplicationContext appContext = ApplicationContextUtils.getApplicationContext();
        PlatformTransactionManager txManager = appContext.getBean("transactionManager", PlatformTransactionManager.class);

        TransactionStatus transactionStatus = null;
        try {
            transactionStatus = txManager.getTransaction(null);
            processJob();
            txManager.commit(transactionStatus);
        }
        catch (Exception e) {
            if ((transactionStatus != null) && !transactionStatus.isCompleted()) {
                txManager.rollback(transactionStatus);
            }
            logger.error("Error executing job with ID = {}", job.getJobId(), e);
        }
    }

    private void processJob() {
        logger.debug("Start processing chain Job with jobId = {}", job.getJobId());

        try {
            jobStatusManager.addJob(job);

            int commandCounter = 0;
            for (Command command : job.getCommands()) {
                CommandResult result = command.execute();

                if (result.failed()) {
                    //Cancelled jobs also have non-zero exit code, but should not be marked FAILED
                    if (!JobStatus.CANCELLED.equals(jobStatusManager.getJobStatusObj(job.getJobId()).getStatus())) {
                        jobStatusManager.setFailed(job.getJobId(), "Command with ID = " + result.getId() + " caused the failure.");
                    }
                    break;
                }
                else {
                    commandCounter++;
                    Integer percentComplete = DbUtils.getJobProgress(job.getJobId());
                    jobStatusManager.updateJob(job.getJobId(), commandCounter + " out of " +
                            job.getCommands().length + " have been processed.", percentComplete);
                }
            }

            jobStatusManager.setCompleted(job.getJobId(), "FULLY PROCESSED");

            ArrayList<JobType> uploadJobs = new ArrayList<>(Arrays.asList(JobType.UPLOAD_CHANGESET, JobType.BULK_ADD, JobType.BULK_DIFFERENTIAL, JobType.BULK_REPLACE));
            if(uploadJobs.contains(job.getJobType())){
                String parentId = job.getTags().get("parentId");
                DbUtils.setStale(parentId);
                DbUtils.checkConflicted(job.getJobId(), parentId);
            }
        }
        catch (WebApplicationException e) {
            logger.error("Job with ID = " + job.getJobId() + " failed with the following error --> " + ExceptionUtils.getStackTrace(e));
            jobStatusManager.setFailed(job.getJobId(), e.getResponse().getEntity().toString());
            throw e;
        }

        logger.debug("End processing chain Job with jobId = {}", job.getJobId());
    }
}
