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
	ThreadCam* tc1 = new ThreadCam("/dev/video0", 640, 360);
	ThreadCam* tc2 = new ThreadCam("/dev/video1", 640, 360);
	tc1->thread_run();
	tc2->thread_run();
	int i = 0;
	int j = 0;
	while(i < 10 || j < 10){
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
		usleep(1000*10);
	}
	tc1->thread_stop();
	cout << "main" << endl;
	return 0;
}