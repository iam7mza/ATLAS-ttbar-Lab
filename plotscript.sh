#!/bin/bash
for filename in ../CutData/output_Selection/*; do
./plotHist.exe $filename;
done