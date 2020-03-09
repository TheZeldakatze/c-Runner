/*
 * PidginImage.c
 *
 *  Created on: 26.01.2020
 *      Author: victor
 */
#include "explosion.h"
#include "Runner.h"

static int animation_index = 0, delay;
static SDL_Rect sdl_rect_output, sdl_rect_source;
static SDL_Surface* image_explosion;

void Explosion_Init() {
	SDL_RWops* rwops = SDL_RWFromConstMem(explosion_bmp, sizeof(explosion_bmp) / sizeof(char));
	image_explosion = SDL_LoadBMP_RW(rwops,1);
	SDL_SetColorKey(image_explosion, SDL_SRCCOLORKEY, SDL_MapRGB(image_explosion->format, 255, 0, 255));

	sdl_rect_source.x = 0;
	sdl_rect_source.w = EXPLOSION_WIDTH;
	sdl_rect_source.h = EXPLOSION_HEIGHT;
}

void draw_Explosion(SDL_Surface* surface, int x, int y) {
	sdl_rect_output.x = x;
	sdl_rect_output.y = y;
	SDL_BlitSurface(image_explosion, &sdl_rect_source, surface, &sdl_rect_output);
}

bool Explosion_IncrementFrame() {
	delay++;
	//printf("%d\n", delay);
	if(delay >= 3) {
		delay=0;
		animation_index++;
		sdl_rect_source.y = animation_index * EXPLOSION_HEIGHT;
		if(animation_index > 6) {
			animation_index = 0;
			return true;
		}

		return false;
	}
}
