#include "SDL_Manager.h"

SDL_Manager::SDL_Manager() {}

void SDL_Manager::printError(std::ostream &os, const std::string &msg) {
	os << msg << " error " << SDL_GetError() << std::endl;
}

SDL_Window *SDL_Manager::createWindow(const char *c, const int w, const int h) {
	
	SDL_Window *window = SDL_CreateWindow(c,
										  SDL_WINDOWPOS_UNDEFINED,
										  SDL_WINDOWPOS_UNDEFINED,
										  w,
										  h,
										  SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS 
										  );
	if (window == nullptr) {
		printError(std::cout, "Cannot create window ");
		SDL_Quit();
		return nullptr;
	}

	return window;
}

SDL_Renderer *SDL_Manager::createRenderer(SDL_Window *w, int i) {
	SDL_Renderer *renderer = SDL_CreateRenderer(w, i, SDL_RENDERER_ACCELERATED);

	if (renderer == nullptr) {
		printError(std::cout, "Cannot create renederer ");
		SDL_DestroyWindow(w);
		SDL_Quit();
		return nullptr;
	}
	return renderer;
}

SDL_Surface *SDL_Manager::createSurface(const char *c, SDL_Window *w, SDL_Renderer *r) {
	SDL_Surface *surface = IMG_Load(c);

	if (surface == nullptr) {
		SDL_DestroyRenderer(r);
		SDL_DestroyWindow(w);
		IMG_Quit();
		SDL_Quit();
		return nullptr;
	}
	return surface;
}

SDL_Rect SDL_Manager::createRect(int w, int h, int x, int y) {
	
	SDL_Rect rect;
	rect.w = w;
	rect.h = h;
	rect.x = x;
	rect.y = y;
	
	return rect;
}

SDL_GameController *SDL_Manager::getGameController(int i) {
	
	if (SDL_IsGameController(i)) {
		std::cout << "Controller found!" << std::endl;
	} else {
		std::cout << "No Controller found!" << std::endl;
		return nullptr;
	}
	SDL_GameController *gc = SDL_GameControllerOpen(i);
	return gc;
}

void SDL_Manager::SetRenderColor(SDL_Renderer *renderer, int r, int g, int b, int a) {
	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
}

void SDL_Manager::clearAndUpdateRenderer(SDL_Renderer *r) {
	SDL_RenderPresent(r);
	SDL_RenderClear(r);
}

void SDL_Manager::clearRenderer(SDL_Renderer *r) {
	SDL_RenderClear(r);
}
