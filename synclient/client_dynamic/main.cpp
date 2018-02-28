/*
**  Author: ZhaoYang
**	Compile: g++ synclient/client_dynamic/main.cpp -std=c++11 -ljpeg -lm -fpermissive -o dynamic.out
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
	Cam* cam = new Cam("/dev/video0", 640, 360);
	
	cam->takePic();
	cam->savePic("re1.jpg");
	usleep(1000*5000);
	
	cam->takePic();
	cam->savePic("re2.jpg");

	return 0;
}