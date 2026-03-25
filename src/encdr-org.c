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
