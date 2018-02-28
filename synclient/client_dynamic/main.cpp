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
	tc1->thread_run();
	int i = 0;
	while(i < 10){
		if(tc1 -> get_move_info() != NULL){
			tc1 -> show();
			i++;
		}
	}
	tc1->thread_stop();
	cout << "main" << endl;
	return 0;
}