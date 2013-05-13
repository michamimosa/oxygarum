//
// A pong-game using the
// oxygarum-engine
//
// Copyright (C) 2013 Michael Sippel
// <micha.linuxfreak@gmail.com>
//

#include <oxygarum.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

GLfloat ambient[]= { 0.8f, 0.7f, 0.9f, 1.0f };
GLfloat diffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat specular[]= { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat position[]= { 1.0f, 0.5f, -0.1f, 1.0f };

int player1_id, player2_id, playground_id, ball_id;
int shade_mode = 1;
object_t *player = NULL;
object_t *ball = NULL;
object_t *load_screen = NULL;
int load_screen_id;

float player1_loc = 0.5;
float player1_x = 0;
float player2_loc = 0.5;
float sens = 0.1;

#define DIRECTION_LEFT 0
#define DIRECTION_RIGHT 1
#define DIRECTION_FORWARD 0
#define DIRECTION_BACK 1

float ball_x = 0, ball_y = -1;
int direction_x = DIRECTION_LEFT;
int direction_y = DIRECTION_BACK;

void player1_up(void) {
  if(player1_loc > 0) {
    player1_loc -= sens;
  }
}

void player1_down(void) {
  if(player1_loc < 1) {
    player1_loc += sens;
  }
}

void anim(void) {
  static game_over = 0;
  if(game_over) {
    return;
  }
  float anim_sens = oxygarum_get_frametime()*0.005;
  
  player1_x = (player1_loc-0.5)*3;
  oxygarum_translate_object_to(player1_id, player1_x, 0, 0);
  
  switch(direction_y) {
    case DIRECTION_FORWARD:
      ball_y -= anim_sens;
      break;
    case DIRECTION_BACK:
      ball_y += anim_sens;
      break;
  }
  switch(direction_x) {
    case DIRECTION_LEFT:
      ball_x -= anim_sens;
      break;
    case DIRECTION_RIGHT:
      ball_x += anim_sens;
      break;
  }
  
  if(ball_x >= 3) {
    direction_x = DIRECTION_LEFT;
  } else if(ball_x <= -3) {
    direction_x = DIRECTION_RIGHT;
  }
  
  if(ball_y <= -3.5) {
    direction_y = DIRECTION_BACK;
  } else if(ball_y >= -0.5) {
    if(ball_x <= player1_x+0.8 && ball_x >= player1_x-0.8) {
      direction_y = DIRECTION_FORWARD;
    } else {
      load_screen->faces[0]->material->texture = oxygarum_load_texture("game_over.png");
      oxygarum_set_object_status(load_screen_id, OBJECT_VISIBLE);
      oxygarum_translate_camera_to(0, 0, 0);
      oxygarum_rotate_camera_to(0, 0, 0);
      game_over = 1;
    }
  }
  
  oxygarum_translate_object_to(ball_id, ball_x, 0, ball_y);
}

void wait_for_begin(void) {
  if(oxygarum_get_fps() > 0 && player != NULL && ball != NULL) {
    oxygarum_set_object_status(load_screen_id, OBJECT_INVISIBLE);
    oxygarum_rotate_camera_to(20, 0, 0);
    oxygarum_translate_camera_to(0, -2, -3);
    oxygarum_animation_func(&anim);
  } else if(player == NULL) {
    player = oxygarum_load_oxy3d_file("player.oxy3d");
    player1_id = oxygarum_add_object(player, 0, 0, 0);
  } else if(ball == NULL) {
    ball = oxygarum_load_oxy3d_file("ball.oxy3d");
    ball_id = oxygarum_add_object(ball, 0, 0, -4);
  }
}

int main(int argc, char **argv) {
  oxygarum_set_resolution(800, 600);
  oxygarum_set_title("Oxygarum test");
  oxygarum_set_flag(OXYGARUM_FULLSCREEN, 0);
  oxygarum_animation_func(&wait_for_begin);
  
  oxygarum_set_keyboard_event('a', &player1_up);
  oxygarum_set_keyboard_event('d', &player1_down);  
  
  init_oxygarum(argc, argv);
  oxygarum_set_light(GL_LIGHT1, ambient, diffuse, specular, position);  
  
  load_screen = oxygarum_load_oxy3d_file("../load_screen.oxy3d");
  load_screen_id = oxygarum_add_object(load_screen, 0, 0, -0.4);
  oxygarum_set_max_fps(60);
  
  glutMainLoop();
  
  return 0;
}

