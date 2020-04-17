#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Character.h"

class Player : public Character
{
public:
	//constructors
	Player(SDL_Texture* t, SDL_Renderer* r, const Uint8* k, std::vector<SDL_Rect>& p);
	Player(SDL_Texture* t, SDL_Rect c, SDL_Renderer* r, const Uint8* k, std::vector<SDL_Rect>& p);

	//movement
	void move(SDL_Surface *surface, int &SCREEN_WIDTH, int &SCREEN_HEIGHT, std::vector<std::vector<char>>& map, std::vector<SDL_Rect>& walls) override;

	//collision
	bool checkWallCollision(std::vector<SDL_Rect>& walls, int x_offset, int y_offset);

	bool checkEdibleCollision(std::vector<SDL_Rect>& edible, std::vector<std::vector<char>>& map);

	//animation 
	void renderTexture();

	void animate(Animation a);

	void animateEx(Animation a, char direction);

	void setAnimation(std::string name, std::shared_ptr<Animation> animation) {
		animations[name] = animation;
	}

	//getters and setters
	SDL_Texture* getTexture() override;

	SDL_Rect* getCoords() override;

	void setPos(int x, int y) override;

	void setSpawnPos(int x, int y) override;

	void setSize(int h, int w) override;

	void setScore(int s) override;

	char getDirection() override;

	void setDirection(char d) {
		direction = d;
	}
	int getScore() {
		return score;
	}
	int getHP() override {
		return hp;
	}

	SDL_Rect getSpawnPos() {
		return startCoords;
	}

	void hitByGhost() override {
		hp--;
		coords.x = startCoords.x;
		coords.y = startCoords.y;
	}

private:
	const Uint8* keys;

	std::vector<SDL_Rect>& pellets;
	
	std::map<std::string, std::shared_ptr<Animation>> animations;
	
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	SDL_Rect coords;
	SDL_Rect startCoords{};
	
	char direction = ' ';
	char next_direction = 'n';
	char collided = 'n';
	
	int speed = 1;
	int score = 0;
	int hp = 3;
};
#endif