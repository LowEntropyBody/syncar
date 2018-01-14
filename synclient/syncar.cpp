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

class RectTarget {	
	private:
		int (*cm)(int,int,int);
		double width;
		double height;
		string id;
		double distance;
		double degree;
		aim_infor* infor;
		int cm_height;
		int cm_width;
	public:
		unsigned char* pic_rgb;
		double base_degree;
		RectTarget(string id_temp, int (*color_match_temp)(int,int,int), double width_temp, double height_temp);
		~RectTarget();
		bool findTarget(bool isSave, string flag);
		bool findTarget(unsigned char* rbg_temp, bool isSave, string flag);
		aim_infor* getAimInfor();
		double getDistance();
		double getDegree();
		void show();
		bool isfind();
};

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
			cout << "------network failed------" << endl;
			exit(-1);
		}
	}
	if(i == 100){
		cout << "------server command timeout------" << endl;
		exit(-1);
	}
	// start
	cout << "------system start------" << endl;
	car.move_frist_start();
	cout << " car motative system start" << endl;
	rts.push_back(new RectTarget("target1", color_match_red, 8.2, 21));
	rts.push_back(new RectTarget("target2", color_match_red, 5.6, 21));
	//rts.push_back(new RectTarget("target3", color_match_green, 8.2, 21));
	//rts.push_back(new RectTarget("target4", color_match_green, 5.6, 21));
	//rts.push_back(new RectTarget("target5", color_match_blue, 8.2, 21));
	//rts.push_back(new RectTarget("target6", color_match_blue, 5.6, 21));
	cout << " target information load success" << endl;
	// 旋转角度
	int rotate_degrees_array[9] = {0,40,80,120,160,200,240,280,320};
	
	cout << "------target detection------" << endl;
	// 旋转一圈寻找目标
	for(int i = 0; i < 10; i++){
		cout << " car base degree:" << rotate_degrees_array[i] << endl;
		car.move_rotate(rotate_degrees_array[i]);
		int takephoto_index = -1;
		int find_num = 0;
		for(int j = 0; j < rts.size(); j++){
			if(!rts[j]->isfind()){
				if(takephoto_index != -1){
					rts[j]->findTarget(rts[takephoto_index]->pic_rgb, false, "0");
				}else{
					takephoto_index = j;
					rts[j]->findTarget(false, "0");
				}
				if(rts[j]->isfind()) rts[j]->base_degree = rotate_degrees_array[i];
			}else find_num++;
		}
		if(find_num == rts.size()) break;
		usleep(1000*1000);
	}
	// 归位
	car.move_rotate(rotate_degrees_array[i]);
	for(int i = 0; i < rts.size(); i++) rts[i]->show();
		
	
	cout << "------system end------" << endl;
	return   0;
}

//构造函数
RectTarget::RectTarget(string id_temp, int (*color_match_temp)(int,int,int), double width_temp, double height_temp){
	id = id_temp;
	cm = color_match_temp;
	width = width_temp;
	height = height_temp;
	distance = -1;
	degree = -1;
	infor = NULL;
	pic_rgb = NULL;
	cm_width = 640;
	cm_height = 320;
	base_degree = 0;
}
// 析构函数
RectTarget::~RectTarget(){
	if(infor != NULL) free(infor);
	if(pic_rgb != NULL) free(pic_rgb);
}
// 找目标返回是否找到目标
bool RectTarget::findTarget(bool isSave, string flag){
	// 打开摄像机
	camera_t* camera = camera_open("/dev/video0", cm_width, cm_height);
	camera_init(camera);
	camera_start(camera);
	struct timeval timeout;
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	// 跳过前面的5帧图像 
	for (int i = 0; i < 5; i++) {
		camera_frame(camera, timeout);
	}
	// 拍摄照片
	camera_frame(camera, timeout);
	// 转换为rgb
	if(pic_rgb != NULL) free(pic_rgb);
	pic_rgb = yuyv2rgb(camera->head.start, camera->width, camera->height);
	if(isSave){
		string name = "orign_pic_";
		name = name + id + "_" + flag + ".jpg";
		FILE* out = fopen(name.c_str(), "w");
		jpeg(out, pic_rgb, camera->width, camera->height, 100);
		fclose(out);
		cout << " save orign picture into " << name << endl;
	}
	// 找目标
	if(infor != NULL) free(infor);
	infor = find_aim(pic_rgb, camera->width, camera->height, cm, width, height);
	/*
	if(isSave){
		string name = "deal_pic_";
		name = name + id + "_" + flag + ".jpg";
		FILE* out = fopen(name.c_str(), "w");
		jpeg(out, pic_rgb, camera->width, camera->height, 100);
		fclose(out);
		cout << " save orign picture into " << name << endl;
	}*/
	// 关闭摄像机
	camera_stop(camera);
	camera_finish(camera);
	camera_close(camera);
	if(infor -> isfind){
		distance = (double)(707.14 * height)/(double)infor -> l;
		degree = 0;
		if(infor -> center_y > 320){
			double k = (double)(640 - infor -> center_y)/(double)(infor -> center_y - 320);
			degree = -atan(1/((1+k)*sqrt(3)))*180.0/3.14159;
		}else if (infor -> center_y < 320){
			double k = (double)(infor -> center_y)/(double)(320 - infor -> center_y);
			degree = atan(1/((1+k)*sqrt(3)))*180.0/3.14159;
		}
		// 余弦定理算机器人中心距目标多远c^2 = a^2 + b^2 - 2abcos(<c)
		distance = sqrt(14.5 * 14.5 + distance * distance -
						2 * 14.5 * distance * cos((double)(180 - abs(degree))/180.0 * 3.1415926));
	}
	return 0;
}

// 找目标返回是否找到目标
bool RectTarget::findTarget(unsigned char* rbg_temp, bool isSave, string flag){
	if(pic_rgb != NULL) free(pic_rgb);
	pic_rgb = rbg_temp;
	if(isSave){
		string name = "orign_pic_";
		name = name + id + "_" + flag + ".jpg";
		FILE* out = fopen(name.c_str(), "w");
		jpeg(out, pic_rgb, cm_width, cm_height, 100);
		fclose(out);
		cout << " save orign picture into " << name << endl;
	}
	// 找目标
	if(infor != NULL) free(infor);
	infor = find_aim(pic_rgb, cm_width, cm_height, cm, width, height);
	if(infor -> isfind){
		// 摄像头到目标距离
		distance = (double)(707.14 * height)/(double)infor -> l;
		degree = 0;
		if(infor -> center_y > 320){
			double k = (double)(640 - infor -> center_y)/(double)(infor -> center_y - 320);
			degree = -atan(1/((1+k)*sqrt(3)))*180.0/3.1415926;
		}else if (infor -> center_y < 320){
			double k = (double)(infor -> center_y)/(double)(320 - infor -> center_y);
			degree = atan(1/((1+k)*sqrt(3)))*180.0/3.1415926;
		}
		// 余弦定理算机器人中心距目标多远c^2 = a^2 + b^2 - 2abcos(<c)
		distance = sqrt(14.5 * 14.5 + distance * distance -
						2 * 14.5 * distance * cos((double)(180 - abs(degree))/180.0 * 3.1415926));
	}
	return 0;
}
bool RectTarget::isfind(){
	if(infor == NULL) return false;
	return infor -> isfind;
}
aim_infor* RectTarget::getAimInfor(){
	return infor;
}
// 返回目标距离
double RectTarget::getDistance(){
	return distance;
}
// 返回目标偏角
double RectTarget::getDegree(){
	return degree;
}
// 输出信息
void RectTarget::show(){
	cout << endl << "//////////////////" << "id: " << id << endl;
	if(cm == color_match_red) cout << " color: red" << endl;
	if(cm == color_match_green) cout << " color: green" << endl;
	cout << " width: " << width << endl;
	cout << " height: " << height << endl;
	cout << " distance: " << distance << endl;
	cout << " degree: " << degree << endl;
	cout << " base degree: " << base_degree << endl;
	if(infor != NULL){
		if(infor->isfind){
			cout << " infor->isfind: " << "yes" << endl;
			cout << " center: (" << infor->center_x <<", "<< infor->center_y <<")"<<endl;
			cout << " infor->l: " << infor->l <<endl;
			cout << " infor->w: " << infor->w <<endl;
			cout << " infor->area: " << infor->area <<endl;
		}else cout << " can not find target" << endl;
	}else cout << " infor is NULL, use getAimInfor() to find" << endl;
}







