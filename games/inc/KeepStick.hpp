#ifndef KEEPSTICK_HPP
#define KEEPSTICK_HPP

#include "Game.hpp"
#include "Display.hpp"
#include "Accelerometer.hpp"

class Stick {
public:
	//重心位置は棒の先端
	double rad, rad_v; //棒の角度, 棒の角速度
	double cos, sin;
	int length; //棒のながさ
	void tri(void){
		double x = rad, x2, x3, x4, x5;
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
	void update();
	bool isGameOver();
	bool isCleared();

private:
	KeepStick();
	~KeepStick();

	static KeepStick *mInstance;

	static const int W_GRIDS = 16; //セグメントLEDの横方向の個数
	static const int MAXTIME = 8; //初期制限時間
	static const double SENSITIVE = 0.2;
	int time = MAXTIME; //残り時間

	Display *display;
	Accelerometer *accel;
	Stick stick;
	kBar bar;

	bool gameOverFlag;
	bool clearFlag;
};

#endif /* end of include guard: KEEPSTICK_HPP */
