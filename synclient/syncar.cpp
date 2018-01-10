/*
**  Author: ZhaoYang
**	Compile: g++ syncar.cpp -ljpeg -lm -fpermissive -o syncar.out
**  Run: ./syncar.out -devId 1 -serverIP "166.111.66.49"
**  Lib: sudo apt-get install libv4l-dev
**		 sudo apt-get install libjpeg-dev
**  Download: git clone https://github.com/LowEntropyBody/syncar.git
**  Date: 2018/1
*/

#include "base/httplib.h"
#include "base/move.h"
#include "base/cap.h"
#include "base/dealimg.h"
#include <math.h>
#include <iostream>
using namespace std;

// 标识设备id
string devId = "1";
// 服务器ip
string serverIP = "166.111.66.49";

int main(int argc, char* argv[])
{
	cout<<"------system prepare------"<<endl;
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
	cout<<"------wait for starting......------"<<endl;
	// 连接网络
	httplib::Client cli(serverIP.c_str(), 8001);
	// 等待50s服务器给指令
	int i = 0;
	for(i = 0; i < 100; i++){
		if(i%2 == 0) cout<<"wait->"<<i/2<<"s"<<endl;
		string send = "devId=";
		send = send + devId;
		auto res = cli.post("/start/", send.c_str(), "application/x-www-form-urlencoded");
		if (res && res->status == 200) {
			string body = res->body;
			if(body == "1")	break; else usleep(1000*500);
		}else{
			cout<<"------network failed------"<<endl;
			exit(-1);
		}
	}
	if(i == 100){
		cout<<"------server command timeout------"<<endl;
		exit(-1);
	}
	// 启动
	cout<<"------system start------"<<endl;
	return   0;
}