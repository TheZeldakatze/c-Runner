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
	SDL_RWops* rwops = SDL_RWFromConstMem(banner_bmp, sizeof(banner_bmp) / sizeof(char));
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

		// draw the background
		sdl_rect.x = 0;
		sdl_rect.y = 0;
		sdl_rect.w = SCREEN_WIDTH;
		sdl_rect.h = SCREEN_HEIGHT;
		SDL_FillRect(screen,&sdl_rect,SDL_MapRGB(screen->format,150,150,150));

		// draw the character
		draw_Character(screen, player.x, PLAYER_Y);
		Character_IncrementFrame();

		// draw an explosion
		// TODO remove
		draw_Explosion(screen, 100, 100);
		Explosion_IncrementFrame();

		// draw the title
		sdl_rect.x = 220;
		sdl_rect.y = 10;
		SDL_BlitSurface(title_image, (void*) 0, screen, &sdl_rect);

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
