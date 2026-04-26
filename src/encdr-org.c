/*
  situsest - The Text File Publisher

  encdr-org.c
  
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
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>

void
parse_org_file(kcl_str* file_str, kcl_arena* arena)
{
	//unsigned cur_posn = 0;
	//unsigned qry_posn = 0;
	//unsigned fstr_len = kcl_str_len(file_str);

	kcl_list* lines = kcl_str_get_lines(file_str, arena);

	kcl_str* line = kcl_lst_get_first(lines);
	while (line) {
		fputc('>', stdout);
		kcl_str_fputs(line, stdout);
		fputc('\n', stdout);
		line = kcl_lst_get_next(lines);
	}

	/*
	while (cur_posn < fstr_len) {
	}
	*/
}

kcl_list*
parse_file_vars(kcl_str* file_str, kcl_arena* arena)
{
	unsigned cur_posn = 0;
	unsigned qry_posn = 0;
	kcl_str* key_str;
	kcl_str* val_str;
	kcl_list* file_vars = kcl_lst_alloc_list(KV_STR, arena, 0);
	while (cur_posn < file_str->len) {
		if ((file_str->str[cur_posn] == '#') &&
		    (file_str->str[cur_posn + 1] == '+')) {
			cur_posn += 2;
			if (kcl_str_find(file_str, cur_posn, ':', &qry_posn)) {
				key_str = kcl_str_slice_new(file_str, cur_posn, qry_posn - cur_posn, arena);
				kcl_str_trim(key_str);
				cur_posn = qry_posn + 1;
				kcl_str_find(file_str, cur_posn, '\n', &qry_posn);
				val_str = kcl_str_slice_new(file_str, cur_posn, qry_posn - cur_posn, arena);
				kcl_str_trim(val_str);
				kcl_lst_add_datum_w_key(file_vars, val_str, key_str);
			}
		}
		if (kcl_str_find(file_str, cur_posn, '\n', &qry_posn)) {
			cur_posn = qry_posn + 1;
		} else {
			cur_posn = file_str->len;
		}
	}
	return file_vars;
}

void
transform_org_file(kcl_str* file_str, kcl_arena* arena)
{
	unsigned cur_posn = 0;
	unsigned qry_posn = 0;
	unsigned i;
	bool prev_line_blank = false;
	bool state_in_para = false;
	kcl_str cur_line_str;
	kcl_str cur_text_str;
	while (cur_posn < file_str->len) {
		if (kcl_str_find(file_str, cur_posn, '\n', &qry_posn)) {
			kcl_str_slice(&cur_line_str, file_str, cur_posn, qry_posn - cur_posn);
		} else {
			kcl_str_slice(&cur_line_str, file_str, cur_posn, file_str->len - cur_posn);
		}
		switch (cur_line_str.str[0]) {
		case '#':
			break;
		case '*':
			// write heading code
			i = 0;
			while (cur_line_str.str[i] != ' ') { i++; }
			fputs("<h", stdout);
			fputc(i + 49, stdout);
			fputc('>', stdout);
			kcl_str_slice(&cur_text_str, &cur_line_str, i, cur_line_str.len - i);
			kcl_str_trim(&cur_text_str);
			kcl_str_fputs(&cur_text_str, stdout);
			fputs("</h", stdout);
			fputc(i + 49, stdout);
			fputc('>', stdout);
			fputc('\n', stdout);
			prev_line_blank = true;
			break;
		case '\n':
			if (state_in_para) {
				fputs("</p>\n", stdout);
			}
			prev_line_blank = true;
			state_in_para = false;
			break;
		default:
			if (prev_line_blank) {
				fputs("<p>", stdout);
			}
			kcl_str_trim(&cur_line_str);
			kcl_str_fputs(&cur_line_str, stdout);
			prev_line_blank = false;
			state_in_para = true;
			break;
		}
		cur_posn = qry_posn + 1;
	}
	if (state_in_para) {
		fputs("</p>", stdout);
	}
}

void
write_org_file_w_template(kcl_str* file_str, struct template_struct* template, kcl_str* output_file_str, FILE* file_ptr, kcl_list* file_vars, kcl_arena* arena)
{
	kcl_str key_str;
	kcl_str* val_str;
	FILE* output_file_ptr = fopen(kcl_str_to_cstr_new(output_file_str, arena), "w");
	if (output_file_ptr) {
		kcl_str* template_str = kcl_lst_get_first(template->template_tree);
		while (template_str) {
			switch (template_str->str[0]) {
			case '%':
				kcl_str_slice(&key_str, template_str, 1, template_str->len - 1);
				val_str = kcl_lst_get_val(file_vars, &key_str);
				kcl_str_fputs(val_str, stdout);
				break;
			case '#':
				//output org file
				//printf("%s", kcl_str_to_cstr_new(template_str, arena));
				transform_org_file(file_str, arena);
				break;
			default:
				//output template str
				kcl_str_fputs(template_str, stdout);
			}
			template_str = kcl_lst_get_next(template->template_tree);
		}
	}
}

void
encode_org_file(struct file_list_node* cur_file, kcl_arena* arena)
{
	kcl_str* output_file_str = kcl_str_new(
		gstate.output_dir,
		strlen(gstate.output_dir) + kcl_str_len(cur_file->lname),
		arena);
	kcl_str_append(output_file_str, cur_file->ename);
	kcl_str_append(output_file_str, cur_file->fname);
	output_file_str->str[output_file_str->len - 3] = 'h';
	output_file_str->str[output_file_str->len - 2] = 't';
	output_file_str->str[output_file_str->len - 1] = 'm';
	output_file_str->str[output_file_str->len - 0] = 'l';
	output_file_str->len++;
	
	printf("Transforming %s to %s\n",
	       kcl_str_to_cstr_new(cur_file->lname, arena),
	       kcl_str_to_cstr_new(output_file_str, arena));

	struct stat file_info;
	FILE * file_ptr = fopen(kcl_str_to_cstr_new(cur_file->lname, arena), "r");
	if (file_ptr) {
		fstat(fileno(file_ptr), &file_info);
		kcl_str* file_str = kcl_str_new("", file_info.st_size, arena);
		fread(file_str->str, 1, file_info.st_size, file_ptr);
		file_str->len = file_info.st_size;
		kcl_list* file_vars = parse_file_vars(file_str, arena);
		kcl_str* tmp = kcl_lst_get_first(file_vars);
		kcl_str* key = kcl_lst_get_cur_key(file_vars);
		printf("File variables in %s:\n", kcl_str_to_cstr_new(cur_file->lname, arena));
		while (tmp) {
			printf("  %s \t: %s\n", kcl_str_to_cstr_new(key, arena), kcl_str_to_cstr_new(tmp, arena));
			tmp = kcl_lst_get_next(file_vars);
			key = kcl_lst_get_cur_key(file_vars);
		}
		kcl_str* tmp_key = kcl_str_new("_default.seht", 15,  arena);
		struct template_struct* template = kcl_lst_get_val(gstate.templates, tmp_key);
		write_org_file_w_template(file_str, template, output_file_str, file_ptr, file_vars, arena);
		parse_org_file(file_str, arena);
	}
}
	       
void
encode_org()
{
	kcl_arena* arena_local = kcl_arn_alloc(STACKPLUS, 4048, 4048, true);
	struct file_list_node* cur_file = kcl_lst_get_first(gstate.files_org);
	while (cur_file) {
		encode_org_file(cur_file, arena_local);
		cur_file = kcl_lst_get_next(gstate.files_org);
	}

	kcl_arn_free(arena_local);
}
