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

void
copy_file(struct file_list_node* cur_file, char* dest_dir, kcl_arena* arena)
{
	if (gstate.verbose) {
		printf("Copy %s to %s\n", kcl_str_to_cstr_new(cur_file->lname, arena), dest_dir);
	}
}

[[maybe_unused]]
void
encode_raw(struct kcl_list* file_list, char* dest_dir)
{
	kcl_arena* arena_local = kcl_arn_alloc(STACKPLUS, 4048, 4048, true);
	struct file_list_node *cur_file = kcl_lst_get_first(file_list);
	while (cur_file != NULL) {
		copy_file(cur_file, dest_dir, arena_local);
		cur_file = kcl_lst_get_next(file_list);
	}

	kcl_arn_free(arena_local);
}

