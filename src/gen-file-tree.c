/*
  situsest - The Text File Publisher

  gen-file-tree.c
  
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
#include <fnmatch.h>
#include <dirent.h>

/*
[[maybe_unused]]
static void
diag_print_fnode(char *preface, struct file_list_node *fnode)
{
	printf("%s %s \t- %li, %li, %li\n", preface, fnode->lname, fnode->lname_len, fnode->dname_len, fnode->fname_len);
}
*/
static void
diag_print_file_list(struct kcl_list *file_list, char* title_str, kcl_arena* arena)
{
	struct file_list_node *cur_file = kcl_lst_get_first(file_list);
	printf("DIAG:  Print file list for: %s\n", title_str);
	while (cur_file != NULL) {
		printf("%s : %s - %s\n",
		       kcl_str_to_cstr_new(cur_file->lname, arena),
		       kcl_str_to_cstr_new(cur_file->dname, arena),
		       kcl_str_to_cstr_new(cur_file->fname, arena)
			);
		cur_file = kcl_lst_get_next(file_list);
	
	}
}

static bool
gen_file_lists_scandir(char* dir, kcl_arena *arena_local) {
	char* dir_str = dir;
	size_t dir_str_len = strlen(dir_str);
	struct dirent* entry;
	size_t fname_len;
	char* sub_dir_str;
	DIR* dir_list = opendir(dir_str);
	if (!dir_list) {
		return (false);
	} else {
		while ((entry = readdir(dir_list))) {
			if (entry->d_name[0] != '.') {
				fname_len = strlen(entry->d_name);
				if (entry->d_type == DT_DIR) {
					sub_dir_str = kcl_arn_push(arena_local, (dir_str_len + fname_len + 2)); // arena_func
					if (sub_dir_str) {
						memcpy(sub_dir_str, dir_str, dir_str_len);
						memcpy(sub_dir_str + dir_str_len, entry->d_name, fname_len);
						sub_dir_str[dir_str_len + fname_len] = '/';
						sub_dir_str[dir_str_len + fname_len + 1] = 0;
						gen_file_lists_scandir(sub_dir_str, arena_local);
					} else { return (false); }
				} else {
					struct file_list_node *file = kcl_arn_push(gstate.files_all->arena, sizeof (struct file_list_node));
					if (file) {
						file->lname = kcl_str_concat_new(dir_str, dir_str_len, entry->d_name, fname_len, gstate.files_all->arena);
						file->dname = kcl_str_slice_new(file->lname, 0, dir_str_len, gstate.files_all->arena);
						file->fname = kcl_str_slice_new(file->lname, dir_str_len, fname_len, gstate.files_all->arena);
						if (file->fname) {
							kcl_lst_add_datum(gstate.files_all, (void *)file);
							if (!regexec(&(gstate.rgx_raw_files), entry->d_name, 0, NULL, 0)) {
								kcl_lst_add_datum(gstate.files_raw, (void *)file);
							}
							if (!regexec(&(gstate.rgx_org_files), entry->d_name, 0, NULL, 0)) {
								kcl_lst_add_datum(gstate.files_org, (void *)file);
							}
						} else { return (false); }
					} else { return (false); }
				}
			}
		}
		closedir(dir_list);
	}

	return (true);
}

static bool
gen_file_lists(kcl_arena* arena_lnklst)
{
	kcl_arena* arena_local = kcl_arn_alloc(STACKPLUS, 4048, 4048, true);
	printf(">> %p\n", arena_local);
	
	gstate.files_all = kcl_lst_alloc_list(LNKLST, arena_lnklst, 0);
	gstate.files_raw = kcl_lst_alloc_list(LNKLST, arena_lnklst, 0);
	gstate.files_org = kcl_lst_alloc_list(LNKLST, arena_lnklst, 0);

	char* dir_str = gstate.input_dir;
	size_t dir_str_len = strlen(dir_str);
	if (dir_str[dir_str_len - 1] != '/') {
		dir_str = kcl_arn_push(arena_local, dir_str_len + 2); // could be arena_lcl
		memcpy(dir_str, gstate.input_dir, dir_str_len);
		memcpy(dir_str + dir_str_len, "/", 2);
		dir_str_len++;
	}

	gen_file_lists_scandir(dir_str, arena_local);

	if (gstate.diagnostics) {
		kcl_arn_mem_display(arena_local, (uintptr_t)arena_local, 128);
	}
	
	kcl_arn_free(arena_local);

	return (true);
}
									   
