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
#include <math.h>
#include <iostream>
#include <vector>

int main(int argc, char* argv[]){
	/**
	Cam* cam1 = new Cam("/dev/video0", 640, 360);
	Cam* cam2 = new Cam("/dev/video1", 640, 360);
	Cam* cam3 = new Cam("/dev/video2", 640, 360);
	cam1 -> take_pic();
	cam1 -> save_pic("re1.jpg");
	cam2 -> take_pic();
	cam2 -> save_pic("re2.jpg");
	cam3 -> take_pic();
	cam3 -> save_pic("re3.jpg");
	**/
 	
	
	ThreadCam* tc1 = new ThreadCam("/dev/video0", 640, 360);
	ThreadCam* tc2 = new ThreadCam("/dev/video1", 640, 360);
	ThreadCam* tc3 = new ThreadCam("/dev/video2", 640, 360);
	tc1->thread_run();
	tc2->thread_run();
	tc3->thread_run();
	
	int i = 0;
	int j = 0;
	int k = 0;
	while(i < 3 || j < 3 || k < 3){
		if(tc1 -> get_move_info() != NULL){
			cout<< "1111111:" << endl;
			tc1 -> show();
			i++;
		}
		if(tc2 -> get_move_info() != NULL){
			cout<< "222222:" << endl;
			tc2 -> show();
			j++;
		}
		if(tc3 -> get_move_info() != NULL){
			cout<< "333333:" << endl;
			tc3 -> show();
			k++;
		}
		usleep(1000*10);
	}
	//usleep(1000*10000);
	tc1->thread_stop();
	tc2->thread_stop();
	tc3->thread_stop();
	cout << "main" << endl;
	
	return 0;
}