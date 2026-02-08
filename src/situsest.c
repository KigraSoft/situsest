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
  along with Situsest.  If not, see <https://www.gnu.org/licenses/>.
*/

#define _DEFAULT_SOURCE
#pragma GCC diagnostic warning "-Wunused-function"

#include <config.h>
#include <stdlib.h>
#include <stdio.h>
#include <argp.h>
#include <sys/types.h>
#include <dirent.h>

#include "situsest.h"

static struct gstate gstate = { 0, 0, "./", "./public_www" };

#include "ksclib/ksclib.c"
#include "arg-parse.c"
#include "gen-file-tree.c"
#include "encdr-html.c"

int
main(int argc, char *argv[])
{
	argp_parse(&argp, argc, argv, 0, 0, 0);

	printf ("INPUT_DIR = %s\nOUTPUT_DIR = %s\n"
		"VERBOSE = %s\nSILENT = %s\n",
		gstate.input_dir,
		gstate.output_dir,
		gstate.verbose ? "yes" : "no",
		gstate.silent ? "yes": "no");

	struct kcl_arena *arena = kcl_arn_alloc(STACK, 4048, 4048, true);
	struct kcl_list *files = kcl_lst_alloc_list(LNKLST, arena, 0);
	get_file_list(gstate.input_dir, files, "*.c", arena);
	diag_print_file_list(files);

	struct file_list_node *cur_file = kcl_lst_get_first(files);
	while (cur_file != NULL) {
		encode_html(cur_file);
		cur_file = kcl_lst_get_next(files);
	}

	exit(0);
}
