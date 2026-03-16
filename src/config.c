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

static char* raw[] = { "html", "js", "css" };
static char raw_num = sizeof (raw) / sizeof (raw[0]);

static char* org[] = { "org" };
static char org_num = sizeof (org) / sizeof (org[0]);

void
get_site_config(kcl_arena* arena)
{
	kcl_arena *arena_lcl = kcl_arn_alloc(STACKPLUS, 4048, 4048, true);
	struct kcl_list *files_raw = kcl_lst_alloc_list(LNKLST, arena_lcl, 0);
	struct kcl_list *files_org = kcl_lst_alloc_list(LNKLST, arena_lcl, 0);

	// I am doing some extra steps here becuase because source data will
	// eventually be read from a config file into kcl_str, etc.
	for (unsigned i = 0; i < raw_num; i++) {
		//printf("Raw file import: %s\n", raw[i]);
		kcl_lst_add_datum(files_raw, (void *)kcl_str_new(raw[i], strlen(raw[i]), arena_lcl));
	}
	for (unsigned i = 0; i < org_num; i++) {
		kcl_lst_add_datum(files_org, (void *)kcl_str_new(org[i], strlen(org[i]), arena_lcl));
	}

	kcl_str *raw_ext_list = kcl_str_new(".*\\.(", 256, arena_lcl);
	kcl_str_append(raw_ext_list, kcl_lst_get_first(files_raw));
	kcl_str *cur_str = kcl_lst_get_next(files_raw);
	while (cur_str) {
		kcl_str_append_cstr(raw_ext_list, "|");
		kcl_str_append(raw_ext_list, cur_str);
		cur_str = kcl_lst_get_next(files_raw);
	}
	kcl_str_append_cstr(raw_ext_list, ")$");
	regcomp(&(gstate.rgx_raw_files), kcl_str_to_cstr_new(raw_ext_list, arena_lcl), REG_NOSUB | REG_EXTENDED);

	kcl_str *org_ext_list = kcl_str_new(".*\\.(", 256, arena_lcl);
	kcl_str_append(org_ext_list, kcl_lst_get_first(files_org));
	cur_str = kcl_lst_get_next(files_org);
	while (cur_str) {
		kcl_str_append_cstr(org_ext_list, "|");
		kcl_str_append(org_ext_list, cur_str);
		cur_str = kcl_lst_get_next(files_org);
	}
	kcl_str_append_cstr(org_ext_list, ")$");
	regcomp(&(gstate.rgx_org_files), kcl_str_to_cstr_new(org_ext_list, arena_lcl), REG_NOSUB | REG_EXTENDED);

	kcl_arn_free(arena_lcl);
}

