#ifndef __DYNAMICCAP_H
#define __DYNAMICCAP_H
//////////////////////////////////////
#include "cap.h"
#include <math.h>
#include <iostream>
#include <vector>
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
		unsigned char* takePic();
		bool savePic(const char * name);
};

bool Cam::savePic(const char * name){
	if(rgb == NULL) return false;
	FILE* out = fopen(name, "w");
	jpeg(out, rgb, width, height, 100);
	fclose(out);
	return true;
}

unsigned char* Cam::takePic(){
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

class DynamicCap{
	vector<Cam*> cameras;
	public:
		DynamicCap();
		~DynamicCap();
};

DynamicCap::DynamicCap(){
	cameras.push_back(new Cam("/dev/video0", 640, 360));
	cameras.push_back(new Cam("/dev/video1", 640, 360));
	cameras.push_back(new Cam("/dev/video2", 640, 360));
	usleep(1000*500);
	cameras[0]->takePic();
	//cameras[0]->savePic("re1.jpg");
	cameras[1]->takePic();
	//cameras[1]->savePic("re2.jpg");
	cameras[2]->takePic();
	//cameras[2]->savePic("re3.jpg");
	
}

#endif
