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

struct template_struct {
	struct file_list_node*	file;
	kcl_str*                template_str;
	kcl_list*		template_tree;
};

bool
//load_template(struct file_list_node* cur_file, kcl_arena* arena)
load_template(struct template_struct* template, kcl_arena* arena_lcl)
{
	struct stat file_info;
	unsigned cur_posn = 0;
	unsigned qry_posn;
	kcl_str* tmp_str;
	FILE * template_file = fopen(kcl_str_to_cstr_new(template->file->lname, arena_lcl), "r");
	if (template_file) {
		fstat(fileno(template_file), &file_info);
		//kcl_str* file_str = kcl_str_new("", file_info.st_size, arena);
		template->template_str = kcl_str_new("", file_info.st_size, template->template_tree->arena);
		fread(template->template_str->str, 1, file_info.st_size, template_file);
		template->template_str->len = file_info.st_size;
		//printf("Template file:\n\n>>>>>\n%s\n<<<<\n", kcl_str_to_cstr_new(file_str, arena));
		printf("Template file:\n\n>>>>>\n%s\n<<<<\n", kcl_str_to_cstr_new(template->template_str, arena_lcl));
		if (kcl_str_find(template->template_str, 0, '$', &qry_posn)) {
			printf("  >+>+> Found at: %u\n", qry_posn);
		}
		if (kcl_str_find(template->template_str, 0, "{$ ", &qry_posn)) {
			printf("  >+>+> Found at: %u\n", qry_posn);
		}
		while (cur_posn < template->template_str->len) {
			if (kcl_str_find(template->template_str, cur_posn, "{$ ", &qry_posn)) {
				tmp_str = kcl_str_slice_new(template->template_str, cur_posn, qry_posn - cur_posn, template->template_tree->arena);
				kcl_lst_append_datum(template->template_tree, tmp_str);
				cur_posn = qry_posn + 3;
				if (kcl_str_find(template->template_str, cur_posn, " }", &qry_posn)) {
					tmp_str = kcl_str_slice_new(template->template_str, cur_posn, qry_posn - cur_posn, template->template_tree->arena);
					kcl_lst_append_datum(template->template_tree, tmp_str);
					cur_posn = qry_posn + 2;
				} else { return false; }
			} else {
				tmp_str = kcl_str_slice_new(template->template_str, cur_posn, template->template_str->len - cur_posn, template->template_tree->arena);
				kcl_lst_append_datum(template->template_tree, tmp_str);
				cur_posn = template->template_str->len;
			}
		}
		printf("Template tree:\n");
		tmp_str = kcl_lst_get_first(template->template_tree);
		while (tmp_str) {
			printf(">_>\n%s\n<_<\n", kcl_str_to_cstr_new(tmp_str, arena_lcl));
			tmp_str = kcl_lst_get_next(template->template_tree);
		}
		return true;
	} else { return false; }
}

bool
load_templates(kcl_arena* arena)
{
	gstate.templates = kcl_lst_alloc_list(LNKLST, arena, 0);
	if (!gstate.templates) { return false; }

	struct template_struct* template;
	struct file_list_node* cur_file = kcl_lst_get_first(gstate.files_templates);
	while (cur_file) {
		template = kcl_arn_push(arena, sizeof (struct template_struct));
		template->file = cur_file;
		template->template_tree = kcl_lst_alloc_list(LNKLST, arena, 0);
		load_template(template, arena);
		kcl_lst_add_datum(gstate.templates, template);
		cur_file = kcl_lst_get_next(gstate.files_templates);
	}

	return true;
}
