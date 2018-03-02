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
using namespace std;

int main(int argc, char* argv[]){
	/*
	CarSpeed* cs = new CarSpeed(); 
	cs -> move_frist_start();
	cs -> speed_x_y_z(-20, 20, 0);
	usleep(1000*2000);
	cs -> speed_x_y_z(0, 0, 0);
	*/
	
 	cout << "--------------------------START" << endl;
	CarSpeed* cs = new CarSpeed(); 
	cs -> move_frist_start();
	
	vector<ThreadCam*> cams;
	cams.push_back(new ThreadCam("/dev/video0", 640, 360, -35));
	cams.push_back(new ThreadCam("/dev/video1", 640, 360, 0));
	cams.push_back(new ThreadCam("/dev/video2", 640, 360, 35));
	for(int i = 0; i < cams.size(); i++) 
		cams[i] -> thread_run();
	
	int dd = 0;
	double time_move = 0;
	double time_move_flag = 0;
	while(dd < 1000){
		MoveInfo* move_info;
		for(int i = 0; i < cams.size(); i++){
			move_info = cams[i] -> get_move_info();
			if(move_info != NULL){
				cout << endl << "-----Cam Index:" << i + 1 << "------" << endl;
				time_move = 0;
				time_move_flag = move_info -> distance/3;
				cs -> move(move_info -> base_degree + move_info -> degree, move_info -> distance, 20);
				cams[i] -> show();
				break;
			}
		}
		usleep(1000*100);
		dd ++;
		if(dd%10 == 0) 
			cout << "This is " << dd/10 << "s" << endl;
		time_move ++;
		if(time_move > time_move_flag){
			cs -> speed_x_y_z(0, 0, 0);
		}
	}
	cs -> speed_x_y_z(0, 0, 0);
	//usleep(1000*10000);
	for(int i = 0; i < cams.size(); i++) 
		cams[i] -> thread_stop();
	cout << "-------------------------END" << endl;
	
	
	return 0;
}