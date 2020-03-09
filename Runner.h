/*
 * Runner.h
 *
 *  Created on: 07.03.2020
 *      Author: victor
 */
#ifdef WIN32
#include <SDL.h>
#else
#include <SDL/SDL.h>
#endif
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define STATE_MAIN_MENU 1
#define STATE_GAME 2
#define STATE_ACCIDENT 3
#define STATE_GAME_OVER 4

#define TICK_SPEED 20
#define TICKS_PER_SECOND 1000 / TICK_SPEED

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define OBSTACLE_COUNT 20

#define OBSTACLE_TYPE_NONE   0
#define OBSTACLE_TYPE_BARREL 1
#define OBSTACLE_TYPE_CAN    2

#define PLAYER_Y 430

struct Player {
	int x;
	int lives, score, ticks_until_next_score;
	
};

struct Obstacle {
	int x;
	int y;
	unsigned char type;
};

// main.c
extern int getRandomInt(int limit);
extern bool checkCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);

// BitmapFontRenderer.c
extern void Font_Init();
extern void Font_DrawString(SDL_Surface* surface, int x, int y, char* string);

// CharImage.c
extern void Character_Init();
extern void draw_Character(SDL_Surface* surface, int x, int y);
extern void Character_IncrementFrame();

// ExplosionImage.c
extern void Explosion_Init();
extern void draw_Explosion(SDL_Surface* surface, int x, int y);
extern bool Explosion_IncrementFrame();

// obstacleImage.c
extern void Obstacles_Init();
extern void draw_Obstacle(SDL_Surface * surface, struct Obstacle *o);
