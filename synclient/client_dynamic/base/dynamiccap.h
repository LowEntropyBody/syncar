#ifndef __DYNAMICCAP_H
#define __DYNAMICCAP_H
//////////////////////////////////////
#include "cap.h"
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

class ThreadCam{
	private:
		Cam* cam;
		string cam_name;
		thread* t;
		bool run_flag;
		void run();
	public:
		ThreadCam(const char * device, uint32_t width, uint32_t height);
		~ThreadCam();
		void thread_run();
		void thread_stop();
};
void ThreadCam::run(){
	while(run_flag){
		cout<< "in: " << cam_name << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));  // 2 休眠100ms
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
	cam_name = device
}


#endif
