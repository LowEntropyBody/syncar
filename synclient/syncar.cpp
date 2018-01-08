/*
**  Author: ZhaoYang
**	Compile: g++ syncar.cpp -ljpeg -lm -fpermissive -o syncar.out
**  Lib: sudo apt-get install libv4l-dev
**		 sudo apt-get install libjpeg-dev
**  Date: 2018/1
*/

#include "base/httplib.h"
#include "base/move.h"
#include "base/cap.h"
#include "base/dealimg.h"
#include <math.h>
#include <iostream>
using namespace std;


int main(int argc, char* argv[])
{
	int i;
	for(i = 0; i < argc; i++)
		cout << argv[i] << endl;
	return   0;
}