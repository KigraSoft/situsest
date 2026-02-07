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
  along with Situsest.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "situsest.h"
#include <string.h>

static int
encode_html(struct file_list_node *fnode)
{
	printf("%s - %s\n", fnode->fname, fnode->lname);
	//printf("-- %s \n", fnode->lname);
	/*
	  algorithm thoughts

	  This encodes a single html file.
	  Initial plan is just to copy html files with no change or processing.
	  Should overwrite be default?

	  Check for overwrite or sync flags
	  If overwrite
	    Copy over file overwriting any file that already exists
	  If sync
	    Check if dest file exists
	      If exists
	        Compare files
		  If different overwrite dest file with new source
		  Else skip file
	      Else copy source file
	 Else ?? any other mode ??

	 How do I clean up files that exist in dest but no longer in source?
	 Opt 1: Gen a list of every file in dest; remove from list as file processed; delete any leftovers
	 Opt 2: Gen a list of files as processed; use list to parse dest, deleting any not on list
	 Opt 3: ?
}

