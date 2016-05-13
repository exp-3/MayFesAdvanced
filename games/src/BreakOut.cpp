#include "mbed.h"
#include "BreakOut.hpp"

BreakOut *BreakOut::mInstance = NULL;

// 8888888b.  888     888 888888b.   888      8888888 .d8888b.
// 888   Y88b 888     888 888  "88b  888        888  d88P  Y88b
// 888    888 888     888 888  .88P  888        888  888    888
// 888   d88P 888     888 8888888K.  888        888  888
// 8888888P"  888     888 888  "Y88b 888        888  888
// 888        888     888 888    888 888        888  888    888
// 888        Y88b. .d88P 888   d88P 888        888  Y88b  d88P
// 888         "Y88888P"  8888888P"  88888888 8888888 "Y8888P"

void BreakOut::init() {
	ball.x  = 0;
	ball.y  = 4;
	ball.vx = 1;
	ball.vy = 1;

	bar.x = 6;

	for(int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {
			blocks[i][j] = true;
		}
	}

	gameOverFlag = false;
	clearFlag = true;
}

Game *BreakOut::getInstance() {
	if(mInstance == NULL) {
		mInstance = new BreakOut();
	}

	return mInstance;
}

void BreakOut::update() {
	/*初期化*/
	if(pushswitch->isPressed()){
		pushswitch->reset();
		init();
	}
	if(!gameOverFlag){
		// ballの衝突判定
		if(ball.y == 6 && ball.x >= bar.x && ball.x < bar.x + 4) {
			ball.vy = -ball.vy;
		}

		for(int i = 0; i < height; i++) {
			for(int j = 0; j < width; j++) {
				if(blocks[i][j]) {
					if((ball.y == i + 1 && ball.x >= 3 * j + 1 && ball.x < 3 * j + 3 &&
						ball.vy < 0) ||
					   (ball.y == i - 1 && ball.x >= 3 * j + 1 && ball.x < 3 * j + 3 &&
						ball.vy > 0)) {
						blocks[i][j] = false;
						ball.vy      = -ball.vy;
					}
					if(ball.y == i && ((ball.x == 3 * j && ball.vx > 0) ||
									   (ball.x == 3 * j + 3 && ball.vx < 0))) {
						blocks[i][j] = false;
						ball.vx      = -ball.vx;
					}
				}
			}
		}

		// barの更新
		bar.x += accel->getY() >> 5;
		if(bar.x < 0) {
			bar.x = 0;
		} else if(bar.x > 12) {
			bar.x = 12;
		}

		// ballの更新
		ball.x += ball.vx;
		if(ball.x <= 0) {
			ball.x  = 0;
			ball.vx = -ball.vx;
		} else if(ball.x >= 15) {
			ball.x  = 15;
			ball.vx = -ball.vx;
		}

		ball.y += ball.vy;
		if(ball.y <= 0) {
			ball.y  = 0;
			ball.vy = -ball.vy;
		} else if(ball.y >= 8) {
			gameOverFlag = true;
			clearFlag = false;
		}
	}
	// blockの描画
	for(int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {
			if(blocks[i][j]) {
				for(int k = 0; k < 2; k++) {
					display->set(i, 3 * j + k + 1);
				}
			}
		}
	}

	// barの描画
	for(int i = 0; i < 4; i++) {
		display->set(7, bar.x + i);
	}

	// ballの描画
	display->set(ball.y, ball.x);
}

bool BreakOut::isGameOver() {
	return gameOverFlag;
}

bool BreakOut::isCleared() {
	return clearFlag;
}

// 8888888b.  8888888b.  8888888 888     888     d8888 88888888888 8888888888
// 888   Y88b 888   Y88b   888   888     888    d88888     888     888
// 888    888 888    888   888   888     888   d88P888     888     888
// 888   d88P 888   d88P   888   Y88b   d88P  d88P 888     888     8888888
// 8888888P"  8888888P"    888    Y88b d88P  d88P  888     888     888
// 888        888 T88b     888     Y88o88P  d88P   888     888     888
// 888        888  T88b    888      Y888P  d8888888888     888     888
// 888        888   T88b 8888888     Y8P  d88P     888     888     8888888888

BreakOut::BreakOut() {
	display = Display::getInstance();
	accel = Accelerometer::getInstance();
	pushswitch = PushSwitch::getInstance();
  
	init();
}
