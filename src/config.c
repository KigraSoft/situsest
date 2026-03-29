/*
  situsest - The Text File Publisher

  config.c
  
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

#define CFG_MAX_RGX_STR_SIZE 1024

static char* raw[] = { "html", "js", "css", "png", "jpg", "gif" };
static char raw_num = sizeof (raw) / sizeof (raw[0]);

static char* org[] = { "org" };
static char org_num = sizeof (org) / sizeof (org[0]);

static char* template[] = { "seht" };
static char template_num = sizeof (template) / sizeof (template[0]);

bool
compile_file_ext_rgx(char* array[], char array_size, regex_t* file_ext_rgx, kcl_arena* arena)
{
	kcl_list *file_list = kcl_lst_alloc_list(LNKLST, arena, 0);
	if (!file_list) { return false; }

	for (unsigned char i = 0; i < array_size; i++) {
		kcl_lst_add_datum(file_list, (void *)kcl_str_new(array[i], strlen(array[i]), arena));
	}

	kcl_str *file_ext_list = kcl_str_new(".*\\.(", CFG_MAX_RGX_STR_SIZE, arena);
	if (!file_ext_list) { return false; }
	kcl_str_append(file_ext_list, (kcl_str*)kcl_lst_get_first(file_list));
	kcl_str *cur_str = kcl_lst_get_next(file_list);
	while (cur_str) {
		kcl_str_append_cstr(file_ext_list, "|");
		kcl_str_append(file_ext_list, cur_str);
		cur_str = kcl_lst_get_next(file_list);
	}
	kcl_str_append_cstr(file_ext_list, ")$");
	if (regcomp(file_ext_rgx, kcl_str_to_cstr_new(file_ext_list, arena), REG_NOSUB | REG_EXTENDED)) {
		return false;
	} else {
		return true;
	}
}

bool
get_site_config(kcl_arena* arena)
{
	kcl_arena *arena_lcl = kcl_arn_alloc(STACKPLUS, CFG_MAX_RGX_STR_SIZE * 4, CFG_MAX_RGX_STR_SIZE * 4, true);
	if (!arena_lcl) { return true; }

	if (compile_file_ext_rgx(raw, raw_num, &(gstate.rgx_raw_files), arena_lcl) &&
	    compile_file_ext_rgx(org, org_num, &(gstate.rgx_org_files), arena_lcl) &&
	    compile_file_ext_rgx(template, template_num, &(gstate.rgx_template_files), arena_lcl)) {
		kcl_arn_free(arena_lcl);
		return true;
	} else {
		return false;
	}
}

