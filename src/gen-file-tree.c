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
	printf("%s %s \t- %i, %i, %i\n", preface, fnode->lname, fnode->lname_len, fnode->dname_len, fnode->fname_len);
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

static void
get_file_list(char *dir, struct kcl_list *files, char *pattern)
{
	struct file_list_node *file;
	struct dirent **dir_list;
	size_t dir_entry_len, fname_len;
	size_t dir_str_len = strlen(dir) + 1;
	char  *dir_str = malloc(dir_str_len * sizeof (char));
	char  *sub_dir_str;
	mempcpy(mempcpy(dir_str, dir, dir_str_len - 1), "/", sizeof (char));

	gstate.cur_pattern = pattern;
	int n = scandir(dir, &dir_list, file_filter, alphasort);
	for (uint i = 0; i < n; i++) {
		fname_len = strlen(dir_list[i]->d_name);
		if (dir_list[i]->d_type == DT_DIR) {
			// should I use an arena for below but maybe not the same as the files list because of different timeline?
			sub_dir_str = malloc((dir_str_len + fname_len + 1) * sizeof (char));
			mempcpy(mempcpy(sub_dir_str, dir_str, dir_str_len), dir_list[i]->d_name, fname_len + 1);
			get_file_list(sub_dir_str, files, pattern);
			free(sub_dir_str);
		} else {
			file = kcl_arn_push(files->arena, sizeof (struct file_list_node));
			//file = (struct file_list_node *){ 0 };
			file->dname_len = dir_str_len;
			file->fname_len = strlen(dir_list[i]->d_name);
			file->lname_len = file->dname_len + file->fname_len;
			file->lname = malloc((file->lname_len + 1) * sizeof (char));
			file->fname = mempcpy(file->lname, dir_str, dir_str_len);
			memcpy(file->fname, dir_list[i]->d_name, file->fname_len + 1);
			kcl_lst_add_datum(files, (void *) file);
		}
	}

	free(dir_str);
}
