#include "Runner.h"
#include "banner.h"
#include "title.h"


SDL_Surface* screen;
SDL_Surface* title_image;
bool run;
SDL_Rect sdl_rect, sdl_rect2;

unsigned char state;

// key handler stuff
bool keyPressed[SDLK_LAST];

struct Player player;
struct Obstacle obstacles[OBSTACLE_COUNT];

int main(int argc, char *argv[]) {
	memset(keyPressed,0,sizeof(keyPressed));
	srand(435334);

	// initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO) == -1) {
		printf("Could not initialize SDL! Error: %s", SDL_GetError());
		return -1;
	}

	// create a window
	if(argc>1 && strcmp(argv[1], "-fs") == 0)
		screen = SDL_SetVideoMode(640, 480, 8, SDL_DOUBLEBUF | SDL_FULLSCREEN);
	else
		screen = SDL_SetVideoMode(640, 480, 8, SDL_DOUBLEBUF);

	if(!screen) {
		printf("Could not create a SDL window! Error: %s", SDL_GetError());
		return -1;
	}

	// draw the background
	sdl_rect.x = 0;
	sdl_rect.y = 0;
	sdl_rect.w = SCREEN_WIDTH;
	sdl_rect.h = SCREEN_HEIGHT;
	SDL_FillRect(screen, &sdl_rect, SDL_MapRGB(screen->format,255,255,255));

	// load the loading banner
	// it has the dimensions of 320x60
	SDL_RWops* rwops = SDL_RWFromMem(banner_bmp, sizeof(banner_bmp) / sizeof(char));
	SDL_Surface * banner = SDL_LoadBMP_RW(rwops,1);

	sdl_rect2.x = 160;
	sdl_rect2.y = 210;
	SDL_BlitSurface(banner, (void*) 0, screen, &sdl_rect2);
	SDL_Flip(screen);
	SDL_FreeSurface(banner);

	// initialize the font renderer
	Font_Init();
	
	// initialize the sprites
	Character_Init();
	Explosion_Init();
	Obstacles_Init();

	// set the correct position of the character
	player.x = SCREEN_WIDTH / 2-6;

	// load the title
	rwops = SDL_RWFromConstMem(title_bmp, sizeof(title_bmp) / sizeof(char));
	title_image = SDL_LoadBMP_RW(rwops,1);
	SDL_SetColorKey(title_image, SDL_SRCCOLORKEY, SDL_MapRGB(title_image->format, 255, 0, 255));

	// wait 2 seconds
	sleep(2);

	// start the event loop
	run = true;
	state = 1;
	while(run) {

		// get the time
		long deltaTime = SDL_GetTicks();

		// process all sdl events
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
					run = false;
					break;

				case SDL_KEYDOWN:
					keyPressed[event.key.keysym.sym] = true;
					break;

				case SDL_KEYUP:
					keyPressed[event.key.keysym.sym] = false;
					break;
			}
		}

		// quit if the esc key was pressed
		if(keyPressed[SDLK_ESCAPE]) run = false;

		// check if the fullscreen toogle button was hit
		if(keyPressed[SDLK_f]) {
			SDL_WM_ToggleFullScreen(screen);
		}

		// the game routine
		switch(state) {
			case STATE_MAIN_MENU: {
				// if space was pressed, start the game
				if(keyPressed[SDLK_SPACE]) {
					keyPressed[SDLK_SPACE] = false;
					state = STATE_GAME;

					// setup the game
					player.lives = 3;
					player.score = 0;
				}

				break;
			}
			case STATE_GAME: {
				// move the player
				if(keyPressed[SDLK_LEFT] && player.x > 0)
					player.x-=3;
				if(keyPressed[SDLK_RIGHT] && player.x < SCREEN_WIDTH - 12)
					player.x+=3;

				// increment the score
				player.ticks_until_next_score--;
				if(player.ticks_until_next_score <= 0) {
					player.ticks_until_next_score = TICKS_PER_SECOND;
					player.score++;
				}

				for(int i = 0; i<OBSTACLE_COUNT; i++) {

					// the obstacles actually move torwards you
					obstacles[i].y+=2 + (player.score / 60.0);

					// reset the obstacle, if it is not visible
					if(obstacles[i].type == OBSTACLE_TYPE_NONE || obstacles[i].y > SCREEN_HEIGHT) {
						obstacles[i].type = (unsigned char) getRandomInt(2);
						obstacles[i].x = getRandomInt(SCREEN_WIDTH-32);
						obstacles[i].y = -getRandomInt(300)-32;
					}

					// check for collision
					if(obstacles[i].type != OBSTACLE_TYPE_NONE && checkCollision(player.x, PLAYER_Y, 12, 36, obstacles[i].x, obstacles[i].y, 32, 32))
						state = STATE_ACCIDENT;
				}
				break;
			}
			case STATE_ACCIDENT: {
				if(Explosion_IncrementFrame()) {
					state = STATE_GAME;
					player.lives--;
					if(player.lives <= 0)
						state = STATE_GAME_OVER;

					// reset all obstacles
					for(int i = 0; i<OBSTACLE_COUNT; i++)
						obstacles[i].type = OBSTACLE_TYPE_NONE;
				}
				break;
			}
			case STATE_GAME_OVER: {
				// if space was pressed, go to the main menu
				if(keyPressed[SDLK_SPACE]) {
					keyPressed[SDLK_SPACE] = false;
					state = STATE_MAIN_MENU;
					player.x = SCREEN_WIDTH / 2-6;
				}
				break;
			}
		}

		// draw the background
		sdl_rect.x = 0;
		sdl_rect.y = 0;
		sdl_rect.w = SCREEN_WIDTH;
		sdl_rect.h = SCREEN_HEIGHT;
		SDL_FillRect(screen,&sdl_rect,SDL_MapRGB(screen->format,150,150,150));

		// the graphics routine
		switch(state) {
			case STATE_MAIN_MENU: {

				// draw the title
				sdl_rect.x = 221;
				sdl_rect.y = 10;
				SDL_BlitSurface(title_image, (void*) 0, screen, &sdl_rect);

				Font_DrawString(screen, 236, 100, "Press Space to start");
				Font_DrawString(screen, 236, 120, "Press ESCape to quit");

				break;
			}

			case STATE_GAME: {
				for(int i = 0; i<OBSTACLE_COUNT; i++) {
					if(obstacles[i].type != OBSTACLE_TYPE_NONE)
						draw_Obstacle(screen, &obstacles[i]);

				}

				// draw the score
				char score_s[50]; // TODO this could create a buffer overflow
				sprintf(score_s, "Score: %d", player.score);
				Font_DrawString(screen, 10, 10, score_s);

				// draw the lives
				sprintf(score_s, "Lives: %d", player.lives);
				Font_DrawString(screen, 10, 20, score_s);

				break;
			}

			case STATE_ACCIDENT: {
				for(int i = 0; i<OBSTACLE_COUNT; i++) {
					if(obstacles[i].type != OBSTACLE_TYPE_NONE)
						draw_Obstacle(screen, &obstacles[i]);
				}
				break;
			}
			case STATE_GAME_OVER: {
				Font_DrawString(screen, SCREEN_WIDTH /2  - 8 * 4, 150, "Game Over");

				// draw the score
				char score_s[50]; // TODO this could create a buffer overflow
				sprintf(score_s, "Score: %d", player.score);
				int length = strlen(score_s) + 1;
				Font_DrawString(screen, SCREEN_WIDTH /2  - length * 4, 180, score_s);

				Font_DrawString(screen, SCREEN_WIDTH / 2 - 23 * 4, 200, "Press Space to continue");
			}
		}

		// draw the character
		if(state != STATE_GAME_OVER) {
			if(state != STATE_ACCIDENT) {
			draw_Character(screen, player.x, PLAYER_Y);
				Character_IncrementFrame();
			}
			else
				// draw an explosion
				draw_Explosion(screen, player.x, PLAYER_Y);
		}

		// draw the screen
		SDL_Flip(screen);

		// get the delta time
		deltaTime = SDL_GetTicks()-deltaTime;

		// calculate the time until the next frame
		deltaTime = TICK_SPEED - deltaTime;

		// if it is higher than 0, sleep
		if(deltaTime > 0) {
				unsigned int time_from_sleep_return = SDL_GetTicks();

				while(deltaTime + time_from_sleep_return > SDL_GetTicks()) {

					// try to sleep for the left time
					unsigned int time = (deltaTime + time_from_sleep_return-SDL_GetTicks()) * 1000;
					if(time > 0)
						usleep(time);
				}
		}
	}

	// dispose the surfaces
	SDL_FreeSurface(screen);
	SDL_Quit();

	return 0;
}

bool checkCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
		if(y1 > y2 + h2) return false;
		if(y2 > y1 + h1) return false;
		if(x1 > x2 + w2) return false;
		if(x2 > x1 + w1) return false;

		return true;
}

int getRandomInt(int limit) {
	return rand() % (limit+1);
}
