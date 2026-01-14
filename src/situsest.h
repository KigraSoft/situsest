/*
  situsest - The Text File Publisher

  situsest.h
  
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

#ifndef SITUSEST_H
#define SITUSEST_H

struct gstate {
	int silent, verbose;
	char *input_dir;
	char *output_dir;
	char **file_list;
	char *cur_pattern;
};

#endif /* not SITUSEST_H */
