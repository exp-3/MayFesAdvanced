#ifndef KEEPSTICK_HPP
#define KEEPSTICK_HPP

#include "Game.hpp"
#include "Display.hpp"
#include "Accelerometer.hpp"
#include "PushSwitch.hpp"

class Stick {
public:
	//重心位置は棒の先端
	float rad, rad_v; //棒の角度, 棒の角速度
	float cos, sin;
	int length; //棒のながさ
	void tri(void){
		float x = rad, x2, x3, x4, x5;
		x2 = x * x;
		x3 = x * x2;
		x4 = x * x3;
		x5 = x * x4;
		cos = 1.0 - x2/2 + x4/24;
		sin = x - x3/6 + x5/120;
	}
};

class kBar {
public:
	int x;
	int width;
};

class KeepStick : Game {
public:
	static Game *getInstance();
	void init();
	void update();
	bool isGameOver();
	bool isCleared();

private:
	KeepStick();
	~KeepStick();

	static KeepStick *mInstance;

	int W_GRIDS; //セグメントLEDの横方向の個数
	float SENSITIVE;
	int MAXTIME;
	int time; //残り時間

	Display *display;
	Accelerometer *accel;
	PushSwitch *pushswitch;
	
	Stick stick;
	kBar bar;

	Timer timer;

	bool gameOverFlag;
	bool clearFlag;
};

#endif /* end of include guard: KEEPSTICK_HPP */
