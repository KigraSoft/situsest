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

struct file_list_node {
	char   *lname;     // file name with full dir path
	char   *fname;     // pointer to file name section
	size_t  lname_len; // len of entire full name
	size_t  dname_len; // len of just dir portion of lname
	size_t  fname_len; // len of just fname portion of lname
};
	
static int
one(const struct dirent *entry)
{
	if (fnmatch("*", entry->d_name, FNM_PERIOD)) {
		return 0;
	} else {
		return 1;
	}
}

static int
file_filter(const struct dirent *entry)
{
	if (fnmatch("*", entry->d_name, FNM_PERIOD)) {
		return 0;
	} else {
		if (entry->d_type == DT_DIR) {
			return 1;
		} else {
			if (fnmatch(gstate.cur_pattern, entry->d_name, FNM_PERIOD)) {
				return 0;
			} else {
				return 1;
			}
		}
	}
}

static void
print_fnode(char *preface, struct file_list_node *fnode)
{
	printf("%s %s \t- %li, %li, %li\n", preface, fnode->lname, fnode->lname_len, fnode->dname_len, fnode->fname_len);
}

static void
diag_print_file_list(struct kcl_list *file_list)
{
	struct file_list_node *cur_file = kcl_lst_get_first(file_list);
	printf("DIAG:  Print file list 2\n");
	while (cur_file != NULL) {
		printf("%s\n", cur_file->lname);
		cur_file = kcl_lst_get_next(file_list);
	
	}
}

static bool
get_file_list(char *dir, struct kcl_list *files, char *pattern, struct kcl_arena *arena)
{
	struct file_list_node *file;
	struct dirent **dir_list;
	char  *dir_str;
	char  *sub_dir_str;
	size_t fname_len;
	size_t dir_str_len = strlen(dir);
	if (dir[dir_str_len - 1] == '/') {
		dir_str = dir;
	} else {
		dir_str = kcl_arn_push(arena, dir_str_len + 2);
		memcpy(dir_str, dir, dir_str_len);
		memcpy(dir_str + dir_str_len, "/", 2);
		dir_str_len++;
	}

	gstate.cur_pattern = pattern;
	int n = scandir(dir, &dir_list, file_filter, alphasort);
	for (uint i = 0; i < n; i++) {
		fname_len = strlen(dir_list[i]->d_name);
		if (dir_list[i]->d_type == DT_DIR) {
			sub_dir_str = kcl_arn_push(arena, (dir_str_len + fname_len + 1) * sizeof (char));
			if (sub_dir_str) {
				memcpy(sub_dir_str, dir_str, dir_str_len);
				memcpy(sub_dir_str + dir_str_len, dir_list[i]->d_name, fname_len + 1);
				get_file_list(sub_dir_str, files, pattern, arena);
			} else { return (false); }
		} else {
			file = kcl_arn_push(files->arena, sizeof (struct file_list_node));
			file->dname_len = dir_str_len;
			file->fname_len = fname_len;
			file->lname_len = file->dname_len + file->fname_len;
			file->lname = kcl_arn_push(files->arena, file->lname_len + 1);
			if (file->lname) {
				file->fname = file->lname + dir_str_len;
				memcpy(file->lname, dir_str, dir_str_len);
				memcpy(file->fname, dir_list[i]->d_name, fname_len + 1);
				kcl_lst_add_datum(files, (void *) file);
			} else { return (false); }
		}
	}

	return (true);
}
