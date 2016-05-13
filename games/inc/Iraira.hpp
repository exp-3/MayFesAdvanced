#ifndef IRAIRA_HPP
#define IRAIRA_HPP

#include "Game.hpp"
#include "Display.hpp"
#include "Accelerometer.hpp"
#include <vector>

class Wall {
public:
	int x;
	bool wallarr[8];
	bool passflag;
};

class Door{
public:
	int x, y;
	bool reverseFlag;
};

class Knight {
public:
	int x, y;
	int health;
	double sensitive;
};

class Iraira : Game {
public:
	static Game *getInstance();
	void update();
	bool isGameOver();
	bool isCleared();

private:
	Iraira();
	~Iraira();

	static Iraira *mInstance;

	Display *display;
	Accelerometer *accel;

	Knight knight;
	int knightbrinkstate;
	
	int wallcount;
	int wallnum;
	int level;

	Timer timer, ktimer;
	
	std::vector<Wall> walls;
	std::vector<Door> doors;
	
	bool wallstartFlag;
	bool gameOverFlag;
	bool clearFlag;
};

#endif /* end of include guard: IRAIRA_HPP */
