/*
  situsest - The Text File Publisher

  situsest.c
  
  Copyright (C) 2026 Stephen R. Kifer

  Situsest is free software: you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Situsest is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with GNU Emacs.  If not, see
  <https://www.gnu.org/licenses/>.
*/

#include <config.h>
#include <stdlib.h>
#include <stdio.h>
#include <argp.h>
#include <sys/types.h>
#include <dirent.h>

#include "situsest.h"

static struct gstate gstate = { 0, 0, "./", "./public_www" };

#include "arg-parse.c"

int
main(int argc, char *argv[])
{
	argp_parse(&argp, argc, argv, 0, 0, 0);

	printf ("OUTPUT_DIR = %s\nOUTPUT_DIR = %s\n"
		"VERBOSE = %s\nSILENT = %s\n",
		gstate.input_dir,
		gstate.output_dir,
		gstate.verbose ? "yes" : "no",
		gstate.silent ? "yes": "no");

	DIR *dp;
	struct dirent *ep;

	dp = opendir(gstate.input_dir);
	if (dp != NULL) {
		while (ep = readdir(dp)) {
			printf("> %s\n", ep->d_name);
		}
		(void) closedir(dp);
	} else {
		perror("Couldn't open the directory");
	}
	
	exit(0);
}
