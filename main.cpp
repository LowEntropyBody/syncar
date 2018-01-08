/*
**  Author: ZhaoYang
**	Compile: g++ main.cpp -ljpeg -lm -fpermissive -o syncar.out
**  Lib: sudo apt-get install libv4l-dev
**		 sudo apt-get install libjpeg-dev
**  Date: 2018/1
*/
#include "base/httplib.h"
#include "base/move.h"
#include "base/cap.h"
#include "base/dealimg.h"
#include <math.h>
#include <iostream>
using namespace std;

int main(void){
	// ///////////////////参数初始化////////////////////////
	
	// 旋转角度0-80-160-240-320-280-200-120-40-0
	int rotate_degrees_array[10] = {0,1680,3360,5040,6720,5880,4200,2520,840,0};
	
	// 是否保存图片 0->不保存，1->保存
	int flag_save_img = 0;
	
	// 用于保存距离和角度
	int aim_distance = -1;
	int aim_degree = -1;
	
	// /////////////////////初始化//////////////////////////
	
	
	// 启动串口，开启向小车发送指令功能
	if(move_start() < 0){
		exit(-1);
	}
	
	// ////////////////////程序主过程////////////////////////
	
	for(int i = 0;i < 10;i++){
		printf("当前角度：%d\n",rotate_degrees_array[i]/420*20);
		move_rotate(rotate_degrees_array[i]);
		usleep(1000*400);
		camera_t* camera = camera_open("/dev/video0", 640,360);
		camera_init(camera);
		camera_start(camera);
		struct timeval timeout;
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		// 跳过前面的5帧图像 
		for (int i = 0; i < 5; i++) {
			camera_frame(camera, timeout);
		}
		usleep(1000*100);
		//拍摄照片
		camera_frame(camera, timeout);
		//转换为rgb
		unsigned char* rgb = yuyv2rgb(camera->head.start, camera->width, camera->height);
		//存储原图
		if(flag_save_img){
			FILE* out = fopen("result1.jpg", "w");
			jpeg(out, rgb, camera->width, camera->height, 100);
			fclose(out);
		}
		//找寻目标
		int (*color_match_t)(int,int,int) = color_match;
		int (*judge_rect_t)(area_infor*) = judge_rect;
		aim_infor* infor = find_aim(rgb, camera->width, camera->height, color_match_t, judge_rect_t);
		//存处理过的图片
		if(flag_save_img){
			FILE* out2 = fopen("result2.jpg", "w");
			jpeg(out2, rgb, camera->width, camera->height, 100);
			fclose(out2);
		}
		//如果找到
		if(infor -> isfind){
			double distance = 14850.0/(double)infor -> l;
			double degree = 0;
			if(infor -> center_y > 320){
				double k = (double)(640 - infor -> center_y)/(double)(infor -> center_y - 320);
				degree = -atan(1/((1+k)*sqrt(3)))*180.0/3.14159;
			}else if (infor -> center_y < 320){
				double k = (double)(infor -> center_y)/(double)(320 - infor -> center_y);
				degree = atan(1/((1+k)*sqrt(3)))*180.0/3.14159;
			}else{
				degree = 0;
			}
		
			printf("目标距离：%f cm\n",distance);
			printf("偏离角度：%f 度\n",degree);
			
			aim_distance = distance;
			aim_degree = rotate_degrees_array[i]/420*20 + degree;
			// 退出工作
			free(infor);
			free(rgb);
		
			// 关闭摄像机
			camera_stop(camera);
			camera_finish(camera);
			camera_close(camera);
	
			sleep(1);
			break;
		}
		free(infor);
		free(rgb);
		
		// 关闭摄像机
		camera_stop(camera);
		camera_finish(camera);
		camera_close(camera);
	
		usleep(1000*500);
	}
	// /////////////////////向目标移动////////////////////////
	
	if(aim_distance != -1){
		//先转到位置
		move_rotate(aim_degree*21);
		usleep(1000*400);
		//再次测量目标
		camera_t* camera = camera_open("/dev/video0", 640,360);
		camera_init(camera);
		camera_start(camera);
		struct timeval timeout;
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		// 跳过前面的5帧图像 
		for (int i = 0; i < 5; i++) {
			camera_frame(camera, timeout);
		}
		usleep(1000*100);
		//拍摄照片
		camera_frame(camera, timeout);
		//转换为rgb
		unsigned char* rgb = yuyv2rgb(camera->head.start, camera->width, camera->height);
		//存储原图
		if(flag_save_img){
			FILE* out = fopen("result1.jpg", "w");
			jpeg(out, rgb, camera->width, camera->height, 100);
			fclose(out);
		}
		//找寻目标
		int (*color_match_t)(int,int,int) = color_match;
		int (*judge_rect_t)(area_infor*) = judge_rect;
		aim_infor* infor = find_aim(rgb, camera->width, camera->height, color_match_t, judge_rect_t);
		//存处理过的图片
		if(flag_save_img){
			FILE* out2 = fopen("result2.jpg", "w");
			jpeg(out2, rgb, camera->width, camera->height, 100);
			fclose(out2);
		}
		//如果找到
		if(infor -> isfind){
			double distance = 14850.0/(double)infor -> l;
			double degree = 0;
			if(infor -> center_y > 320){
				double k = (double)(640 - infor -> center_y)/(double)(infor -> center_y - 320);
				degree = -atan(1/((1+k)*sqrt(3)))*180.0/3.14159;
			}else if (infor -> center_y < 320){
				double k = (double)(infor -> center_y)/(double)(320 - infor -> center_y);
				degree = atan(1/((1+k)*sqrt(3)))*180.0/3.14159;
			}else{
				degree = 0;
			}
		
			printf("目标距离：%f cm\n",distance);
			printf("偏离角度：%f 度\n",degree);
			aim_distance = distance;
			aim_degree = aim_degree + degree + 1;
		}
		free(infor);
		free(rgb);
		
		// 关闭摄像机
		camera_stop(camera);
		camera_finish(camera);
		camera_close(camera);
		
		//再次转到目标位置
		move_rotate(aim_degree*21);
		sleep(1);
		//向目标移动
		move_forward(aim_degree*21,aim_distance*64);
	}
	
	
	// 关闭串口
	move_end();
	
	// /////////////////////////////////////////////////////
	
	return 0;
}

/**
int main(void){
	httplib::Client cli("166.111.66.49", 8001);
	
	auto res = cli.post("/uploadinfor/", "name=john1&note=coder", "application/x-www-form-urlencoded");
    if (res && res->status == 200) {
		cout << "success" << endl;
        cout << res->body << endl;
    }else{
		cout << "failed" << endl;
	}
}
**/