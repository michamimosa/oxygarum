/**
 *  include/scene.h
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

#ifndef _SCENE_H
#define _SCENE_H

#include "object.h"
#include "particle.h"
#include "physics.h"
#include "light.h"
#include "font.h"
#include "list.h"
#include "logger.h"
#include "vector.h"
#include "volumebox.h"

namespace oxygarum
{

class SceneNode : public Transformation3D, public VolumeBox
{
    public:
        SceneNode();
        ~SceneNode();

        List<SceneNode> *subnodes;
        List<Object3D> *objects3D;
        List<Object2D> *objects2D;
        //List<Text> *texts;
        //List<ParticleEmitter> *particle_emitters;
        List<Light> *lights;

        ListEntry<Object3D> *addObject3D(Object3D *object);
        ListEntry<Object2D> *addObject2D(Object3D *object);
        //ListEntry *addText(Text *text);
        //ListEntry *addParticleEmitter(ParticleEmitter *emitter);
        //ListEntry *addLight(Light *light);

        void calcVolumeBox(void);

        void render3D(void);
        void render2D(void);

        static Logger *logger;
};

/**
 * @class Scene
 * @brief Class for a set of objects
 */
class Scene
{
    public:
        Scene();
        ~Scene();

        SceneNode *base_node;

        ListEntry<Object3D> *addObject3D(Object3D *object);
        ListEntry<Object2D> *addObject2D(Object3D *object);
        //ListEntry *addText(Text *text);
        //ListEntry *addParticleEmitter(ParticleEmitter *emitter);
        //ListEntry *addLight(Light *light);

        void render3D(void);
        void render2D(void);

        static Logger *logger;
};

};

#endif

