#!/bin/bash
for filename in ../events/*; do
./effCalcCsv.exe $filename;
done