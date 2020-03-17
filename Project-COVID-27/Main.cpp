#include <stdlib.h>
#include <time.h>
#include <iostream>

#include "SDL/include/SDL.h"

#pragma comment(lib, "SDL/x86/SDL2.lib")
#pragma comment(lib, "SDL/x86/SDL2main.lib")



int main(int argc, char* argv[]) {
	//SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

	srand(time(NULL));

	bool bucle = true;

	int x = 500;
	int y = 500;
	int dx = 0;
	int dy = 0;

	bool firing = false;
	int bx;
	int by;

	SDL_Window* window = SDL_CreateWindow("COVID_27", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, x, y, SDL_WINDOW_RESIZABLE);
	
	SDL_Rect r = { 240, 240, 100, 100 };
	SDL_Rect bullet = { 240, 240, 10, 10 };

	while (bucle) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_UP:
					if (dy == 5)
						dy = 0;
					else
						dy = -5;
					break;
				case SDLK_DOWN:
					if (dy == -5)
						dy = 0;
					else
						dy = 5;
					break;
				case SDLK_RIGHT:
					if (dx == -5)
						dx = 0;
					else
						dx = 5;
					break;
				case SDLK_LEFT:
					if (dx == 5)
						dx = 0;
					else
						dx = -5;
					break;
				case SDLK_SPACE:
					if (!firing) {
						firing = true;
						if (dx == 0 && dy == 0) {
							bx = 10;
							by = 0;
						}
						else {
							bx = dx * 2;
							by = dy * 2;
						}
					}
					break;
				case SDLK_ESCAPE:
					bucle = false;
				}
				break;
			case SDL_KEYUP:
				switch (event.key.keysym.sym) {
				case SDLK_UP:
					if(dy < 0)
						dy = 0;
					break;
				case SDLK_DOWN:
					if(dy > 0)
						dy = 0;
					break;
				case SDLK_RIGHT:
					if(dx > 0)
						dx = 0;
					break;
				case SDLK_LEFT:
					if(dx < 0)
						dx = 0;
					break;
				}
				break;
			case SDL_QUIT:
				bucle = false;
				break;
			}
		}

		SDL_Surface* screenSurface = SDL_GetWindowSurface(window);

		if (r.x > 0) {
			if (r.x < x - r.w) {
				r.x += dx;
			} else {
				r.x = x - r.w - 1;
				//dx = rand() % 4 - 5;
			}
		} else {
			r.x = 1;
			//dx = rand() % 4 + 1;
		}

		if (r.y > 0) {
			if (r.y < y - r.h) {
				r.y += dy;
			} else{
				r.y = y - r.h - 1;
				//dy = rand() % 4 - 5;
			}
		} else {
			r.y = 1;
			//dy = rand() % 4 + 1;
		}

		if (firing) {
			if (bullet.x > 0 - bullet.w) {
				if (bullet.x < x) {
					bullet.x += bx;
				}
				else {
					firing = false;
				}
			}
			else {
				firing = false;
			}

			if (bullet.y > 0 - bullet.h) {
				if (bullet.y < y) {
					bullet.y += by;
				}
				else {
					firing = false;
				}
			}
			else {
				firing = false;
			}
		} else {
			bullet.x = r.x + r.w/2 - bullet.w/2;
			bullet.y = r.y + r.h/2 - bullet.h/2;
		}


		SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0x3B, 0x70, 0xFF));
		SDL_FillRect(screenSurface, &r, SDL_MapRGB(screenSurface->format, 0xFF, 0x00, 0x00));

		if(firing)
			SDL_FillRect(screenSurface, &bullet, SDL_MapRGB(screenSurface->format, 0x00, 0xFF, 0x00));

		SDL_Delay(20);
		SDL_UpdateWindowSurface(window);
	}


	SDL_Quit();
	return 0;
}