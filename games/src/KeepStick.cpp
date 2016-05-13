#include "mbed.h"
#include "KeepStick.hpp"

int clearpanel[8][16] = {
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,1,0,1,0,0,1,1,0,0,1,0,0,1,1,0},
	{1,0,0,1,0,0,1,0,0,1,0,1,0,1,0,1},
	{1,0,0,1,0,0,1,1,0,1,0,1,0,1,1,0},
	{1,0,0,1,0,0,1,0,0,1,1,1,0,1,0,1},
	{0,1,0,1,1,0,1,1,0,1,0,1,0,1,0,1},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

KeepStick *KeepStick::mInstance = NULL;

Game *KeepStick::getInstance() {
	if(mInstance == NULL) {
		mInstance = new KeepStick();
	}

	return mInstance;
}

void KeepStick::update() {
	/*処理*/
	if(!clearFlag){
		// stickの更新
		stick.tri();
		if(stick.cos <= 0){
			gameOverFlag = true;
		}else{		
			stick.rad += stick.rad_v;
			double temp;
			if(stick.rad < 0) temp = -0.1;
			else temp = 0.1;
			stick.rad_v -= ((double)(accel->getY() >> 5) + temp) * SENSITIVE;
		}
		// barの更新
		bar.x += accel->getY() >> 5;
		if(bar.x < 0) {
			bar.x = 0;
		} else if(bar.x > 11) {
			bar.x = 11;
		}
		// timeの更新
		if(timer.read_ms() > 1000){
			time--;
			if(time < 0) clearFlag = true;
			timer.reset();
		}
	}
	/*描画*/
	if(clearFlag){
		int temp = (int)(timer.read_ms() / 100) % 16;
		if(temp < 8){
			display->set(0, 2*temp);
			display->set(0, 2*temp + 1);
		}else{
			display->set(7, 2*(15 - temp));
			display->set(7, 2*(15 - temp) + 1);
		}
		for(int i = 0;i < 8;i++){
			for(int j = 0;j < 16;j++){
				if(clearpanel[i][j] == 1) display->set(i, j);
			}
		}
	}
	else{
		// 残り時間の描画
		for(int i = 0;i < time; i++){
			for(int j = 0;j < W_GRIDS / MAXTIME; j++){
				display->set(0, i * W_GRIDS / MAXTIME + j);
			}
		}
		// barの描画
		for(int i = 0; i < bar.width; i++) {
			display->set(7, bar.x + i);
		}
		// stickの描画
		for(double i = 0.5; (int)i < stick.length; i += 1.0){
			int temp = bar.x + 2 + (int)(i * stick.sin);
			if(temp >= 0 && temp <= 15){
				display->set(6 - (int)(i * stick.cos), bar.x + 2 + (int)(i * stick.sin));
			}
		}
	}
}

bool KeepStick::isGameOver() {
	return gameOverFlag;
}

bool KeepStick::isCleared() {
	return clearFlag;
}

KeepStick::KeepStick() {
	display = Display::getInstance();
	accel = Accelerometer::getInstance();

	timer.start();
	time = MAXTIME;
	
	stick.rad = 0.0;
	stick.rad_v = 0.2;
	stick.length = 5;

	bar.x = 6;
	bar.width = 5;
	
	gameOverFlag = false;
	clearFlag = false;
}
