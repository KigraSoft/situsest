/*
  situsest - The Text File Publisher

  encdr-html.c
  
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
  along with GNU Emacs.  If not, see
  <https://www.gnu.org/licenses/>.
*/

#include "situsest.h"
#include <string.h>
// #include <fnmatch.h>

/*
struct file_list_node {
	char   *lname;     // file name with full dir path
	char   *fname;     // point to file name section
	size_t  lname_len; // len of entire full name
	size_t  dname_len; // len of just dir portion of lname
	size_t  fname_len; // len of just fname portion of lname
	struct file_list_node *next_file;
};
*/

static int
encode_html(struct file_list_node *fnode)
{
	printf("%s - %s\n", fnode->fname, fnode->lname);
	//printf("-- %s \n", fnode->lname);
}

/*
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

int
diag_print_file_list(struct file_list_node *file_list)
{
	struct file_list_node *cur_file = file_list;
	printf("DIAG:  Print file list\n");
	while (cur_file != NULL) {
		// this test is needed if last file in root is a dir with no matching files
		if (cur_file->lname_len > 0) {
			printf("%s\n", cur_file->lname);
		}
		cur_file = cur_file->next_file;
	
	}

	return 0;
}

void
print_fnode(char *preface, struct file_list_node *fnode)
{
	printf("%s %s \t- %i, %i, %i\n", preface, fnode->lname, fnode->lname_len, fnode->dname_len, fnode->fname_len);
}
	
struct file_list_node *
get_file_list(char *dir, struct file_list_node *files, char *pattern)
{
	struct file_list_node *cur_file;
	struct dirent **dir_list;
	size_t dir_entry_len, fname_len;
	size_t dir_str_len = strlen(dir) + 1;
	char  *dir_str = malloc(dir_str_len * sizeof (char));
	char  *sub_dir_str;
	mempcpy(mempcpy(dir_str, dir, dir_str_len - 1), "/", sizeof (char));

	cur_file = files;

	int i = 0;
	gstate.cur_pattern = pattern;
	int n = scandir(dir, &dir_list, file_filter, alphasort);
	if (n >= 0 ) {
		while (i < n) {
			cur_file->dname_len = dir_str_len;
			cur_file->fname_len = strlen(dir_list[i]->d_name);
			if (dir_list[i]->d_type == DT_DIR) {
				sub_dir_str = malloc((cur_file->dname_len + cur_file->fname_len + 1) * sizeof (char));
				mempcpy(mempcpy(sub_dir_str, dir_str, dir_str_len), dir_list[i]->d_name, cur_file->fname_len + 1);
				cur_file = get_file_list(sub_dir_str, cur_file, pattern);
				free(sub_dir_str);
			} else {
				cur_file->lname_len = cur_file->dname_len + cur_file->fname_len;
				cur_file->lname = malloc((cur_file->lname_len + 1) * sizeof (char));
				cur_file->fname = mempcpy(cur_file->lname, dir_str, dir_str_len);
				memcpy(cur_file->fname, dir_list[i]->d_name, cur_file->fname_len + 1);
			}
				i++;
				// lname_len test needed if dir_list[i] was a dir with no matching files
				if ((i < n) && (cur_file->lname_len > 0)) {
					cur_file->next_file = calloc(1, sizeof (struct file_list_node));
					cur_file = cur_file->next_file;
			}
		}
	}

	free(dir_str);
	return cur_file;
}
*/
