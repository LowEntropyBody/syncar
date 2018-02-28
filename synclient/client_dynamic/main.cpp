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
	DynamicCap* dc = new DynamicCap();

	return 0;
}