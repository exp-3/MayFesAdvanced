#include "mbed.h"
#include "Iraira.hpp"
#include <stdlib.h>

int num[10][5][5] = {
	{
		{0,0,1,0,0},
		{0,1,0,1,0},
		{0,1,0,1,0},
		{0,1,0,1,0},
		{0,0,1,0,0}
	},
	{
		{0,1,1,0,0},
		{0,0,1,0,0},
		{0,0,1,0,0},
		{0,0,1,0,0},
		{0,1,1,1,0}
	},
	{
		{0,1,1,1,0},
		{0,0,0,1,0},
		{0,1,1,1,0},
		{0,1,0,0,0},
		{0,1,1,1,0}
	},
	{
		{0,1,1,1,0},
		{0,0,0,1,0},
		{0,1,1,1,0},
		{0,0,0,1,0},
		{0,1,1,1,0}
	},
	{
		{0,1,1,0,0},
		{1,0,1,0,0},
		{1,1,1,1,1},
		{0,0,1,0,0},
		{0,0,1,0,0}
	},
	{
		{0,1,1,1,0},
		{0,1,0,0,0},
		{0,1,1,0,0},
		{0,0,0,1,0},
		{0,1,1,0,0}
	},
	{
		{0,1,1,1,0},
		{0,1,0,0,0},
		{0,1,1,1,0},
		{0,1,0,1,0},
		{0,1,1,1,0}
	},
	{
		{0,1,1,1,0},
		{0,0,0,1,0},
		{0,0,0,1,0},
		{0,0,1,0,0},
		{0,1,0,0,0}
	},
	{
		{0,1,1,1,0},
		{0,1,0,1,0},
		{0,1,1,1,0},
		{0,1,0,1,0},
		{0,1,1,1,0}
	},
	{
		{0,1,1,1,0},
		{0,1,0,1,0},
		{0,1,1,1,0},
		{0,0,0,1,0},
		{0,1,1,1,0}
	}
};

typedef std::vector<Wall>::iterator witr;
typedef std::vector<Door>::iterator ditr;

Iraira *Iraira::mInstance = NULL;

Game *Iraira::getInstance() {
	if(mInstance == NULL) {
		mInstance = new Iraira();
	}

	return mInstance;
}

void Iraira::init() {
	timer.start();

	wallcount = 0;
	wallnum = 0;
	level = 0;

	knight.x = 8;
	knight.y = 4;
	knight.health = 3;
	knight.sensitive = 0.1;
	knightbrinkstate = 0;

	wallstartFlag = false;

	gameOverFlag = false;
	clearFlag = false;
}

void Iraira::update() {
    if(!clearFlag){
		// wallの更新
		if(!wallstartFlag){
			if(timer.read_ms() > 1000){ //壁を生成し始めるまで1秒まつ
				wallstartFlag = true;
				srand((knight.x + 10)*(knight.y + 10));
				timer.reset();
			}
		}else{
			int waittime;
			if(level < 10) waittime = 700 - level * 50;
			else waittime = 700 - 10 * 50;
			if(timer.read_ms() > waittime) {
				if(!walls.empty()){
					if(!doors.empty()){
						ditr temp;
						bool Doors_eraseFlag =false;
						for(ditr it = doors.begin();it != doors.end();it++){
							it->x++;
							if(!it->reverseFlag) it->y++;
							else it->y--;
							if(it->y > 10 || it->y < 0){
								temp = it;
								Doors_eraseFlag = true;
							}
						}
						if(Doors_eraseFlag) doors.erase(temp);
					}
					witr temp;
					bool eraseFlag = false;
					for(witr it = walls.begin();it != walls.end();it++){
						it->x += 1;
						if(it->x > 20){
							temp = it;
							eraseFlag = true;
						}
					}
					if(rand() % 20 == 0){
						doors.push_back(Door());
						doors.back().x = walls.back().x;
						if(rand() % 3 == 0) {
							doors.back().reverseFlag = true;
							doors.back().y = 7;
						}else {
							doors.back().reverseFlag = false;
							doors.back().y = 0;
						}
					}
					if(eraseFlag) walls.erase(temp);
				}
				if(wallcount % 6 == 0){
					walls.push_back(Wall());
					walls.back().x = 0;
					walls.back().passflag = false;
					int temp = rand() % 6;
					for(int i = 0;i < 8;i++) walls.back().wallarr[i] = true;
					for(int i = 0;i < 3;i++){
						if(temp + i > 7) continue;
						else walls.back().wallarr[temp + i] = false;
					}
				}
				wallcount++;
				/*難易度上昇*/
				if(wallcount % 10 == 9) level += 1;
				timer.reset();
			}
		}
		// 衝突判定
		if(!walls.empty()){
			witr temp;
			bool eraseFlag = false;
			for(witr it = walls.begin();it != walls.end();it++){
				if(knight.x == it->x){
					if(!it->passflag){
						wallnum++;
						it->passflag = true;
					}
					if(it->wallarr[knight.y]){
						if(it->passflag) wallnum--;
						knight.health--;
						temp = it;
						eraseFlag = true;
					}
				}
			}
			if(eraseFlag) walls.erase(temp);
		}
		if(!doors.empty()){
			ditr temp;
			bool eraseFlag = false;
			for(ditr it = doors.begin();it != doors.end();it++){
				for(int i = 0;i < 4;i++){
					if(it->x + i == knight.x && it->y == knight.y){
						knight.health--;
						temp = it;
						eraseFlag = true;
					}
				}
			}
			if(eraseFlag) doors.erase(temp);
		}
		if(knight.health == 0) {
			clearFlag = true;
		}
		// knightの更新
		int move_x = accel->getY() >> 6;
		if(move_x > 1) move_x = 1;
		else if(move_x < -1) move_x = -1;
		knight.x += move_x;
		if(knight.x < 0) knight.x = 0;
		else if(knight.x >15) knight.x = 15;

		int move_y = accel->getX() >> 6;
		if(move_y > 1) move_y = 1;
		else if(move_y < -1) move_y = -1;
		knight.y += move_y;
		if(knight.y < 0) knight.y = 0;
		else if(knight.y >7) knight.y = 7;

        /*描画*/
		// 壁の描画
		if(!walls.empty()){
			for(witr it = walls.begin();it != walls.end();it++){
				for(int i = 0;i < 8;i++){
					if(it->wallarr[i] && it->x < 16) display->set(i, it->x);
				}
			}
		}
		if(!doors.empty()){
			for(ditr it = doors.begin();it != doors.end();it++){
				for(int i = 0;i < 4;i++){
					if(it->y > 7 || (it->x + i) > 15) continue;
					display->set(it->y, it->x + i);
				}
			}
		}
		// knightの描画
		display->set(knight.y, knight.x);
		knightbrinkstate++;
		if(knightbrinkstate % 2 == 0){
			for(int i = 1;i < knight.health;i++){
				if(knight.x + i > 15) break;
				display->set(knight.y, knight.x + i);
			}
		}
	}else{
		int temp = (int)(timer.read_ms() / 100) % 16;
		if(temp < 8){
			display->set(0, 2*temp);
			display->set(0, 2*temp + 1);
		}else{
			display->set(7, 2*(15 - temp));
			display->set(7, 2*(15 - temp) + 1);
		}
		int order[3];
		order[0] = wallnum / 100;
		order[1] = wallnum / 10 - 10 * order[0];
		order[2] = wallnum % 10;
		if(order[0] >= 10){
			for(int i = 0;i < 3;i++) order[i] = 9;
		}
		for(int k = 0;k < 3;k++){
			for(int i = 0;i < 5;i++){
				for(int j = 0;j < 5;j++){
					if(num[order[k]][i][j]) display->set(i + 2, j + 5*k + 1);
				}
			}
		}
	}
}

bool Iraira::isGameOver() {
	return gameOverFlag;
}

bool Iraira::isCleared() {
	return clearFlag;
}

Iraira::Iraira() {
	display = Display::getInstance();
	accel = Accelerometer::getInstance();

	init();
}
