#include "Player.h"

Player::Player(SDL_Texture* t) {
	texture = t;
	coords.h = 0;
	coords.w = 0;
	coords.x = 0;
	coords.y = 0;
}
Player::Player(SDL_Texture* t, SDL_Rect c) {
	texture = t;
	coords = c;
}

//movement function
void Player::movePlayer(const Uint8 *keys, SDL_Surface *surface, int &SCREEN_WIDTH, int &SCREEN_HEIGHT) {

	SDL_PumpEvents();
	//if idle, check input
	if (keys[SDL_SCANCODE_W]) {
		direction = 'w';
	}

	if (keys[SDL_SCANCODE_S]) {
		direction = 's';
	}

	if (keys[SDL_SCANCODE_A]) {
		direction = 'a';
	}

	if (keys[SDL_SCANCODE_D]) {
		direction = 'd';
	}

	//if out of bounds, set to idle
	else {

		//Make sure image dont move out of bounds
		if (coords.x > SCREEN_WIDTH - coords.w) {
			coords.x = SCREEN_WIDTH - coords.w;
			direction = 'i';
		}

		if (coords.y > SCREEN_HEIGHT - coords.h) {
			coords.y = SCREEN_HEIGHT - coords.h;
			direction = 'i';
		}

		if (coords.x < 0) {
			coords.x = 0;
			direction = 'i';
		}

		if (coords.y < 0) {
			coords.y = 0;
			direction = 'i';
		}
	}
	//decide movement
	switch (direction) {
	case 'w':
		coords.y -= speed;
		break;
	case 's':
		coords.y += speed;
		break;
	case 'a':
		coords.x -= speed;
		break;
	case 'd':
		coords.x += speed;
		break;
	default:
		break;
	}
}

SDL_Texture* Player::getTexture() {
	return texture;
}

SDL_Rect* Player::getCoords() {
	return &coords;
}

void Player::setPos(int x, int y) {
	coords.x = x;
	coords.y = y;
}

void Player::setSize(int h, int w) {
	coords.h = h;
	coords.w = w;
}
bool Player::checkWallCollision(std::vector<SDL_Rect> &walls) {
	
	bool hitWall = false;
	
	for (auto &wall : walls) {
		if (coords.y + coords.h <= wall.y) {
			hitWall = false;
			//break;
		}

		if (coords.y >= wall.y + wall.h) {
			hitWall = false;
			//break;
		}

		if (coords.x >= wall.x + wall.w) {
			hitWall = false;
			//break;
		}

		if (coords.x + coords.w <= wall.x) {
			hitWall = false;
			//break;
		}
		else {
			hitWall = true;
		}
		
	}
	return hitWall;
};