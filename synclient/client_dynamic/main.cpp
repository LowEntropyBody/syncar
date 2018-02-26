/*
**  Author: ZhaoYang
**	Compile: g++ synclient/client_dynamic/main.cpp -std=c++11 -ljpeg -lm -fpermissive -o dynamic.out
**  Run: ./dynamic.out
**  Lib: sudo apt-get install libv4l-dev
**		 sudo apt-get install libjpeg-dev
**  Download: git clone https://github.com/LowEntropyBody/syncar.git
**  Date: 2018/2
*/
#include "base/cap.h"
#include <math.h>
#include <iostream>
#include <vector>

int main(int argc, char* argv[]){
	camera_t* camera = camera_open("/dev/video0", 640,360);
	camera_init(camera);
	camera_start(camera);
	struct timeval timeout;
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	// Ìø¹ýÇ°ÃæµÄ5Ö¡Í¼Ïñ 
	for (int i = 0; i < 5; i++) {
		camera_frame(camera, timeout);
	}
	//ÅÄÉãÕÕÆ¬
	camera_frame(camera, timeout);
	unsigned char* rgb = yuyv2rgb(camera->head.start, camera->width, camera->height);

	FILE* out = fopen("result.jpg", "w");
	jpeg(out, rgb, camera->width, camera->height, 100);
	
	fclose(out);
	free(rgb);
	camera_stop(camera);
	camera_finish(camera);
	camera_close(camera);
	
	
	camera_t* camera1 = camera_open("/dev/video1", 640,360);
	camera_init(camera1);
	camera_start(camera1);
	struct timeval timeout1;
	timeout1.tv_sec = 1;
	timeout1.tv_usec = 0;
	// Ìø¹ýÇ°ÃæµÄ5Ö¡Í¼Ïñ 
	for (int i = 0; i < 5; i++) {
		camera_frame(camera1, timeout1);
	}
	//ÅÄÉãÕÕÆ¬
	camera_frame(camera1, timeout1);
	unsigned char* rgb1 = yuyv2rgb(camera1->head.start, camera1->width, camera1->height);

	FILE* out1 = fopen("result1.jpg", "w");
	jpeg(out1, rgb1, camera1->width, camera1->height, 100);
	
	fclose(out1);
	free(rgb1);
	camera_stop(camera1);
	camera_finish(camera1);
	camera_close(camera1);
	
	
	
	return 0;
}