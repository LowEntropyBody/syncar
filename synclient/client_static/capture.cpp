/*
**  Author: ZhaoYang
**	Compile: g++ synclient/client_static/capture.cpp -ljpeg -lm -fpermissive -o capture.out
**  Run: ./capture.out re1.jpg
**  Lib: sudo apt-get install libv4l-dev
**		 sudo apt-get install libjpeg-dev
**  Download: git clone https://github.com/LowEntropyBody/syncar.git
**  Date: 2018/1
*/
#include "base/cap.h"
#include "base/dealimg.h"
#include <math.h>
#include <iostream>
#include <vector>
using namespace std;

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
	usleep(1000*100);
	//ÅÄÉãÕÕÆ¬
	camera_frame(camera, timeout);
	unsigned char* rgb = yuyv2rgb(camera->head.start, camera->width, camera->height);

	FILE* out = fopen("result.jpg", "w");
	jpeg(out, rgb, camera->width, camera->height, 100);
	fclose(out);
	int (*cm)(int,int,int) = color_match_red;
	aim_infor* infor = find_aim2(rgb, camera->width, camera->height, cm, 3.4, 21);
	
	FILE* out2 = fopen("result2.jpg", "w");
	jpeg(out2, rgb, camera->width, camera->height, 100);
	fclose(out2);
	
	free(infor);
	free(rgb);
	camera_stop(camera);
	camera_finish(camera);
	camera_close(camera);
	
	return 0;
}