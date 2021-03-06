/**
 *  include/material.h
 *
 *  Copyright (C) 2013-2014 Michael Sippel
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

/**
 * @author Michael Sippel <michamimosa@gmail.com>
 */

#ifndef _MATERIAL_H
#define _MATERIAL_H

#include <GL/glew.h>
#include <GL/gl.h>

#include "logger.h"
#include "list.h"
#include "texture.h"
#include "shader.h"

namespace oxygarum
{

class Color
{
    public:
        Color();
        Color(float r_, float g_, float b_);
        Color(float r_, float g_, float b_, float a_);
        ~Color();

        float r,g,b,a;

        void use(void);
};

typedef struct mapped_texture
{
    Texture *texture;

    GLint location;
    GLint mapping;
} mapped_texture_t;

class Material
{
    public:
        Material();
        Material(Color color_);
        ~Material();

        List<mapped_texture> *textures;

        Color color;
        float roughness;
        float emission;
        float refractivity;

        GLfloat gl_ambient[4];
        GLfloat gl_diffuse[4];
        GLfloat gl_specular[4];
        GLfloat gl_emission[4];
        GLfloat gl_shininess[1];

        ShadeProgram *shade_program;

        void update_values(void);
        GLint map_texture(Texture *tex, char *name, GLint mapping);
        void use(void);

        static Logger *logger;
};

};

#endif

