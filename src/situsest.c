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

#include "situsest.h"

static struct gstate gstate = {
	.input_dir = "./",
	.output_dir = "./public_www"
};

#include "arg-parse.c"
#include "config.c"
#include "gen-file-tree.c"
#include "encdr-html.c"

int
main(int argc, char *argv[])
{
	argp_parse(&argp, argc, argv, 0, 0, 0);

	if (gstate.diagnostics) {
		printf ("INPUT_DIR \t= %s\nOUTPUT_DIR \t= %s\n"
			"VERBOSE \t= %s\nSILENT\t\t= %s\nSYNC DEST\t= %s\n",
			gstate.input_dir,
			gstate.output_dir,
			gstate.verbose ? "yes" : "no",
			gstate.silent ? "yes": "no",
			gstate.sync_dest ? "yes": "no");
	}

	regex_t regpat;
	struct kcl_arena *arena = nullptr;
	kcl_arn_alloc(&arena, STACKPLUS, 4048, 4048, true);

	get_site_config(arena);

	printf(">> get_site_config complete\n");

	gen_file_lists(arena);
	
	//printf(">> %s\n",
	//kcl_str_cstr_alloc(kcl_lst_get_first(gstate.raw_files)));
	//printf(">> %s\n",
	//kcl_str_cstr_alloc(kcl_lst_get_next(gstate.raw_files)));

	//struct kcl_list *files = kcl_lst_alloc_list(LNKLST, arena, 0);
	regcomp(&regpat, ".*\\.[c|h]$", REG_NOSUB);
	//get_file_list_regex(gstate.input_dir, files, &regpat, arena);

	//diag_print_file_list(files);

	struct kcl_list *files2 = kcl_lst_alloc_list(LNKLST, arena, 0);
	//regcomp(&regpat, ".*\\.[c|h]$", REG_NOSUB);
	get_file_list_regex_2(gstate.input_dir, files2, &regpat, arena);

	diag_print_file_list_2(files2);

	struct file_list_node *cur_file = kcl_lst_get_first(files2);
	while (cur_file != NULL) {
		encode_html(cur_file);
		cur_file = kcl_lst_get_next(files2);
	}

	kcl_arn_mem_display(arena, (uintptr_t)arena, 128);

	kcl_arn_free(arena);
	
	exit (EXIT_SUCCESS);
}
