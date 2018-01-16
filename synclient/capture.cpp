#include "base/cap.h"
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
	if(argc > 1){
		FILE* out = fopen("result1.jpg", "w");
		jpeg(out, rgb, camera->width, camera->height, 100);
		fclose(out);
	}else{
		FILE* out = fopen(argv[1], "w");
		jpeg(out, rgb, camera->width, camera->height, 100);
		fclose(out);
	}
	free(rgb);
	camera_stop(camera);
	camera_finish(camera);
	camera_close(camera);
	
	return 0;
}