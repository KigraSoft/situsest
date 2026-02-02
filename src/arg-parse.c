/*
  situsest - The Text File Publisher

  arg-parse.c
  
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

const char * argp_program_version = PACKAGE_STRING;
const char * argp_program_bug_address = PACKAGE_BUGREPORT;

static char doc[] = "Example:  situsest -o public_www site";

static char args_doc[] = "[SOURCE_DIR]";

static struct argp_option options[] = {
	{"verbose", 'v', 0, 0, "Produce verbose output" },
	{"quiet",   'q', 0, 0, "Do not produce any output" },
	{"silent",  's', 0, OPTION_ALIAS },
	{"output",  'o', "FILE", 0, "Output to FILE" },
	{0}
};

static error_t
parse_opt(int key, char *arg, struct argp_state *state)
{
	struct arguments *arguments = state->input;

	switch (key)
	{
	case 'q': case 's':
		gstate.silent = 1;
		break;
	case 'v':
		gstate.verbose = 1;
		break;
	case 'o':
		gstate.output_dir = arg;
		break;
	case ARGP_KEY_ARG:
		if (state->arg_num >= 1)
			argp_usage(state);
		gstate.input_dir = arg;
		break;
	case ARGP_KEY_END:
		break;
	default:
		return ARGP_ERR_UNKNOWN;
	}

	return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc };

