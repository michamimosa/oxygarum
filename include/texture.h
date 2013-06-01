/**
 *  src/texture.h
 *
 *  (C) Copyright 2013 Michael Sippel
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <stdint.h>

typedef unsigned int texture_id;

typedef struct uv_t {
  float u;
  float v;
} uv_t;

typedef struct texture {
  unsigned int width;
  unsigned int height;
  
  uint8_t *data;
  texture_id id;
} texture_t;

uint8_t *oxygarum_load_png(const char *path, unsigned int *width, unsigned int *height);
texture_t *oxygarum_load_texture(const char *path);

#endif

