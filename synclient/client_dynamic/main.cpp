/*
**  Author: ZhaoYang
**	Compile: g++ synclient/client_dynamic/main.cpp -lpthread -std=c++11 -ljpeg -lm -fpermissive -o dynamic.out
**  Run: ./dynamic.out
**  Lib: sudo apt-get install libv4l-dev
**		 sudo apt-get install libjpeg-dev
**  Download: git clone https://github.com/LowEntropyBody/syncar.git
**  Date: 2018/2
*/
#include "base/dynamiccap.h"
#include "base/car.h"
#include <math.h>
#include <iostream>
#include <vector>

int main(int argc, char* argv[]){
	CarSpeed* cs = new CarSpeed(); 
	cs -> move_frist_start();
	cs -> speed_x_y_z(10,10,0);
	usleep(1000*2);
	cs -> speed_x_y_z(0,0,0);
	/**
 	cout << "--------------------------START" << endl;
	Car* car = new Car();
	car -> move_frist_start();
	
	vector<ThreadCam*> cams;
	cams.push_back(new ThreadCam("/dev/video0", 640, 360, -35));
	cams.push_back(new ThreadCam("/dev/video1", 640, 360, 0));
	cams.push_back(new ThreadCam("/dev/video2", 640, 360, 35));
	for(int i = 0; i < cams.size(); i++) 
		cams[i] -> thread_run();
	
	int dd = 0;
	while(dd < 1000){
		MoveInfo* move_info;
		for(int i = 0; i < cams.size(); i++){
			move_info = cams[i] -> get_move_info();
			if(move_info != NULL){
				cout << endl << "-----Cam Index:" << i + 1 << "------" << endl;
				cout << "now_degree:" << car -> now_degree << endl;
				cout << "base_degree:" << move_info -> base_degree << endl;
				cout << "degree:" << move_info -> degree << endl;
				car -> move_rotate(car -> now_degree + move_info -> base_degree + move_info -> degree);
				car -> move_forward(move_info -> distance/2);
				cams[i] -> show();
				break;
			}
		}
		usleep(1000*100);
		dd ++;
		if(dd%10 == 0) cout << "This is " << dd/10 << "s" << endl;
	}
	//usleep(1000*10000);
	for(int i = 0; i < cams.size(); i++) 
		cams[i] -> thread_stop();
	cout << "-------------------------END" << endl;
	**/
	
	return 0;
}