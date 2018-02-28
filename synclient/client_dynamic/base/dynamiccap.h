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
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	rgb = NULL;
	width = width_temp;
	height = height_temp;
}

Cam::~Cam(){
	if(rgb != NULL) free(rgb);
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
