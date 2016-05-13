#include "mbed.h"
#include "Display.hpp"
#include "Accelerometer.hpp"
#include "Game.hpp"
#include "PushSwitch.hpp"
#include "BreakOut.hpp"
#include "KeepStick.hpp"
#include "Iraira.hpp"

int waittime_ms = 100;
Game* Opening(Display *display){
	const int openingpanel[3][7][5] = {
		{
			{0,0,0,0,0},
			{0,0,0,0,0},
			{0,0,0,0,0},
			{0,0,0,0,0},
			{0,0,1,0,0},
			{0,0,1,0,0},
			{0,1,1,1,0}
		},
		{
			{0,0,1,1,0},
			{0,0,1,1,0},
			{0,0,0,0,0},
			{0,0,0,0,0},
			{0,0,0,0,0},
			{0,0,0,0,0},
			{1,1,1,1,0}
		},
		{
			{0,1,0,0,0},
			{0,1,0,0,0},
			{0,1,0,0,0},
			{0,0,0,0,0},
			{0,0,0,0,0},
			{0,0,0,0,0},
			{0,1,0,0,0}
		}
	};

	int x = 8, y = 4;
	Timer timer;
	timer.start();
	Accelerometer *accel;
	accel = Accelerometer::getInstance();
	PushSwitch *pushswitch;
	pushswitch = PushSwitch::getInstance();
	int count = 0;
	int mode;

	while(1){
		display->clear();
		/*処理*/
		if(timer.read_ms() > 200){
			count++;
			timer.reset();
		}
		if(x < 6){
			mode = 0;
		}else if(x < 11){
			mode = 1;
		}else{
			mode = 2;
		}

		x += accel->getY() >> 6;
		if(x < 0) x = 0;
		else if(x > 15) x = 15;
		y += accel->getX() >> 6;
		if(y < 0) y = 0;
		else if(y > 7) y = 7;

		/*ゲームモード確定*/
		if(pushswitch->isPressed()){
			pushswitch->reset();
			switch(mode){
			case 0:
				return KeepStick::getInstance();
				break;
			case 1:
        waittime_ms = 150;
				return BreakOut::getInstance();
				break;
			case 2:
				return Iraira::getInstance();
				break;
			default:
				break;
			}
		}

		/*描画*/
		for(int k = 0;k < 3;k++){
			for(int i = 0;i < 7;i++){
				for(int j = 0;j < 5;j++){
					if(openingpanel[k][i][j] == 1){
						display->set(i + 1, j + 5*k + 1);
					}
				}
			}
		}
		switch(mode){
		case 0:
			switch(count % 5){
			case 0:
				display->set(2, 3);
				display->set(3, 3);
				display->set(4, 3);
				break;
			case 1:
				display->set(2, 4);
				display->set(3, 3);
				display->set(4, 3);
				break;
			case 2:
				display->set(2, 2);
				display->set(3, 3);
				display->set(4, 3);
				break;
			case 3:
				display->set(3, 1);
				display->set(3, 2);
				display->set(4, 3);
				break;
			case 4:
				display->set(2, 2);
				display->set(3, 3);
				display->set(4, 3);
				break;
			default:
				break;
			}
			display->set(5, 6);
			display->set(3, 8);
			display->set(3, 9);

			display->set(2, 13);
			display->set(2, 14);
			display->set(2, 15);
			display->set(5, 15);
			break;
		case 1:
			switch(count % 5){
			case 0:
				display->set(5, 6);
				display->set(3, 8);
				display->set(3, 9);
				break;
			case 1:
				display->set(6, 7);
				display->set(3, 8);
				display->set(3, 9);
				break;
			case 2:
				display->set(5, 8);
				display->set(3, 8);
				display->set(3, 9);
				break;
			case 3:
				display->set(4, 9);
				display->set(3, 8);
				display->set(3, 9);
				break;
			case 4:
				display->set(5, 10);
				break;
			default:
				break;
			}
			display->set(2, 3);
			display->set(3, 3);
			display->set(4, 3);

			display->set(2, 13);
			display->set(2, 14);
			display->set(2, 15);
			display->set(5, 15);
			break;
		case 2:
			switch(count % 5){
			case 0:
				display->set(2, 13);
				display->set(2, 14);
				display->set(2, 15);
				display->set(5, 15);
				break;
			case 1:
				display->set(3, 13);
				display->set(3, 14);
				display->set(3, 15);
				display->set(5, 14);
				break;
			case 2:
				display->set(4, 13);
				display->set(4, 14);
				display->set(4, 15);
				display->set(5, 13);
				break;
			case 3:
				display->set(5, 13);
				display->set(5, 14);
				display->set(5, 15);
				display->set(5, 12);
				break;
			case 4:
				display->set(6, 13);
				display->set(6, 14);
				display->set(6, 15);
				display->set(5, 11);
				break;
			default:
				break;
			}
			display->set(2, 3);
			display->set(3, 3);
			display->set(4, 3);

			display->set(5, 6);
			display->set(3, 8);
			display->set(3, 9);
			break;
		default:
			break;
		}
		display->set(0, x);
		display->set(y, 0);

		display->flush();
		wait(0.1);
	}
}

int main() {
	Display *display = Display::getInstance();

	Game *game = Opening(display);

	while(1) {
		//描画開始
		display->clear();
		game->update();
		display->flush();

		wait_ms(waittime_ms);
	}
}
