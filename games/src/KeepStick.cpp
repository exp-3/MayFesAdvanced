#include "mbed.h"
#include "KeepStick.hpp"

KeepStick *KeepStick::mInstance = NULL;

// 8888888b.  888     888 888888b.   888      8888888 .d8888b.
// 888   Y88b 888     888 888  "88b  888        888  d88P  Y88b
// 888    888 888     888 888  .88P  888        888  888    888
// 888   d88P 888     888 8888888K.  888        888  888
// 8888888P"  888     888 888  "Y88b 888        888  888
// 888        888     888 888    888 888        888  888    888
// 888        Y88b. .d88P 888   d88P 888        888  Y88b  d88P
// 888         "Y88888P"  8888888P"  88888888 8888888 "Y8888P"

Game *KeepStick::getInstance() {
	if(mInstance == NULL) {
		mInstance = new KeepStick();
	}

	return mInstance;
}

void KeepStick::update() {
	/*処理*/
	// stickの更新
	if(stick.rad >= 1.57 || stick.rad <= -1.57){
		if(stick.rad >= 1.57) stick.rad = 1.57;
		if(stick.rad >= -1.57) stick.rad = -1.57;
		gameOverFlag = true;
		clearFlag = false;
	}else{		
		stick.rad += stick.rad_v;
		stick.rad_v += (double)(accel->getY() >> 5) * SENSITIVE;
		stick.tri();
	}
    // barの更新
	bar.x += accel->getY() >> 5;
	if(bar.x < 0) {
		bar.x = 0;
	} else if(bar.x > 13) {
		bar.x = 13;
	}
	// timeの更新
	
	
	/*描画*/	
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
		display->set(6 - (int)(i * stick.cos), bar.x + 1 + (int)(i * stick.sin));
	}
}

bool KeepStick::isGameOver() {
	return gameOverFlag;
}

bool KeepStick::isCleared() {
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

KeepStick::KeepStick() {
	display = Display::getInstance();
	accel = Accelerometer::getInstance();

	stick.rad = 0.0;
	stick.rad_v = 0.0;
	stick.length = 5;

	bar.x = 6;
	bar.width = 3;
	
	gameOverFlag = false;
	clearFlag = true;
}
