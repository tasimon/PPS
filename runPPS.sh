#!/bin/bash
rm ./bin/PPS
make -s PPS

for file in `ls -1 data/*.txt`
	do
	echo "filename="$file
	./bin/PPS $file | column -t
	done

