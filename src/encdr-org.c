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
	unsigned cur_posn = 0;
	unsigned qry_posn = 0;
	kcl_str* key_str;
	kcl_str* val_str;
	FILE * file_ptr = fopen(kcl_str_to_cstr_new(cur_file->lname, arena), "r");
	if (file_ptr) {
		fstat(fileno(file_ptr), &file_info);
		kcl_str* file_str = kcl_str_new("", file_info.st_size, arena);
		fread(file_str->str, 1, file_info.st_size, file_ptr);
		file_str->len = file_info.st_size;
		while (cur_posn < file_str->len) {
			if ((file_str->str[cur_posn] == '#') &&
			    (file_str->str[cur_posn + 1] == '+')) {
				cur_posn += 2;
				if (kcl_str_find(file_str, cur_posn, ':', &qry_posn)) {
					key_str = kcl_str_slice_new(file_str, cur_posn, qry_posn - cur_posn, arena);
					cur_posn = qry_posn + 1;
					kcl_str_find(file_str, cur_posn, '\n', &qry_posn);
					val_str = kcl_str_slice_new(file_str, cur_posn, qry_posn - cur_posn, arena);
					kcl_str_trim(val_str);
					printf("Key/Value Found:\n  Key: %s\n  Val: %s\n",
					       kcl_str_to_cstr_new(key_str, arena),
					       kcl_str_to_cstr_new(val_str, arena));
				}
			}
			if (kcl_str_find(file_str, cur_posn, '\n', &qry_posn)) {
				cur_posn = qry_posn + 1;
			} else {
				cur_posn = file_str->len;
			}
		}
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
