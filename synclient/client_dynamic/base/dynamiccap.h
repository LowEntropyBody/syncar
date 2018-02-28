#ifndef __DYNAMICCAP_H
#define __DYNAMICCAP_H
//////////////////////////////////////
#include "cap.h"
#include "dealimg.h"
#include <math.h>
#include <iostream>
#include <vector>
#include <string>
#include <thread>
using namespace std;


class Cam{
	camera_t* camera;
	struct timeval timeout;
	unsigned char* rgb;
	public:
		uint32_t width;
		uint32_t height;
		Cam(const char * device, uint32_t width, uint32_t height);
		~Cam();
		unsigned char* take_pic();
		bool save_pic(const char * name);
};

bool Cam::save_pic(const char * name){
	if(rgb == NULL) return false;
	FILE* out = fopen(name, "w");
	jpeg(out, rgb, width, height, 100);
	fclose(out);
	return true;
}

unsigned char* Cam::take_pic(){
	if(rgb != NULL) {
		free(rgb);
		rgb = NULL;
	}
	//拍摄照片
	camera_start(camera);
	camera_frame(camera, timeout);
	rgb = yuyv2rgb(camera->head.start, camera->width, camera->height);
	camera_stop(camera);
	return rgb;
}

Cam::Cam(const char * device, uint32_t width_temp, uint32_t height_temp){
	camera = camera_open(device, width_temp, height_temp);
	camera_init(camera);
	camera_start(camera);
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	// 跳过前面的6帧图像 
	for (int i = 0; i < 6; i++) {
		camera_frame(camera, timeout);
	}
	rgb = NULL;
	width = width_temp;
	height = height_temp;
	camera_stop(camera);
}

Cam::~Cam(){
	if(rgb != NULL) free(rgb);
	camera_finish(camera);
	camera_close(camera);
}



typedef struct {
  bool isfind;
  double distance;
  double degree;
  double base_degree;
  double center_distance;
  long long index;
} MoveInfo;

class ThreadCam{
	private:
		Cam* cam;
		string cam_name;
		thread* t;
		bool run_flag;
		void run();
		MoveInfo* move_info;
		MoveInfo* move_info_out;
		mutex mx;
		DealImg* deal_img;
	public:
		ThreadCam(const char * device, uint32_t width, uint32_t height);
		~ThreadCam();
		void thread_run();
		void thread_stop();
		bool isfind();
		MoveInfo* get_move_info();
		void show();
		
};
void ThreadCam::show(){
	deal_img -> show();
}
MoveInfo* get_move_info(){
	lock_guard<mutex> guard(mx);
	if(move_info -> isfind){
		move_info -> isfind = false;
		move_info_out -> isfind = true;
		move_info_out -> distance = move_info -> distance;
		move_info_out -> degree = move_info -> degree;
		move_info_out -> center_distance = move_info -> center_distance;
		move_info_out -> index = move_info -> index;
		return move_info_out;
	}
	return NULL;
}

void ThreadCam::run(){
	long long index = 0;
	while(run_flag){
		index ++;
		if(deal_img -> find_aim(cam -> take_pic(), cam -> width, cam -> height) -> isfind){
			lock_guard<mutex> guard(mx);
			move_info -> isfind = true;
			move_info -> distance = deal_img -> distance;
			move_info -> degree = deal_img -> degree;
			move_info -> center_distance = deal_img -> center_distance;
			move_info -> index = index;
		}else{
			lock_guard<mutex> guard(mx);
			move_info -> isfind = false;
		}
		//deal_img -> show();
		this_thread::sleep_for(chrono::milliseconds(500));  // 2 休眠500ms
	}	
}
void ThreadCam::thread_run() {
   t = new thread(&ThreadCam::run, this);
}
void ThreadCam::thread_stop(){
	run_flag = false;
	t->join();
}
ThreadCam::ThreadCam(const char * device, uint32_t width, uint32_t height){
	t = NULL;
	run_flag = true;
	cam = new Cam(device, width, height);
	cam_name = device;
	deal_img = new DealImg();
	move_info = (MoveInfo*)malloc(sizeof (MoveInfo));
	move_info_out = (MoveInfo*)malloc(sizeof (MoveInfo));
	move_info -> isfind = false;
	move_info -> distance = 0;
	move_info -> degree = 0;
	move_info -> base_degree = 0;
	move_info -> center_distance = 0;
	move_info -> index = 0;
}
ThreadCam::~ThreadCam(){
	free(move_info);
	free(move_info_out);
}

#endif
