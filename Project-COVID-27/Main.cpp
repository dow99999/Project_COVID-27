#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <list>

#include "SDL/include/SDL.h"

#pragma comment(lib, "SDL/x86/SDL2.lib")
#pragma comment(lib, "SDL/x86/SDL2main.lib")

//de momento meto un define, ya se cambiara por un const

//velocidad del usuario (afecta a velocidad de las balas)
#define SPEED 7;

//espacio entre balas (en frames)
#define SPACE_BULLETS 7;

typedef struct {
	int bx;
	int by;
	bool out;
	SDL_Rect blt;
} Bullet;

bool isOut(Bullet* a) { return a == nullptr; }

void setSpeed(int& dx, int& dy, const int &VEL, const int &DIAGONAL) {
	if (dx != 0 && dy != 0) {
		dx = dx > 0 ? DIAGONAL : DIAGONAL * -1;
		dy = dy > 0 ? DIAGONAL : DIAGONAL * -1;
	}
	else {
		if (dx != 0)
			dx = dx > 0 ? VEL : -VEL;
		if (dy != 0)
			dy = dy > 0 ? VEL : -VEL;
	}
	//std::cout << "dx: " << dx << " dy: " << dy << std::endl;
	//std::cout << "vel: " << (int)sqrt((dx*dx) + (dy*dy)) << std::endl;
}

int getCenterX(const SDL_Rect &r) {
	return (r.x + r.w / 2);
}

int getCenterY(const SDL_Rect &r) {
	return (r.y + r.h / 2);
}

bool areTouching(const SDL_Rect& a, const SDL_Rect& b) {
	return !((a.y + a.h <= b.y) || (a.y >= b.y + b.h) || (a.x + a.w <= b.x) || (a.x >= b.x + b.w));
}

void manageEvents(int &dx, int &dy, bool &fire, int &ready, bool &bucle) {
	int vel = SPEED;
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_UP:
				dy = -vel;
				break;
			case SDLK_DOWN:
				dy = vel;
				break;
			case SDLK_RIGHT:
				dx = vel;
				break;
			case SDLK_LEFT:
				dx = -vel;
				break;
			case SDLK_z:
				fire = true;
				break;
			case SDLK_ESCAPE:
				bucle = false;
			}
			break;
		case SDL_KEYUP:
			switch (event.key.keysym.sym) {
			case SDLK_UP:
				if (dy < 0)
					dy = 0;
				break;
			case SDLK_DOWN:
				if (dy > 0)
					dy = 0;
				break;
			case SDLK_RIGHT:
				if (dx > 0)
					dx = 0;
				break;
			case SDLK_LEFT:
				if (dx < 0)
					dx = 0;
				break;
			case SDLK_z:
				fire = false;
				ready = 0;
				break;
			}
			break;
		case SDL_QUIT:
			bucle = false;
			break;
		}
	}
}

void draw(SDL_Rect &r, SDL_Rect &f, std::list<Bullet*> &bullets, SDL_Surface* screenSurface) {
	//background
	SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0x3B, 0x70, 0xFF));
	
	//bullets
	for (std::list<Bullet*>::iterator it = bullets.begin(); it != bullets.end(); it++) {
		SDL_FillRect(screenSurface, &((*it)->blt), SDL_MapRGB(screenSurface->format, 0x00, 0xFF, 0x00));
	}

	//player
	SDL_FillRect(screenSurface, &r, SDL_MapRGB(screenSurface->format, 0xFF, areTouching(r,f) ? 0xFF : 0x00, 0x00));

	//follower
	SDL_FillRect(screenSurface, &f, SDL_MapRGB(screenSurface->format, 0xFF, 0x88, 0x00));
}

int main(int argc, char* argv[]) {
	//SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	srand(time(NULL));

	bool bucle = true;

	int x = 1280;
	int y = 720;
	int dx = 0;
	int dy = 0;
	bool fire = false;
	int ready = 0;

	int fdx = 0;
	int fdy = 0;

	int vel = SPEED;
	const int vel_f = vel * 0.6f;
	int t_wait = SPACE_BULLETS;
	const int DIAGONAL = ceil(vel / sqrt(2));
	const int FOLLOWER_D = ceil((vel_f) / sqrt(2));

	std::list<Bullet*> bullets;

	SDL_Window* window = SDL_CreateWindow("COVID_27", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, x, y, SDL_WINDOW_RESIZABLE);
	
	SDL_Rect r = { x / 2 - 50, y / 2 - 50, 100, 100 };
	SDL_Rect f = { 0, 0, 50, 50 };

	while (bucle) {
		SDL_Surface* screenSurface = SDL_GetWindowSurface(window);
		
		//input del usuario + set de acciones
		manageEvents(dx, dy, fire, ready, bucle);
		setSpeed(dx, dy, vel, DIAGONAL);

		if (getCenterX(f) < getCenterX(r)) {
			if (getCenterX(r) -getCenterX(f) >= vel_f)
				fdx = vel_f;
			else
				fdx = 0;
		} else {
			if (getCenterX(f) > getCenterX(r)) {
				if (getCenterX(f) - getCenterX(r) >= vel_f)
					fdx = -vel_f;
				else
					fdx = 0;
			} else {
				fdx = 0;
			}
		}
		if (getCenterY(f) < getCenterY(r)) {
			if (getCenterY(r) - getCenterY(f) >= vel_f)
				fdy = vel_f;
			else
				fdy = 0;
		}
		else {
			if (getCenterY(f) > getCenterY(r)) {
				if (getCenterY(f) - getCenterY(r) >= vel_f)
					fdy = -vel_f;
				else
					fdy = 0;
			}
			else {
				fdy = 0;
			}
		}
		setSpeed(fdx, fdy, vel_f, FOLLOWER_D);
		//std::cout << areTouching(r, f) << " r: " << r.x << ";" << r.y << " f: " << f.x << ";" << f.y << std::endl;

		//gestion del player
		if (r.x > 0) {
			if (r.x < x - r.w) {
				r.x += dx;
			} else {
				r.x = x - r.w - 1;
			}
		} else {
			r.x = 1;
		}
		if (r.y > 0) {
			if (r.y < y - r.h) {
				r.y += dy;
			} else{
				r.y = y - r.h - 1;
			}
		} else {
			r.y = 1;
		}

		ready--;
		if (fire && ready < 0 && !areTouching(f,r)) { //disparos deshabilitados si el follower te esta tocando
			ready = t_wait;
			Bullet* blt_aux = new Bullet;
			blt_aux->blt = { r.x + r.w / 2 - 5, r.y + r.h / 2 - 5, 10, 10 }; //tamanos de bala hardcodeados

			//caso en que este quieto el usuario
			if (dx == 0 && dy == 0) {
				blt_aux->bx = vel * 2;
				blt_aux->by = 0;
			}
			else {
				blt_aux->bx = dx * 2;
				blt_aux->by = dy * 2;
			}
			blt_aux->out = false;
			bullets.push_back(blt_aux);

			blt_aux = nullptr;
		}

		//follower
		f.x += fdx;
		f.y += fdy;

		//gestion de cada bala
		for (std::list<Bullet*>::iterator it = bullets.begin(); it != bullets.end(); it++) {
			//que chulos los iterators de listas de punteros btw, para nada es rara la linea de abajo
			SDL_Rect* aux = &((*it)->blt);

			if (aux->x > 0 - aux->w && aux->x < x) {
				aux->x += (*it)->bx;
			} else {
				(*it)->out = true;
			}

			if (aux->y > 0 - aux->h && aux->y < y) {
				aux->y += (*it)->by;
				if (areTouching((*aux), f)) { 
					switch (rand() % 4) {
					case 0:
						f.x = 0;
						f.y = 0;
						break;
					case 1:
						f.x = x - f.w;
						f.y = 0;
						break;
					case 2:
						f.x = 0;
						f.y = y - f.h;
						break;
					case 3:
						f.x = x - f.w;
						f.y = y - f.h;
						break;
					}
					(*it)->out = true; //si impacta el proyectil desaparece
				}
			} else {
				(*it)->out = true;
			}

			if ((*it)->out) {
				delete (*it);
				(*it) = nullptr;
			}
		}

		//borramos de la lista los punteros nulos
		bullets.remove_if(isOut);
		//std::cout << "bullets: " << bullets.size() << std::endl;

		//mostramos el frame
		draw(r, f, bullets, screenSurface);

		//update de la pantalla
		SDL_Delay(15);
		SDL_UpdateWindowSurface(window);
	}

	SDL_Quit();
	return 0;
}