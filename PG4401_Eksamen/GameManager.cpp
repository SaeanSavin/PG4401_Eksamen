//#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "GameManager.h"
#include "SDL_Manager.h"

#include "Player.h"
//#include "Texture_Manager.h"

//Window size
int SCREEN_WIDTH = 450;
int SCREEN_HEIGHT = 500;

SDL_Texture* LoadTexture(const char* pos, SDL_Renderer *renderer) {

	SDL_Surface* tmpSurface = IMG_Load(pos);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);

	return texture;
}

void GameManager::setFramerate(const int FPS) {
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;

	frameStart = SDL_GetTicks();

	frameTime = SDL_GetTicks() - frameStart;

	if (frameDelay > frameTime) {
		SDL_Delay(frameDelay - frameTime);
	}
}

GameManager::GameManager() {}

int GameManager::play(std::string name) {

	auto p1 = std::make_unique<Player>();

	auto sdl_manager = std::make_unique<SDL_Manager>();
	//auto texture_manager = std::make_unique<Texture_Manager>();
	
	const int FPS = 60;

	std::vector<std::vector<char>> map{};
	loadMap(name, map);

	//for debug
	for (auto& row : map) {
		for (auto& c : row) {
			std::cout << c;
		}
		std::cout << std::endl;
	}

	SDL_Window *window = sdl_manager->createWindow("Pac-man", SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_Renderer *renderer = sdl_manager->createRenderer(window);

	//draws background
	sdl_manager->SetRenderColor(renderer, 0, 0, 0, 255);
	sdl_manager->ClearRender(renderer);

	SDL_Surface *surface = sdl_manager->createSurface("../images/pacman.png", window, renderer);
	//SDL_Texture *drawable = texture_manager->draw(renderer, surface);
	
	//SDL_Rect coords = texture_manager->setCoords(surface);

	
	//Draw images to VRAM
	SDL_Texture* drawable = SDL_CreateTextureFromSurface(renderer, surface);

	
	//Setting coordinates for images
	SDL_Rect coords;
	coords.h = 16;
	coords.w = 16;
	coords.x = 0;
	coords.y = 100;	
	
	//Freeing the RGB surface
	//SDL_FreeSurface(surface);

	//Create Textures

	SDL_Texture* pellet = LoadTexture("../images/mapTiles/pellet.png", renderer);
	SDL_Texture* wall_bottom = LoadTexture("../images/mapTiles/wall_bottom_single.png", renderer);
	SDL_Texture* wall_top = LoadTexture("../images/mapTiles/wall_top_single.png", renderer);
	SDL_Texture* wall_left = LoadTexture("../images/mapTiles/wall_left_single.png", renderer);
	SDL_Texture* wall_right = LoadTexture("../images/mapTiles/wall_right_single.png", renderer);
	SDL_Texture* corner_top_right = LoadTexture("../images/mapTiles/wall_corner_tr_single.png", renderer);
	SDL_Texture* corner_top_left = LoadTexture("../images/mapTiles/wall_corner_tl_single.png", renderer);
	SDL_Texture* corner_bottom_right = LoadTexture("../images/mapTiles/wall_corner_br_single.png", renderer);
	SDL_Texture* corner_bottom_left = LoadTexture("../images/mapTiles/wall_corner_bl_single.png", renderer);

	bool isRunning = true;
	const Uint8* keys = nullptr;
	int numKeys;
	keys = SDL_GetKeyboardState(&numKeys);
	SDL_Event evt;

	//movement variables
	char direction = 'i';

	//Game Loop
	while (isRunning) {

		//setFramerate(FPS);
		setFramerate(FPS);

		//Checks if Escape is press or X in the window
		if (SDL_PollEvent(&evt)) {
			if (evt.type == SDL_KEYDOWN) {
				if (evt.key.keysym.sym == SDLK_ESCAPE) {
					isRunning = false;
				}
			}
			else if (evt.type == SDL_QUIT) {
				isRunning = false;
			}
		}

		//Keys input for movement
		
		p1->movePlayer(keys, coords, surface, SCREEN_WIDTH, SCREEN_HEIGHT);

		//Prepare Renderer for a new frame
		SDL_RenderCopy(renderer, drawable, nullptr, &coords);

		//render map

		SDL_Rect mapRect;
		mapRect.w = 16;
		mapRect.h = 16;
		mapRect.x = 0;
		mapRect.y = 0;

		for (auto& row : map) {
			for (auto& c : row) {

				switch (c) {
				case 'x':
					SDL_RenderCopy(renderer, pellet, nullptr, &mapRect);
					break;
				case '1':
					SDL_RenderCopy(renderer, corner_bottom_left, nullptr, &mapRect);
					break;
				case '3':
					SDL_RenderCopy(renderer, corner_bottom_right, nullptr, &mapRect);
					break;
				case '7':
					SDL_RenderCopy(renderer, corner_top_left, nullptr, &mapRect);
					break;
				case '9':
					SDL_RenderCopy(renderer, corner_top_right, nullptr, &mapRect);
					break;
				case '2':
					SDL_RenderCopy(renderer, wall_bottom, nullptr, &mapRect);
					break;
				case '4':
					SDL_RenderCopy(renderer, wall_left, nullptr, &mapRect);
					break;
				case '6':
					SDL_RenderCopy(renderer, wall_right, nullptr, &mapRect);
					break;
				case '8':
					SDL_RenderCopy(renderer, wall_top, nullptr, &mapRect);
					break;
				case ' ':
				case '-':
					break;
				}
				mapRect.x += 16;
			}
			mapRect.x = 0;
			mapRect.y += 16;
		}

		SDL_RenderPresent(renderer);
		SDL_RenderClear(renderer);
	}

	//Program exit 
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
	return EXIT_SUCCESS;
}

void GameManager::loadMap(std::string map, std::vector<std::vector<char>> &mapVector) {
	std::ifstream fIn(map);

	if (!fIn.is_open()) {
		std::cout << "Cannot open map file" << std::endl;
		return;
	}

	std::string str;

	while (std::getline(fIn, str)) {

		std::vector<char> mapRow;

		for (size_t i = 0; i < str.length(); i++) {
			mapRow.emplace_back(str[i]);
		}

		mapVector.emplace_back(mapRow);
	}

	fIn.close();
}

void GameManager::drawMap(std::vector<std::vector<char>> &mapVector) {}