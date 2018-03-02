/*
**  Author: ZhaoYang
**	Compile: g++ synclient/client_dynamic/capture.cpp -lpthread -std=c++11 -ljpeg -lm -fpermissive -o capture.out
**  Run: ./capture.out
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
	Cam* cam1 = new Cam("/dev/video0", 640, 360);
	Cam* cam2 = new Cam("/dev/video1", 640, 360);
	Cam* cam3 = new Cam("/dev/video2", 640, 360);
	cam1 -> take_pic();
	cam1 -> save_pic("re1.jpg");
	cam2 -> take_pic();
	cam2 -> save_pic("re2.jpg");
	cam3 -> take_pic();
	cam3 -> save_pic("re3.jpg");
	return 0;
}