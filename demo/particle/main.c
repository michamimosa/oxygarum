//
// Particle System Test
//
// Copyright (C) 2013 Michael Sippel
// <micha.linuxfreak@gmail.com>
//

#include <oxygarum.h>

particle_emitter_t *emitter;

void anim(void) {
  oxygarum_update_all_particles(oxygarum_get_frametime());
}

int main(int argc, char **argv) {
  oxygarum_set_resolution(800, 600);
  oxygarum_set_title("Oxygarum test");  
  oxygarum_animation_func(&anim);  
  
  init_oxygarum(argc, argv);

  particle_t *min = malloc(sizeof(particle_t));
  particle_t *max = malloc(sizeof(particle_t));
  
  min->velocity.x = -0.01;
  min->velocity.y =  0;
  min->velocity.z = -0.01;
  min->color.rgb.r = 1;
  min->color.rgb.g = 0;
  min->color.rgb.b = 0;
  min->color.rgb.a = 1;
  min->saturation = 0.3;
  min->lifetime = 100;
  min->size = 0.1;  
  
  max->velocity.x =  0.01;
  max->velocity.y =  0.01;
  max->velocity.z =  0.01;
  max->lifetime = 600;
  max->color.rgb.r = 1;
  max->color.rgb.g = 1;
  max->color.rgb.b = 0;
  max->color.rgb.a = 1;
  max->saturation = 0.6;
  max->size = 0.5;
  
  emitter = malloc(sizeof(particle_emitter_t));
  emitter->mask_min = min;
  emitter->mask_max = max;
  
  emitter->pos.x = 0;
  emitter->pos.y = -2;
  emitter->pos.z = 0;
  emitter->gravity.x = 0;
  emitter->gravity.y = 0;
  emitter->gravity.z = 0;
  emitter->gravity_type = OXYGARUM_GRAVITY_TYPE_VERTEX;
  emitter->gravity_speed = 0.00001;
  emitter->num_particles_per_emission = 30;
  emitter->emision_rate = 1;
  
  oxygarum_add_particle_system(emitter);  
  
  oxygarum_translate_camera_to(0, 0, -10);
  oxygarum_rotate_camera_to(0, 0, 0);
  
  glutMainLoop();
  
  return 0;
}
