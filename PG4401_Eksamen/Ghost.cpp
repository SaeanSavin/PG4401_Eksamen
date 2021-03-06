#include "Ghost.h"

Ghost::Ghost(SDL_Texture* t, SDL_Renderer* r, std::vector<SDL_Rect>& w, enum class TargetType m, enum class GhostType g)
	: texture(t), renderer(r), walkable(w), targetMode(m), type(g)
{
	coords.h = 0;
	coords.w = 0;
	coords.x = 0;
	coords.y = 0;
}
Ghost::Ghost(SDL_Texture* t, SDL_Renderer* r, SDL_Rect c, std::vector<SDL_Rect>& w, enum class TargetType m, enum class GhostType g)
	:texture(t), renderer(r), coords(c), walkable(w), targetMode(m), type(g) {}

void Ghost::move(int &SCREEN_WIDTH, int &SCREEN_HEIGHT, std::vector<std::vector<char>> &map, std::vector<SDL_Rect> &walls) {

	SDL_PumpEvents();

	//calculate direction based on target
	if (checkTileEntered(walkable)) {

		if (eaten) {
			speed = 2;
		}
		else {
			speed = 1;
		}

		if (eaten && coords.x == spawn.first && coords.y == spawn.second) {
			eaten = false;
			speed = 1;
		}

		if (isHome(walkable, map) && !eaten) { 
			target.first = coords.x;
			target.second = 0;
		}

		double closestDir = SCREEN_WIDTH > SCREEN_HEIGHT ? SCREEN_WIDTH * 2 : SCREEN_HEIGHT * 2;
		double newDir = 0;
		char newDirection = direction;

		if (direction != 'd' && !checkWallCollision(walls, -1, 0)) {
			closestDir = std::sqrt(std::pow(abs((coords.x - 16) - target.first),2) + std::pow(abs((coords.y) - target.second),2));
			newDirection = 'a';
		} 

		//the map-check makes sure ghosts doesn't return to ghost-house
		if (direction != 'w' && !checkWallCollision(walls, 0, 1) && (map[(coords.y - 50 + 16) / 16][coords.x / 16] != '~' || eaten)) {
			newDir = std::sqrt(std::pow(abs((coords.x) - target.first), 2) + std::pow(abs((coords.y + 16) - target.second), 2));
			if (newDir <= closestDir) {
				closestDir = newDir;
				newDirection = 's';
			}
		}

		if (direction != 'a' && !checkWallCollision(walls, 1, 0)) {
			newDir = std::sqrt(std::pow(abs((coords.x + 16) - target.first), 2) + std::pow(abs((coords.y) - target.second), 2));
			if (newDir <= closestDir) {
				closestDir = newDir;
				newDirection = 'd';
			}
		}

		if (direction != 's' && !checkWallCollision(walls, 0, -1)) {
			newDir = std::sqrt(std::pow(abs((coords.x) - target.first), 2) + std::pow(abs((coords.y - 16) - target.second), 2));
			if (newDir <= closestDir) {
				closestDir = newDir;
				newDirection = 'w';
			}
		}

		direction = newDirection;
		collided = 'n';
	}

	//if out of bounds, re-enter on the opposite side
	else {

		//Make sure image dont move out of bounds
		if (coords.x > SCREEN_WIDTH && direction == 'd') {
			coords.x = -coords.w;
		}

		if (coords.y > SCREEN_HEIGHT && direction == 's') {
			coords.y = -coords.h;
		}

		if (coords.x < -coords.w && direction == 'a') {
			coords.x = SCREEN_WIDTH;
		}

		if (coords.y < -coords.h && direction == 'w') {
			coords.y = SCREEN_HEIGHT;
		}
	}
	//decide movement
	switch (direction) {
	case 'w':
		if (!checkWallCollision(walls, 0, -1)) {
			coords.y -= speed;
			if (eaten) {
				animate(*animations["eaten_up"]);
			}
			else if (frightenedEnding) {
				animate(*animations["frightened_ending"]);
			}
			else if (frightened) {
				animate(*animations["frightened"]);
			}
			else {
				animate(*animations["up"]);
			}
		}
		collided = 'n';
		break;
	case 's':
		if (!checkWallCollision(walls, 0, 1)) {
			coords.y += speed;
			if (eaten) {
				animate(*animations["eaten_down"]);
			}
			else if (frightenedEnding) {
				animate(*animations["frightened_ending"]);
			}
			else if (frightened) {
				animate(*animations["frightened"]);
			}
			else {
				animate(*animations["down"]);
			}
		}
		collided = 'n';
		break;
	case 'a':
		if (!checkWallCollision(walls, -1, 0)) {
			coords.x -= speed;
			if (eaten) {
				animate(*animations["eaten_left"]);
			}
			else if (frightenedEnding) {
				animate(*animations["frightened_ending"]);
			}
			else if (frightened) {
				animate(*animations["frightened"]);
			}
			else {
				animate(*animations["left"]);
			}
		}
		collided = 'n';
		break;
	case 'd':
		if (!checkWallCollision(walls, 1, 0)) {
			coords.x += speed;
			if (eaten) {
				animate(*animations["eaten_right"]);
			}
			else if (frightenedEnding) {
				animate(*animations["frightened_ending"]);
			}
			else if (frightened) {
				animate(*animations["frightened"]);
			}
			else {
				animate(*animations["right"]);
			}
		}
		collided = 'n';
		break;
	case 'i':
		renderTexture();
	default:
		break;
	}
}

bool Ghost::checkTileEntered(std::vector<SDL_Rect> &walkable) {

	for (auto& mapTile : walkable) {
		if (coords.y == mapTile.y && coords.x == mapTile.x) {
			return true;
		}
	}
	return false;
}

bool Ghost::isHome(std::vector<SDL_Rect> &walkable, std::vector<std::vector<char>> &map) {
	for (auto& mapTile : walkable) {
		if (coords.y == mapTile.y && coords.x == mapTile.x && map[(coords.y - 50) / 16][coords.x / 16] == '~') {
			return true;
		}
	}
	return false;
}

void Ghost::setTarget(std::pair<int, int> t) {
	target = t;
}

SDL_Texture* Ghost::getTexture() {
	return texture;
}

SDL_Rect* Ghost::getCoords() {
	return &coords;
}

void Ghost::setPos(int x, int y) {
	coords.x = x;
	coords.y = y;
}

void Ghost::setSize(int h, int w) {
	coords.h = h;
	coords.w = w;
}

void Ghost::setSpawnPos(int x, int y) {
	spawn.first = x;
	spawn.second = y;
	setPos(x, y);
}

std::pair<int, int> Ghost::getSpawnPos() {
	return spawn;
}

void Ghost::respawn() {
	setPos(spawn.first, spawn.second);
	eaten = false;
	frightened = false;
	frightenedEnding = false;
}

enum class TargetType Ghost::getTargetMode() {
	return targetMode;
}

void Ghost::animate(Animation a) {
	SDL_RenderCopy(renderer, a.getFrame(), nullptr, &coords);
}

void Ghost::animateEx(Animation a, char direction) {
	SDL_RendererFlip flipType = SDL_FLIP_NONE;
	double rotation = 0;
	if (direction == 'a') {
		flipType = SDL_FLIP_HORIZONTAL;
	}
	else if (direction == 'w') {
		flipType = SDL_FLIP_VERTICAL;
		rotation = 270;
	}
	else if (direction == 's') {
		rotation = 90;
	}
	SDL_RenderCopyEx(renderer, a.getFrame(), nullptr, &coords, rotation, nullptr, flipType);
}

void Ghost::renderTexture() {
	SDL_RenderCopy(renderer, texture, nullptr, &coords);
}

bool Ghost::checkWallCollision(std::vector<SDL_Rect>& walls, int x_offset, int y_offset) {
	int x = coords.x + x_offset;
	int y = coords.y + y_offset;
	for (auto& wall : walls) {
		if (y + coords.h > wall.y && y < wall.y + wall.h) {
			if (x + coords.w > wall.x && x < wall.x + wall.w) {
				if (direction != 'i') {
					collided = direction;
					renderTexture();
				}
				return true;
			}
		}
	}
	return false;
};
