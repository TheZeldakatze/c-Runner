/*
 * PidginImage.c
 *
 *  Created on: 26.01.2020
 *      Author: victor
 */
#include "obstacles.h"
#include "Runner.h"

static SDL_Rect sdl_rect_output, sdl_rect_source;
static SDL_Surface* image_obstacles;

void Obstacles_Init() {
	SDL_RWops* rwops = SDL_RWFromConstMem(obstacles_bmp, sizeof(obstacles_bmp) / sizeof(char));
	image_obstacles = SDL_LoadBMP_RW(rwops,1);
	SDL_SetColorKey(image_obstacles, SDL_SRCCOLORKEY, SDL_MapRGB(image_obstacles->format, 255, 0, 255));

	sdl_rect_source.y = 0;
	sdl_rect_source.w = OBSTACLE_WIDTH;
	sdl_rect_source.h = OBSTACLE_HEIGHT;
}

void draw_Obstacle(SDL_Surface * surface, struct Obstacle *o) {
	sdl_rect_output.x = o->x;
	sdl_rect_output.y = o->y;

	switch(o->type) {
	case OBSTACLE_TYPE_BARREL:
		sdl_rect_source.y = 0;
		break;
	case OBSTACLE_TYPE_CAN:
		sdl_rect_source.y=OBSTACLE_HEIGHT*1;
		break;
	default:
		return;
	}

	SDL_BlitSurface(image_obstacles, &sdl_rect_source, surface, &sdl_rect_output);
}
