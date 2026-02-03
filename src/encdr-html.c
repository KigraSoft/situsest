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

static int
encode_html(struct file_list_node *fnode)
{
	printf("%s - %s\n", fnode->fname, fnode->lname);
	//printf("-- %s \n", fnode->lname);
}

