/*
 * PidginImage.c
 *
 *  Created on: 26.01.2020
 *      Author: victor
 */
#include "char.h"
#include "Runner.h"

static int animation_index = 0, delay, ax = 1;
static SDL_Rect sdl_rect_output, sdl_rect_source;
static SDL_Surface* image_pidgin;
SDL_Surface* image_pidgin_dead;

void Character_Init() {
	SDL_RWops* rwops = SDL_RWFromConstMem(char_bmp, sizeof(char_bmp) / sizeof(char));
	image_pidgin = SDL_LoadBMP_RW(rwops,1);
	SDL_SetColorKey(image_pidgin, SDL_SRCCOLORKEY, SDL_MapRGB(image_pidgin->format, 255, 0, 255));

	sdl_rect_source.y = 0;
	sdl_rect_source.w = CHAR_WIDTH;
	sdl_rect_source.h = CHAR_HEIGHT;
}

void draw_Character(SDL_Surface* surface, int x, int y) {
	sdl_rect_output.x = x;
	sdl_rect_output.y = y;
	SDL_BlitSurface(image_pidgin, &sdl_rect_source, surface, &sdl_rect_output);
}

void Character_IncrementFrame() {
	delay++;
	//printf("%d\n", delay);
	if(delay >= 10) {
		delay = 0;
		animation_index+=ax;
		sdl_rect_source.x = animation_index * CHAR_WIDTH;
		if(animation_index > 1) ax = -1;
		if(animation_index < 1) ax = 1;
	}
}
