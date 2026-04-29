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
		printf("%s : %s - %s - %s\n",
		       kcl_str_to_cstr_new(cur_file->src_path, arena),
		       kcl_str_to_cstr_new(cur_file->src_sub_dir, arena),
		       kcl_str_to_cstr_new(cur_file->file_name, arena),
		       kcl_str_to_cstr_new(cur_file->output_file_str, arena)
			);
		cur_file = kcl_lst_get_next(file_list);
	
	}
}

static bool
gen_file_lists_scandir(char* src_dir_str, size_t src_dir_str_len, kcl_arena *arena_local) {
	//size_t input_dir_str_len = strlen(gstate.input_dir);
	size_t input_dir_str_len = gstate.input_dir_str_len;
	struct dirent* entry;
	size_t file_name_len;
	char* sub_dir_str;
	size_t sub_dir_str_len;
	DIR* dir_list = opendir(src_dir_str);
	if (!dir_list) {
		return (false);
	} else {
		while ((entry = readdir(dir_list))) {
			if (entry->d_name[0] != '.') {
				file_name_len = strlen(entry->d_name);
				if (entry->d_type == DT_DIR) {
					//sub_dir_str = kcl_arn_push(arena_local, (src_dir_str_len + file_name_len + 2)); // arena_func
					sub_dir_str_len = src_dir_str_len + file_name_len + 2;
					sub_dir_str = kcl_arn_push(arena_local, sub_dir_str_len);
					if (sub_dir_str) {
						memcpy(sub_dir_str, src_dir_str, src_dir_str_len);
						memcpy(sub_dir_str + src_dir_str_len, entry->d_name, file_name_len);
						sub_dir_str[src_dir_str_len + file_name_len] = '/';
						sub_dir_str[src_dir_str_len + file_name_len + 1] = 0;
						gen_file_lists_scandir(sub_dir_str, sub_dir_str_len, arena_local);
					} else { return (false); }
				} else {
					struct file_list_node *file = kcl_arn_push(gstate.files_all->arena, sizeof (struct file_list_node));
					if (file) {
						file->src_path = kcl_str_concat_new(src_dir_str, src_dir_str_len, entry->d_name, file_name_len, gstate.files_all->arena);
						file->src_dir = kcl_str_slice_new(file->src_path, 0, src_dir_str_len, gstate.files_all->arena);
						file->src_sub_dir = kcl_str_slice_new(file->src_path, input_dir_str_len, kcl_str_len(file->src_dir) - input_dir_str_len, gstate.files_all->arena);
						file->file_name = kcl_str_slice_new(file->src_path, src_dir_str_len, file_name_len, gstate.files_all->arena);
						file->output_file_str = kcl_str_new(gstate.output_dir, strlen(gstate.output_dir) + kcl_str_len(file->src_path), gstate.files_all->arena);
						kcl_str_append(file->output_file_str, "/");
						kcl_str_append(file->output_file_str, file->src_sub_dir);
						kcl_str_append(file->output_file_str, file->file_name);
						if (file->file_name) {
							kcl_lst_add_datum(gstate.files_all, (void *)file);
							if (!regexec(&(gstate.rgx_raw_files), entry->d_name, 0, NULL, 0)) {
								kcl_lst_add_datum(gstate.files_raw, (void *)file);
							}
							if (!regexec(&(gstate.rgx_org_files), entry->d_name, 0, NULL, 0)) {
								file->output_file_str->str[file->output_file_str->len - 3] = 'h';
								file->output_file_str->str[file->output_file_str->len - 2] = 't';
								file->output_file_str->str[file->output_file_str->len - 1] = 'm';
								file->output_file_str->str[file->output_file_str->len - 0] = 'l';
								file->output_file_str->len++;
								kcl_lst_add_datum(gstate.files_org, (void *)file);
							}
							if (!regexec(&(gstate.rgx_template_files), entry->d_name, 0, NULL, 0)) {
								kcl_lst_add_datum(gstate.files_templates, (void *)file);
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
	
	gstate.files_all = kcl_lst_alloc_list(LNKLST, arena_lnklst, 0);
	gstate.files_raw = kcl_lst_alloc_list(LNKLST, arena_lnklst, 0);
	gstate.files_org = kcl_lst_alloc_list(LNKLST, arena_lnklst, 0);
	gstate.files_templates = kcl_lst_alloc_list(LNKLST, arena_lnklst, 0);

	char* src_dir_str = gstate.input_dir;
	size_t src_dir_str_len = gstate.input_dir_str_len;
	//size_t src_dir_str_len = strlen(src_dir_str);
	if (src_dir_str[src_dir_str_len - 1] != '/') {
		src_dir_str = kcl_arn_push(arena_local, src_dir_str_len + 2); // could be arena_lcl
		memcpy(src_dir_str, gstate.input_dir, src_dir_str_len);
		memcpy(src_dir_str + src_dir_str_len, "/", 2);
		src_dir_str_len++;
	}

	gen_file_lists_scandir(src_dir_str, src_dir_str_len, arena_local);

	if (gstate.diagnostics) {
		kcl_arn_mem_display(arena_local, (uintptr_t)arena_local, 128);
	}
	
	kcl_arn_free(arena_local);

	return (true);
}
									   
