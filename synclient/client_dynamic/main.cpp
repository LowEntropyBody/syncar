/*
**  Author: ZhaoYang
**	Compile: g++ synclient/client_dynamic/main.cpp -ljpeg -lm -fpermissive -o dynamic.out
**  Run: ./dynamic.out re1.jpg
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
	
	return 0;
}