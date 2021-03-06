/**
 *  include/object.h
 *
 *  Copyright (C) 2012-2014 Michael Sippel
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

#ifndef _OBJECT_H
#define _OBJECT_H

#include "logger.h"
#include "vector.h"
#include "transformation.h"
#include "mesh.h"
#include "material.h"
#include "volumebox.h"

namespace oxygarum
{

#define OBJECT_VISIBLE         0x1
#define OBJECT_TRANSPARENT     0x2
#define OBJECT_DEPTH_BUFFERING 0x4
#define OBJECT_RENDER_VBO      0x8

/**
 * @class Object3D
 * @brief blah
 */
class Object3D : public Transformation3D
{
    public:
        Object3D();
        Object3D(Transformation3D transform);
        Object3D(Vector3D position_);
        Object3D(Vector3D position_, Vector3D rotation_);
        ~Object3D();

        Mesh3D *mesh;
        Material *material;
        Vector3D feedback;

        void render(void);
        void setFlag(int flag);
        void removeFlag(int flag);
        int getStatus(void);

        static Logger *logger;

    private:
        int status;
};

/**
 * @class Object2D
 * @brief blah
 */
class Object2D : public Transformation2D
{
    public:
        Object2D();
        Object2D(Transformation2D transform);
        Object2D(Vector2D position_);
        Object2D(Vector2D position_, float rotation_);
        ~Object2D();

        Mesh2D *mesh;
        Material *material;

        void render(void);
        void setFlag(int flag);
        void removeFlag(int flag);
        int getStatus(void);

        static Logger *logger;

    private:
        int status;
};

};

#endif

