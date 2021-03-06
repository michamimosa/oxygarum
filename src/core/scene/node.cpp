/**
 *  src/core/node.cpp
 *
 *  (C) Copyright 2013-2014 Michael Sippel
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
#include <stdio.h>
#include <stdlib.h>

#include "scene.h"
#include "object.h"
#include "particle.h"
#include "font.h"
#include "light.h"
#include "math.h"

namespace oxygarum
{

SceneNode::SceneNode()
{
    Transformation3D();

    this->objects3D = new List<Object3D>();
    //this->objects2D = new List<Object2D>();
    //this->texts = new List<Text>();
    //this->particle_emitters = new List<ParticleEmitter>();
    this->lights = new List<Light>();

    this->subnodes = new List<SceneNode>();
}

SceneNode::~SceneNode()
{
    delete this->objects3D;
    //delete this->objects2D;
    //delete this->texts;
    //delete this->particle_emitters;
    delete this->lights;
    delete this->subnodes;
}

void SceneNode::render3D(void)
{
    // update lights
    ListEntry<Light> *l_entry = this->lights->getHead();

    while(l_entry != NULL)
    {
        //this->logger->log(INFO, "rendering light \"%s\"", l_entry->name);
        Light *light = l_entry->element;
        if(light == NULL)
        {
            continue;
        }

        glPushMatrix();

        light->useTransformation();
        light->update();

        glPopMatrix();

        l_entry = l_entry->getNext();
    }

    // render 3D-Objects
    ListEntry<Object3D> *o_entry = this->objects3D->getHead();

    while(o_entry != NULL)
    {
        //this->logger->log(INFO, "rendering object \"%s\"", o_entry->name);
        Object3D *obj = o_entry->element;
        if(obj == NULL)
        {
            continue;
        }

        if(obj->getStatus() & OBJECT_VISIBLE)
        {
            glPushMatrix();
            glPushAttrib(GL_ENABLE_BIT);

            obj->useTransformation();
            obj->render();

            glPopAttrib();
            glPopMatrix();
        }

        o_entry = o_entry->getNext();
    }
    /*
      // render particles
      entry = scene->particle_emitters->head;
      while(entry != NULL) {
        particle_emitter_t *emitter = (particle_emitter_t*) entry->element;
        if(emitter == NULL) {
          continue;
        }

        glPushMatrix();
          glTranslatef(emitter->pos.x, emitter->pos.y, emitter->pos.z);
          oxygarum_render_particle_system(emitter);
        glPopMatrix();

        entry = entry->next;
      }*/

    glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
    this->drawVolumeBox();

    // render subnodes
    ListEntry<SceneNode> *s_entry = this->subnodes->getHead();
    while(s_entry != NULL)
    {
        SceneNode *subnode = s_entry->element;
        if(subnode != NULL)
        {
            glPushMatrix();
            subnode->useTransformation();
            subnode->render3D();
            glPopMatrix();
        }
        s_entry = s_entry->getNext();
    }
}

void SceneNode::render2D(void)
{
    /*  group_entry_t *entry;

      // render 2D-Objects
      entry = scene->objects2d->head;
      while(entry != NULL) {
        object2d_t *obj = (object2d_t*) entry->element;
        if(obj == NULL) {
          continue;
        }

        if(obj->status & OBJECT_VISIBLE) {
          glPushMatrix();
            oxygarum_render_object2d(obj);
          glPopMatrix();
        }

        entry = entry->next;
      }

      // render texts
      entry = scene->texts->head;
      while(entry != NULL) {
        text_t *text = (text_t*) entry->element;
        if(text == NULL) {
          continue;
        }

        glPushMatrix();
          oxygarum_render_text(text);
        glPopMatrix();

        entry = entry->next;
      }*/
}

void SceneNode::calcVolumeBox(void)
{
    this->VolumeBox::calcVolumeBox();

    // check subnodes
    ListEntry<SceneNode> *s_entry = this->subnodes->getHead();
    while(s_entry != NULL)
    {
        SceneNode *subnode = s_entry->element;
        if(subnode != NULL)
        {
            subnode->calcVolumeBox();
            Vector3D m1 = subnode->box_size1;
            Vector3D m2 = subnode->box_size2;
            m1.rotate(*subnode->rotation);
            m2.rotate(*subnode->rotation);
            m1.add(*subnode->position);
            m2.add(*subnode->position);
            this->box_size1.min(m1);
            this->box_size1.max(m2);
        }
        s_entry = s_entry->getNext();
    }

    // check objects
    ListEntry<Object3D> *o_entry = this->objects3D->getHead();
    while(o_entry != NULL)
    {
        Object3D *obj = o_entry->element;
        if(obj != NULL)
        {
            Mesh3D *mesh = obj->mesh;
            if(mesh != NULL)
            {
                Vector3D m1 = mesh->box_size1;
                Vector3D m2 = mesh->box_size2;

                Vector3D a = Vector3D(m1.x, m1.y, m1.z);
                a.rotate(*obj->rotation);
                a.add(*obj->position);
                Vector3D b = Vector3D(m2.x, m1.y, m1.z);
                b.rotate(*obj->rotation);
                b.add(*obj->position);
                Vector3D c = Vector3D(m1.x, m2.y, m1.z);
                c.rotate(*obj->rotation);
                c.add(*obj->position);
                Vector3D d = Vector3D(m2.x, m2.y, m1.z);
                d.rotate(*obj->rotation);
                d.add(*obj->position);
                Vector3D e = Vector3D(m1.x, m1.y, m2.z);
                e.rotate(*obj->rotation);
                e.add(*obj->position);
                Vector3D f = Vector3D(m2.x, m1.y, m2.z);
                f.rotate(*obj->rotation);
                f.add(*obj->position);
                Vector3D g = Vector3D(m1.x, m2.y, m2.z);
                g.rotate(*obj->rotation);
                g.add(*obj->position);
                Vector3D h = Vector3D(m2.x, m2.y, m2.z);
                h.rotate(*obj->rotation);
                h.add(*obj->position);

                this->box_size1.min(a);
                this->box_size1.min(b);
                this->box_size1.min(c);
                this->box_size1.min(d);
                this->box_size1.min(e);
                this->box_size1.min(f);
                this->box_size1.min(g);
                this->box_size1.min(h);

                this->box_size2.max(a);
                this->box_size2.max(b);
                this->box_size2.max(c);
                this->box_size2.max(d);
                this->box_size2.max(e);
                this->box_size2.max(f);
                this->box_size2.max(g);
                this->box_size2.max(h);
            }
        }
        o_entry = o_entry->getNext();
    }
    //padding
//    this->box_size1.sub(0.1);
//    this->box_size2.add(0.1);

    this->logger->log(INFO, "box_size is %f, %f, %f", this->box_size1.x, this->box_size1.y, this->box_size1.z);
}

};

