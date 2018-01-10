/*
**  Author: ZhaoYang
**	Compile: g++ synclient/syncar.cpp -ljpeg -lm -fpermissive -o syncar.out
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

class RectTarget {	
	private:
		int (*cm)(int,int,int);
		int width;
		int hight;
		int id;
		double distance;
		double degree;
		aim_infor* infor;
	public:
		RectTarget(int id_temp, int (*color_match_temp)(int,int,int), int width_temp, int hight_temp);
		bool findTarget();
		aim_infor* getAimInfor();
		double getDistance();
		double getDegree();
		void show();
};

// 标识设备id
string devId = "1";
// 服务器ip
string serverIP = "166.111.66.49";

int main(int argc, char* argv[])
{
	cout << "------system prepare------" << endl;
	if( argc >= 2){
		devId = argv[2];
		cout << " read dev Id: " << devId << endl;
	}else{
		cout << " use default dev Id: " << devId << endl;
	}
	if( argc >= 4){
		serverIP = argv[4];
		cout << " read server IP: " << serverIP << endl;
	}else{
		cout << " use default server IP: " << serverIP << endl;
	}
	cout << "------wait for starting------" << endl;
	// 连接网络
	httplib::Client cli(serverIP.c_str(), 8001);
	// 等待50s服务器给指令
	int i = 0;
	for(i = 0; i < 100; i++){
		if(i%2 == 0) cout << " " << i/2 + 1 << "s pass..." << endl;
		string send = "devId=";
		send = send + devId;
		auto res = cli.post("/start/", send.c_str(), "application/x-www-form-urlencoded");
		if (res && res->status == 200) {
			string body = res->body;
			if(body == "1")	break; else usleep(1000*500);
		}else{
			cout << "------network failed------" << endl;
			exit(-1);
		}
	}
	if(i == 100){
		cout << "------server command timeout------" << endl;
		exit(-1);
	}
	// 启动
	cout << "------system start------" << endl;
	RectTarget rt(1, color_match_red, 8.5, 21);
	rt.findTarget();
	rt.show();
	return   0;
}

//构造函数
RectTarget::RectTarget(int id_temp, int (*color_match_temp)(int,int,int), int width_temp, int hight_temp){
	id = id_temp;
	cm = color_match_temp;
	width = width_temp;
	hight = hight_temp;
	distance = -1;
	degree = -1;
	infor = NULL;	
}
//找目标返回是否找到目标
bool RectTarget::findTarget(){
	return 0;
}

aim_infor* RectTarget::getAimInfor(){
	return infor;
}
//返回目标距离
double RectTarget::getDistance(){
	return distance;
}
//返回目标偏角
double RectTarget::getDegree(){
	return degree;
}
//输出信息
void show(){
	cout << "id: " << id << endl;
	if(cm == color_match_red) cout << "color: red" << endl;
	if(cm == color_match_green) cout << "color: green" << endl;
	cout << "width: " << width << endl;
	cout << "hight: " << hight << endl;
	cout << "distance: " << distance << endl;
	cout << "degree: " << degree << endl;
	if(infor != NULL){
		if(infor->isfind){
			cout << "infor->isfind: " << "yes" << endl;
			cout << "center: (" << infor->center_x <<", "<< infor->center_y <<")"<<endl;
			cout << "infor->l: " << infor->l <<endl;
			cout << "infor->w: " << infor->w <<endl;
			cout << "infor->area: " << infor->area <<endl;
		}else cout << "can not find target" << endl;
	}else cout << "infor is NULL, use getAimInfor() to find" << endl;
}







