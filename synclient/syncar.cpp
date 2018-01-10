/*
**  Author: ZhaoYang
**	Compile: g++ syncar.cpp -ljpeg -lm -fpermissive -o syncar.out
**  Run: ./syncar.out -devId 1 -serverIP "166.111.66.49"
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

//标识设备id
int devId = 1;
//服务器ip
char* serverIP = "166.111.66.49";

int main(int argc, char* argv[])
{
	cout<<"system prepare"<<endl;
	if( argc >= 2){
		devId = argv[2];
		cout<<"read dev Id: "<<devId<<endl;
	}else{
		cout<<"use default dev Id: "<<devId<<endl;
	}
	if( argc >= 4){
		serverIP = argv[4];
		cout<<"read server IP: "<<serverIP<<endl;
	}else{
		cout<<"use default server IP: "<<serverIP<<endl;
	}
	cout<<"wait for starting......"<<endl;
	
	return   0;
}