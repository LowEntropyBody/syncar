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
		uint32_t height
		Cam(const char * device, uint32_t width, uint32_t height);
		~Cam();
		unsigned char* getrgb();
};

unsigned char* Cam::getrgb(){
	if(rgb != NULL) {
		free(rgb);
		rgb = NULL;
	}
	//拍摄照片
	camera_frame(camera, timeout);
	rgb = yuyv2rgb(camera->head.start, camera->width, camera->height);
	return rgb;
}

Cam::Cam(const char * device, uint32_t width_temp, uint32_t height_temp){
	camera = camera_open(device, width_temp, height_temp);
	camera_init(camera);
	camera_start(camera);
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	// 跳过前面的5帧图像 
	for (int i = 0; i < 5; i++) {
		camera_frame(camera, timeout);
	}
	rgb = NULL;
	width = width_temp;
	height = height_temp;
}

Cam::~Cam(){
	if(rgb != NULL) free(rgb);
	camera_stop(camera);
	camera_finish(camera);
	camera_close(camera);
}

/**
class DynamicCap{
	vector<camera_t*> cameras;
	public:
		DynamicCapture();
		~DynamicCapture();
};**/


#endif
