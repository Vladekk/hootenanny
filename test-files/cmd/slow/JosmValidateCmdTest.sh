#!/bin/bash
set -e

IN_DIR=test-files/cmd/slow/JosmValidateCmdTest
OUT_DIR=test-output/cmd/slow/JosmValidateCmdTest
mkdir -p $OUT_DIR

LOG_LEVEL=--warn
CONFIG="-C Testing.conf"

INPUT=test-files/ops/JosmMapCleanerTest/runCleanTest-in.osm

OUT_SINGLE=$OUT_DIR/single-out.osm
GOLD_SINGLE=$IN_DIR/single-out.osm

OUT_FILE_MULTIPLE=$OUT_DIR/multiple-out.osm
GOLD_MULTIPLE=$IN_DIR/multiple-out.osm

SEPARATE_OUTPUT_OUT_1=$OUT_DIR/runCleanTest1-in-validated.osm
SEPARATE_OUTPUT_OUT_2=$OUT_DIR/runCleanTest2-in-validated.osm
SEPARATE_OUTPUT_GOLD_1=$IN_DIR/runCleanTest1-in-validated.osm
SEPARATE_OUTPUT_GOLD_2=$IN_DIR/runCleanTest2-in-validated.osm

echo ""
echo "Listing available validators..."
echo ""
hoot validate $LOG_LEVEL $CONFIG --validators | grep "DuplicatedWayNodes"

echo ""
echo "Validating a single input..."
echo ""
# No output being compared here, just making sure it runs.
hoot validate $LOG_LEVEL $CONFIG -D josm.validators.file=$IN_DIR/validators $INPUT

echo ""
echo "Validating a single input and using an output file..."
echo ""
hoot validate $LOG_LEVEL $CONFIG -D josm.validators.file=$IN_DIR/validators $INPUT --output $OUT_SINGLE
hoot diff $LOG_LEVEL $CONFIG $GOLD_SINGLE $OUT_SINGLE

echo ""
echo "Validating a single input and writing the report to a file..."
echo ""
hoot validate $LOG_LEVEL $CONFIG -D josm.validators.file=$IN_DIR/validators $INPUT --report-output $OUT_DIR/single-validation-report
diff $IN_DIR/single-validation-report $OUT_DIR/single-validation-report

echo ""
echo "Validating multiple inputs..."
echo ""
# Just loading the same file twice for now...could eventually get a second input file. Should get 
# double the number of validation errors compared to the single input file.
hoot validate $LOG_LEVEL $CONFIG -D josm.validators.file=$IN_DIR/validators $INPUT $INPUT --output $OUT_FILE_MULTIPLE
hoot diff $LOG_LEVEL $CONFIG $GOLD_MULTIPLE $OUT_FILE_MULTIPLE

echo ""
echo "Validating recursively in a directory structure with a filter..."
echo ""
hoot validate $LOG_LEVEL $CONFIG -D josm.validators.file=$IN_DIR/validators test-files/ops/JosmMapCleanerTest --recursive "*.json"

echo ""
echo "Validating files, writing to separate outputs, and writing the report to a file..."
echo ""
# Copy the inputs to the output directory and work from there, so we don't write to the input dir 
# under source control. Just loading the same file twice for now...could eventually get a second 
# input file. Should get two identical validation reports in the output.
cp $INPUT $OUT_DIR/runCleanTest1-in.osm
cp $INPUT $OUT_DIR/runCleanTest2-in.osm
hoot validate $LOG_LEVEL $CONFIG -D josm.validators.file=$IN_DIR/validators $OUT_DIR/runCleanTest1-in.osm $OUT_DIR/runCleanTest2-in.osm --separate-output --report-output $OUT_DIR/separate-output-validation-report
hoot diff $LOG_LEVEL $CONFIG $SEPARATE_OUTPUT_GOLD_1 $SEPARATE_OUTPUT_OUT_1
hoot diff $LOG_LEVEL $CONFIG $SEPARATE_OUTPUT_GOLD_2 $SEPARATE_OUTPUT_OUT_2
diff $IN_DIR/separate-output-validation-report $OUT_DIR/separate-output-validation-report
