/*
  situsest - The Text File Publisher

  situsest.h
  
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
  along with Situsest.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef SITUSEST_H
#define SITUSEST_H

#define _DEFAULT_SOURCE
// #define _GNU_SOURCE
// #pragma GCC diagnostic warning "-Wunused-function"

#include <config.h>
#include <stdlib.h>
#include <stdio.h>
#include <argp.h>
#include <sys/types.h>
#include <dirent.h>
#include <regex.h>

#include "ksclib/ksclib.c"

struct gstate {
	char *input_dir;
	char *output_dir;
	char **file_list;
	char *cur_pattern;
	regex_t cur_regex;
	bool silent;
	bool verbose;
	bool sync_dest;
};

struct file_list_node {
	char   *lname;     // file name with full dir path
	char   *fname;     // pointer to file name section
	size_t  lname_len; // len of entire full name
	size_t  dname_len; // len of just dir portion of lname
	size_t  fname_len; // len of just fname portion of lname
};

struct file_list_node2 {
	kcl_str *lname;
	kcl_str *dname;
	kcl_str *fname;
};

#endif /* not SITUSEST_H */
