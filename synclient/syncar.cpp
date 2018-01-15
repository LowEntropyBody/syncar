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
#include "base/car.h"
#include "base/cap.h"
#include "base/dealimg.h"
#include <math.h>
#include <iostream>
#include <vector>
using namespace std;

// 标识设备id
string devId = "1";
// 服务器ip
string serverIP = "166.111.66.49";
// 识别目标们
vector<RectTarget*> rts;
// 小车运动
Car car;

int main(int argc, char* argv[])
{
	cout << endl << "------system prepare------" << endl;
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
	
	cout << endl << "------wait for starting------" << endl;
	httplib::Client cli(serverIP.c_str(), 8001);
	// 50s
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
			cout << endl << "------network failed------" << endl;
			exit(-1);
		}
	}
	if(i == 100){
		cout << endl << "------server command timeout------" << endl;
		exit(-1);
	}
	
	cout << endl << "------system start------" << endl;
	car.move_frist_start();
	cout << " car motative system start" << endl;
	rts.push_back(new RectTarget("1", color_match_red, 8.2, 21));
	rts.push_back(new RectTarget("2", color_match_red, 5.6, 21));
	rts.push_back(new RectTarget("3", color_match_red, 8.2, 21));
	rts.push_back(new RectTarget("4", color_match_red, 5.6, 21));
	rts.push_back(new RectTarget("5", color_match_red, 8.2, 21));
	rts.push_back(new RectTarget("6", color_match_red, 5.6, 21));
	//rts.push_back(new RectTarget("target3", color_match_green, 8.2, 21));
	//rts.push_back(new RectTarget("target4", color_match_green, 5.6, 21));
	//rts.push_back(new RectTarget("target5", color_match_blue, 8.2, 21));
	//rts.push_back(new RectTarget("target6", color_match_blue, 5.6, 21));
	cout << " target information load success" << endl;
	// 旋转角度
	int rotate_degrees_array[19] = {0,40,80,120,160,200,240,280,320,360,320,280,240,200,160,120,80,40,0};
	
	cout << endl << "------target detection------" << endl;
	// 旋转一圈寻找目标
	for(int i = 0; i < 19; i++){
		cout << " car base degree----> " << rotate_degrees_array[i] << endl;
		car.move_rotate(rotate_degrees_array[i]);
		int takephoto_index = -1;
		int find_num = 0;
		for(int j = 0; j < rts.size(); j++){
			if(rts[j]->isfind()) find_num++;
		}
		for(int j = 0; j < rts.size(); j++){
			
			if(!rts[j]->isfind()){
				if(takephoto_index != -1){
					rts[j]->findTarget(rts[takephoto_index]->pic_rgb, false, "0");
				}else{
					takephoto_index = j;
					rts[j]->findTarget(false, "0");
				}
				if(rts[j]->isfind()){
					rts[j]->base_degree = rotate_degrees_array[i];
					find_num++;
					cout << "      find target: " << rts[j]->id << endl;
				}
			}
		}
		//if(find_num == rts.size()) break;
		usleep(1000*500);
	}
	//car.move_rotate(rotate_degrees_array[0]);
	//usleep(1000*1000);
	for(int i = 0; i < rts.size(); i++) rts[i]->show();
	
	cout << endl << "------upload data------" << endl;
	string send = "devId=";
	send = send + devId + "&targetdata=";
	string targetdata = "[";
	for(int i = 0; i < rts.size(); i++){
		string dd = "{aimId=\'" + rts[i]->id + "\',distance=\'" + to_string(rts[i]->center_distance) +"},";
		targetdata = targetdata + dd;
	}
	send = send + targetdata +"]";
	cout << send << endl; 
	auto res = cli.post("/uploadinfor/", send.c_str(), "application/x-www-form-urlencoded");
	if (res && res->status == 200) {
		string body = res->body;
		cout << body << endl;
	}else{
		cout << endl << "------network failed------" << endl;
		exit(-1);
	}
	
	cout << endl << "------wait aim id------" << endl;
	int aim_index = -1;
	// 50s
	for(i = 0; i < 100; i++){
		if(i%2 == 0) cout << " " << i/2 + 1 << "s pass..." << endl;
		string send = "devId=";
		send = send + devId;
		auto res = cli.post("/getaimid/", send.c_str(), "application/x-www-form-urlencoded");
		if (res && res->status == 200) {
			string body = res->body;
			if(body != "-1"){
				for(int j = 0; j < rts.size(); j++){
					if(rts[j]->id == body){
						aim_index = j;
						break;
					}
				}
				break;
			}else usleep(1000*500);
		}else{
			cout << endl << "------network failed------" << endl;
			exit(-1);
		}
	}
	if(i == 100){
		cout << endl << "------wait aim timeout------" << endl;
		exit(-1);
	}
	
	cout << endl << "------move to aim------" << endl;
	cout << " aim id: " << rts[aim_index]->id << endl;
	double move_degree = rts[aim_index]->base_degree + rts[aim_index]->degree;
	if( move_degree > 180) move_degree = move_degree - 360;
	cout << " move degree: " << move_degree << endl;
	car.move_rotate(move_degree);
	usleep(1000 * 500);
	rts[aim_index]->findTarget(false, "0");
	rts[aim_index]->show();
	
	cout<< endl << "------system end success------" << endl;
	return   0;
}