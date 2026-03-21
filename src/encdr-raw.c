/*
  situsest - The Text File Publisher

  encdr-raw.c
  
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

#define file_buffer_size 1024

void
copy_file(struct file_list_node* cur_file, kcl_arena* arena)
{
	char file_buffer[file_buffer_size];
	size_t bytes_read = 0;
	
	kcl_str* output_file_str = kcl_str_new(
		gstate.output_dir,
		strlen(gstate.output_dir) + kcl_str_len(cur_file->lname),
		arena);
	kcl_str_append(output_file_str, cur_file->ename);
	if (gstate.verbose) {
		printf("Copy %s to %s\n",
		       kcl_str_to_cstr_new(cur_file->lname, arena),
		       kcl_str_to_cstr_new(output_file_str, arena));
	}

	FILE * src_file = fopen(kcl_str_to_cstr_new(cur_file->lname, arena), "r");
	if (src_file) {
		while ((bytes_read = fread(file_buffer, 1, sizeof(file_buffer), src_file))) {
			printf(">>> file buffer contents:\n");
			for (unsigned i = 0; i < bytes_read; i++) {
				putchar((int)file_buffer[i]);
			}
		}
	}
}

[[maybe_unused]]
void
encode_raw(struct kcl_list* file_list, char* dest_dir)
{
	kcl_arena* arena_local = kcl_arn_alloc(STACKPLUS, 4048, 4048, true);
	struct file_list_node *cur_file = kcl_lst_get_first(file_list);
	while (cur_file != NULL) {
		copy_file(cur_file, arena_local);
		cur_file = kcl_lst_get_next(file_list);
	}

	kcl_arn_free(arena_local);
}

