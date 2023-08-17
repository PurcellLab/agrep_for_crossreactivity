#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>

#define MAXSTR 51200
#define TOL 2

/* MIT License

Copyright (c) 2021 Kate Scull

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. */

void find_and_report(FILE *, char *, char *, char *);
//matchfinder: Purpose = to find locations and precise string matched for 'records'
//determined by agrep to approximately match a peptide sequence
//input = output from agrep command in shellscript agrep_for_crossreactivity.sh
//i.e. folder of files with names SEQUENCE.txt.
//output = csv file named output.csv with precise match, location in string, protein description.
/**********************************************************************************************************/
void main(int argc,char **argv)
{
	DIR *dirp;
        struct dirent *d;
	FILE *f, *g;
	char sequence[512], *ptr = NULL, copy[512], line[MAXSTR], out_fn[512], in_fn[512], title[512];

	if (argc != 2) {
		printf("Please enter directory name with SEQUENCE.txt files to search\n");
		exit(0);
	}
	if ((dirp = opendir(argv[1]))==NULL) {
                printf("error: check directory\n");
                exit(0);
        }
	sprintf(out_fn,"%s/output.csv", argv[1]);
	if ((g = fopen(out_fn,"w")) == NULL) {
                printf("Can't open file %s\n",out_fn);
                exit(0);
        }
	fprintf(g,"Search Peptide,Match,Location,Protein\n");

	while ((d = readdir(dirp)) != NULL) {
                if((strstr(d->d_name,".txt")) != NULL) {
			sprintf(in_fn, "%s/%s",argv[1],d->d_name);
			printf("finding matches in file: %s\n",in_fn);

			strcpy(sequence, d->d_name);
			ptr = strstr(sequence,".txt");
			*ptr = '\0';

        		if ((f = fopen(in_fn,"r")) == NULL) {
               	 		printf("Can't open file %s\n",argv[1]);
                		exit(0);
			}
	
			while(fgets(line,MAXSTR - 1,f) != NULL) {
				if (strlen(line)) {
					if (line[0] == '>') {
						strcpy(title, &line[1]);
						while (title[(int)strlen(title)-1] == '\n' || title[(int)strlen(title) - 1] == '\r' ||
                                			title[(int)strlen(title) - 1] == ' ') {
                                			title[(int)strlen(title)-1] = '\0';
                        }
					} else {
						find_and_report(g,line,sequence,title);
					}
				}
        		}	
			fclose(f);
		}
	}
	fclose(g);

	exit(0);
}
/**********************************************************************************************************/

/**********************************************************************************************************/
void find_and_report(FILE *f, char *prot, char *sequence, char *title)
{
	int i, j, error_cnt = 0, len = strlen(prot), seqlen = strlen(sequence);
	char match[MAXSTR];
//i is the starting position of the substring. j is for checking for errors
	for (i = 0;i < (len - seqlen); i++) {
		error_cnt = j = 0;
		while (error_cnt < (TOL + 1) && j < seqlen) {
			if (prot[i+j] != sequence[j]) 
				error_cnt++;
			j++;
		}
		if (error_cnt < (TOL +1)) {
			match[0] = '\0';
			strcpy(match,&prot[i]);
			match[strlen(sequence)] = '\0';
			fprintf(f,"%s,%s,%d,\"%s\"\n",sequence,match,i + 1,title);
		}
	}

	return;
}
/**********************************************************************************************************/
