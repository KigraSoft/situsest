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

[[maybe_unused]]
static void
diag_print_fnode(char *preface, struct file_list_node *fnode)
{
	printf("%s %s \t- %li, %li, %li\n", preface, fnode->lname, fnode->lname_len, fnode->dname_len, fnode->fname_len);
}

[[maybe_unused]]
static void
diag_print_file_list(struct kcl_list *file_list)
{
	struct file_list_node *cur_file = kcl_lst_get_first(file_list);
	printf("DIAG:  Print file list:\n");
	while (cur_file != NULL) {
		printf("%s\n", cur_file->lname);
		cur_file = kcl_lst_get_next(file_list);
	
	}
}

[[maybe_unused]]
static void
diag_print_file_list_2(struct kcl_list *file_list)
{
	struct file_list_node2 *cur_file = kcl_lst_get_first(file_list);
	printf("DIAG:  Print file list 2\n");
	while (cur_file != NULL) {
		printf("%s : %s - %s\n",
		       kcl_str_cstr_alloc(cur_file->lname),
		       kcl_str_cstr_alloc(cur_file->dname),
		       kcl_str_cstr_alloc(cur_file->fname)
			);
		cur_file = kcl_lst_get_next(file_list);
	
	}
}

static bool
get_file_list_regex(char *dir, struct kcl_list *files, regex_t *rfunc, struct kcl_arena *arena)
{
	char *dir_str = dir;
	size_t dir_str_len = strlen(dir_str);
	if (dir_str[dir_str_len - 1] != '/') {
		dir_str = kcl_arn_push(arena, dir_str_len + 2);
		memcpy(dir_str, dir, dir_str_len);
		memcpy(dir_str + dir_str_len, "/", 2);
		dir_str_len++;
	}

	struct dirent *ep;
	DIR *dp = opendir(dir);
	if (dp != NULL) {
		while ((ep = readdir(dp))) {
			if (ep->d_name[0] != '.') {
				size_t fname_len = strlen(ep->d_name);
				if (ep->d_type == DT_DIR) {
					char *sub_dir_str = kcl_arn_push(arena, (dir_str_len + fname_len + 1) * sizeof (char));
					if (sub_dir_str) {
						memcpy(sub_dir_str, dir_str, dir_str_len);
						memcpy(sub_dir_str + dir_str_len, ep->d_name, fname_len + 1);
						get_file_list_regex(sub_dir_str, files, rfunc, arena);
					} else { return (false); }
				} else if (!regexec(rfunc, ep->d_name, 0, NULL, 0)) {
					struct file_list_node *file = kcl_arn_push(files->arena, sizeof (struct file_list_node));
					file->dname_len = dir_str_len;
					file->fname_len = fname_len;
					file->lname_len = file->dname_len + file->fname_len;
					file->lname = kcl_arn_push(files->arena, file->lname_len + 1);
					if (file->lname) {
						file->fname = file->lname + dir_str_len;
						memcpy(file->lname, dir_str, dir_str_len);
						memcpy(file->fname, ep->d_name, fname_len + 1);
						kcl_lst_add_datum(files, (void *) file);
					} else { return (false); }
				}
			}
		}
		closedir(dp);
	} else { return (false); }

	return (true);
}

static bool
get_file_list_regex_2(char *dir, struct kcl_list *files, regex_t *rfunc, struct kcl_arena *arena)
{
	char *dir_str = dir;
	size_t dir_str_len = strlen(dir_str);
	if (dir_str[dir_str_len - 1] != '/') {
		dir_str = kcl_arn_push(arena, dir_str_len + 2);
		memcpy(dir_str, dir, dir_str_len);
		memcpy(dir_str + dir_str_len, "/", 2);
		dir_str_len++;
	}

	struct dirent *ep;
	DIR *dp = opendir(dir);
	if (dp != NULL) {
		while ((ep = readdir(dp))) {
			if (ep->d_name[0] != '.') {
				size_t fname_len = strlen(ep->d_name);
				if (ep->d_type == DT_DIR) {
					char *sub_dir_str = kcl_arn_push(arena, (dir_str_len + fname_len + 1) * sizeof (char));
					if (sub_dir_str) {
						memcpy(sub_dir_str, dir_str, dir_str_len);
						memcpy(sub_dir_str + dir_str_len, ep->d_name, fname_len + 1);
						get_file_list_regex_2(sub_dir_str, files, rfunc, arena);
					} else { return (false); }
				} else if (!regexec(rfunc, ep->d_name, 0, NULL, 0)) {
					struct file_list_node2 *file = kcl_arn_push(files->arena, sizeof (struct file_list_node2));
					file->lname = kcl_str_concat_new(dir_str, dir_str_len, ep->d_name, fname_len, arena);
					file->dname = kcl_str_slice_new(file->lname, 0, dir_str_len, arena);
					file->fname = kcl_str_slice_new(file->lname, dir_str_len, fname_len, arena);
					kcl_lst_add_datum(files, (void *) file);
				}
			}
		}
		closedir(dp);
	} else { return (false); }

	return (true);
}

