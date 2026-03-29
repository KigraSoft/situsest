/*
  situsest - The Text File Publisher

  load-templates.c
  
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

void
load_template(struct file_list_node* cur_file, kcl_arena* arena)
{
	struct stat file_info;
	unsigned qry_posn;
	//unsigned file_size;
	FILE * template_file = fopen(kcl_str_to_cstr_new(cur_file->lname, arena), "r");
	if (template_file) {
		fstat(fileno(template_file), &file_info);
		kcl_str* file_str = kcl_str_new("", file_info.st_size, arena);
		fread(file_str->str, 1, file_info.st_size, template_file);
		file_str->len = file_info.st_size;
		printf("Template file:\n\n>>>>>\n%s\n<<<<\n", kcl_str_to_cstr_new(file_str, arena));
		if (kcl_str_find(file_str, 0, '$', &qry_posn)) {
			printf("  >+>+> Found at: %u\n", qry_posn);
		}
		if (kcl_str_find(file_str, 0, "{$ ", &qry_posn)) {
			printf("  >+>+> Found at: %u\n", qry_posn);
		}
	}
}

bool
load_templates(kcl_arena* arena)
{
	gstate.templates = kcl_lst_alloc_list(LNKLST, arena, 0);
	if (!gstate.templates) { return false; }

	struct file_list_node* cur_file = kcl_lst_get_first(gstate.files_templates);
	while (cur_file) {
		load_template(cur_file, arena);
		cur_file = kcl_lst_get_next(gstate.files_templates);
	}

	return true;
}
