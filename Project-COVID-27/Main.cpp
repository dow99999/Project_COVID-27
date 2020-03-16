#include "SDL/include/SDL.h"
#include <stdlib.h>
#include <time.h>

#pragma comment(lib, "SDL/x86/SDL2.lib")
#pragma comment(lib, "SDL/x86/SDL2main.lib")



int main(int argc, char* argv[]) {
	//SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

	srand(time(NULL));

	int x = 500;
	int y = 500;
	int dx;
	int dy;
	do {
		dx = rand() % 11 - 5;
	} while (dx == 0);

	do {
		dy = rand() % 11 - 5;
	} while (dy == 0);

	SDL_Window* window = SDL_CreateWindow("COVID_27", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, x, y, SDL_WINDOW_RESIZABLE);
	
	SDL_Rect r = { 240, 240, 120, 100 };

	while (true) {

		SDL_Surface* screenSurface = SDL_GetWindowSurface(window);

		if (r.x > 0) {
			if (r.x < x - r.w) {
				r.x += dx;
			} else {
				r.x -= 1;
				dx = rand() % 4 - 5;
			}
		} else {
			r.x += 1;
			dx = rand() % 4 + 1;
		}

		if (r.y > 0) {
			if (r.y < y - r.h) {
				r.y += dy;
			} else{
				r.y -= 1;
				dy = rand() % 4 - 5;
			}
		} else {
			r.y += 1;
			dy = rand() % 4 + 1;
		}

		SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0x3B, 0x70, 0xFF));
		SDL_FillRect(screenSurface, &r, SDL_MapRGB(screenSurface->format, 0xFF, 0x00, 0x00));

		SDL_Delay(20);
		SDL_UpdateWindowSurface(window);
	}

	return 0;
}