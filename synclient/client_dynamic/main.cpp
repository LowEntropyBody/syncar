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
	
	FILE* out = fopen("result.jpg", "w");
	jpeg(out, cam->getPic(), cam->width, cam->height, 100);
	fclose(out);
	
	FILE* out2 = fopen("result2.jpg", "w");
	jpeg(out2, cam->getrgb(), cam->width, cam->height, 100);
	fclose(out2);

	return 0;
}