#!/bin/bash
#MIT License

#Copyright (c) 2021 Kate Scull

#Permission is hereby granted, free of charge, to any person obtaining a copy
#of this software and associated documentation files (the "Software"), to deal
#in the Software without restriction, including without limitation the rights
#to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#copies of the Software, and to permit persons to whom the Software is
#furnished to do so, subject to the following conditions:

#The above copyright notice and this permission notice shall be included in all
#copies or substantial portions of the Software.

#THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#SOFTWARE.


# Description: script to run agrep to find potential crossreactive peptides (up to two aa substitutions)
# Usage: this script depends on matchfinder.o - make sure the filepath below is correct
#Command to run: ./agrep_for_crossreactivity.sh peptidelistfilename[don't include the .txt] proteins.fasta

mapfile -t peptides < $1.txt
printf "%s\n" "${peptides[@]}"

db=$2
outputdir=$1_output
mkdir $outputdir
for i in "${peptides[@]}"
do
	fn="$outputdir/$i.txt"
	echo $fn
	awk '{ if ($1 ~ /^\>/) printf("%s\n",$0); else printf("%s",$0); }' $db | agrep -2 -D5 -I5 -d '\>' $i | sed 's/>/\n>/g' > $fn
done

/path/to/agrep_program/matchfinder.o $outputdir

echo "Finished"


