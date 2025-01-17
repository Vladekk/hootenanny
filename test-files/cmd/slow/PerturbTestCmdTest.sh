#!/bin/bash
set -e

mkdir -p $HOOT_HOME/test-output/cmd/slow/PerturbTestCmdTest

# ROAD TEST

infile=$HOOT_HOME/test-files/DcGisRoads.osm
indir=$HOOT_HOME/test-files/cmd/slow/PerturbTestCmdTest
outdir=$HOOT_HOME/test-output/cmd/slow/PerturbTestCmdTest

myCmd="hoot perturb -C Testing.conf \
        -D random.seed=1 \
        -D perty.test.num.runs=1 \
        -D perty.test.num.simulations=1 \
        -D perty.test.expected.scores=0.5 \
        -D perty.test.allowed.score.variance=0.55 \
        $infile $outdir --test"

pass=`$myCmd | grep "Test Passed:" | sed "s/Test Passed: //g;"`

if [ $pass = "true" ]; then
  echo "PERTY Test Passed"
else
  cleanCmd=`echo $myCmd | sed "s/\s+?/ /g"`  
  printf "PERTY Test Failed! Try running\n\n$cleanCmd\n\nto find out why\n\n"
fi 

# POI TEST

infile=$indir"/poi-input.osm"

myCmd="hoot perturb -C Testing.conf \
        -D random.seed=1 \
        -D perty.test.num.runs=1 \
        -D perty.test.num.simulations=1 \
        -D perty.test.expected.scores=0.5 \
        -D perty.test.allowed.score.variance=0.9 \
        $infile $outdir --test"

pass=`$myCmd | grep "Test Passed:" | sed "s/Test Passed: //g;"`

if [ $pass = "true" ]; then
  echo "PERTY Test Passed"
else
  cleanCmd=`echo $myCmd | sed "s/\s+?/ /g"`  
  printf "PERTY Test Failed! Try running\n\n$cleanCmd\n\nto find out why\n\n"
fi 

