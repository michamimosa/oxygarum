/**
 *  include/particle.h
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
#ifndef _PARTICLE_H
#define _PARTICLE_H

#include "vertex.h"
#include "vector.h"
#include "material.h"

typedef struct particle {
  vertex3d_t pos;
  vector3d_t velocity;
  unsigned int lifetime;
  unsigned int age;
  unsigned int fade_in;
  unsigned int fade_out;
  
  float size;
  color_t color;
  float saturation;
} particle_t;

typedef struct particle_emiter {
  vertex3d_t pos;
  vertex3d_t gravity;
  unsigned int max_particles;
  unsigned int num_particles;
  unsigned int num_particles_per_emision;
  unsigned int emision_rate;
  particle_t **particles;
} particle_emiter_t;

void oxygarum_render_particle(particle_t *particle);
void oxygarum_render_particle_system(particle_emiter_t *emiter);
void oxygarum_render_all_particles(void);

int oxygarum_add_particle_system(particle_emiter_t *emiter);
void oxygarum_remove_particle_system(int id);

void oxygarum_update_particle_system(particle_emiter_t *emiter, float frametime);
void oxygarum_update_all_particles(float frametime);

int oxygarum_particle_vitalize(particle_emiter_t *emiter, vector3d_t *velocity);
void oxygarum_particle_die(particle_emiter_t *emiter, int id);

#endif

