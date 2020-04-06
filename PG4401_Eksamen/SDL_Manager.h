#ifndef _SDLMANAGER_H_
#define _SDL_MANAGER_H_

#include <iostream>

#include "Texture_Manager.h"

class SDL_Manager {

public:
	SDL_Manager();

	SDL_Window *createWindow(const char * c, const int w, const int h);
	SDL_Renderer *createRenderer(SDL_Window *w);
	SDL_Surface *createSurface(const char *c, SDL_Window *window, SDL_Renderer *renderer);

	void SetRenderColor(SDL_Renderer *renderer, int r, int g, int b, int a);
	void ClearRender(SDL_Renderer *renderer);

private:
	void printError(std::ostream &os, const std::string &msg);

};
#endif
