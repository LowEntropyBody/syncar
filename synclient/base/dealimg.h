#ifndef __DEALIMG_H
#define __DEALIMG_H
//////////////////////////////////////
#include "cap.h"
#include <math.h>
//处理图片返回的结果
typedef struct {
  int isfind;
  int center_x;
  int center_y;
  int l;
  int w;
  int area;
} aim_infor;

/*
	O------------------------>height=640  y j
	|
	|
	|
	|
	|
	|
	|
	|
	width=360 x i
*/
typedef struct {
  int area;
  int left_x;
  int left_y;
  int right_x;
  int right_y;
  int top_x;
  int top_y;
  int bottom_x;
  int bottom_y;
} area_infor;


//处理图片返回目标信息
aim_infor* find_aim(unsigned char* rgb,int height,int width,int(*color_match)(int,int,int), int width_tag, int height_tag);

//颜色是否匹配->红
int color_match_red(int r,int g,int b);

//颜色是否匹配->绿
int color_match_green(int r,int g,int b);

//颜色是否匹配->蓝
int color_match_blue(int r,int g,int b);

//判断连通图是否是8.5*21 矩形
int judge_rect(area_infor* ar, double width, double height);

/*递归计算连通图大小，上下左右边界点并返回
**flags为呆测矩阵,二维用一维形式表示flags[(i * width + j)]，0表示没有走过，1表示走过
*/
area_infor* deal_area(unsigned char* flags ,int height,int width,int start_x,int start_y);

//两个area_infor比较参数，并将需要的保存在第一个里
void compare_two_ainfor(area_infor* a,area_infor* b);


//////////////////////////////////////

//处理图片返回目标信息
/*
	O------------------------>height=640  y j
	|
	|
	|
	|
	|
	|
	|
	|
	width=360 x i
*/

//处理图片返回目标信息
aim_infor* find_aim(unsigned char* rgb,int height,int width,int(*color_match)(int,int,int), int width_tag, int height_tag){
	//用于计算连通图
	unsigned char* flags = (unsigned char*)calloc(width * height, sizeof (unsigned char)); 
	//返回结果
	aim_infor* infor = (aim_infor*)malloc(sizeof (aim_infor));
	infor -> isfind = 0;
	infor -> center_x = 0;
	infor -> center_y = 0;
	infor -> l = 0;
	infor -> w = 0;
	infor -> area = 0;
	
	//颜色过滤
	for (size_t i = 0; i < width; i++) {
		for (size_t j = 0; j < height; j++) {
			if((*color_match)(rgb[(i * height + j) * 3 + 0],
				rgb[(i * height + j) * 3 + 1],rgb[(i * height + j) * 3 + 2])){
				flags[i * height + j] = 0;
			}else{
				flags[i * height + j] = 1;
				
				//rgb[(i * height + j) * 3 + 0] = 0;
				//rgb[(i * height + j) * 3 + 1] = 0;
				//rgb[(i * height + j) * 3 + 2] = 0;
				
			}
		}
	}
	
	
	//形状过滤
	for (size_t i = 0; i < width; i++) {
		for (size_t j = 0; j < height; j++) {
			if(flags[i * height + j] == 0){
				area_infor* temp_area = deal_area(flags,height,width,i,j);
				if(judge_rect(temp_area, width_tag, height_tag)){
					printf("(%d,%d)\n",i,j);
					printf("area:%d\n",temp_area->area);
					printf("left_x:%d\n",temp_area->left_x);
					printf("left_y:%d\n",temp_area->left_y);
					printf("right_x:%d\n",temp_area->right_x);
					printf("right_y:%d\n",temp_area->right_y);
					printf("top_x:%d\n",temp_area->top_x);
					printf("top_y:%d\n",temp_area->top_y);
					printf("bottom_x:%d\n",temp_area->bottom_x);
					printf("bottom_y:%d\n",temp_area->bottom_y);
					
					printf("面积 S = %d,中心 O = (%d,%d),长 l = %d,宽 w = %d\n",temp_area->area,
					(temp_area -> bottom_x + temp_area -> top_x)/2,
					(temp_area -> right_y + temp_area -> left_y)/2,
					(temp_area -> bottom_x - temp_area -> top_x),
					(temp_area -> right_y - temp_area -> left_y));
					//保存结果
					infor -> isfind = 1;
					if(infor -> area < temp_area -> area){
						infor -> area = temp_area -> area;
						infor -> center_x = (temp_area -> bottom_x + temp_area -> top_x)/2;
						infor -> center_y = (temp_area -> right_y + temp_area -> left_y)/2;
						infor -> l = (temp_area -> bottom_x - temp_area -> top_x);
						infor -> w = (temp_area -> right_y - temp_area -> left_y);
					}
					free(temp_area);
				}
			}
		}
	}
	free(flags);
	return infor;
}

//判断连通图是否是矩形
int judge_rect(area_infor* ar, double width, double height){
	//太小不算目标
	if(ar -> area < 200)
		return 0;
	
	//左右直径
	double left_right_l = (ar -> right_y - ar -> left_y);
	//上下直径
	double top_bottom_l = (ar -> bottom_x - ar -> top_x);

	if(width == 8.2){
	//上下和左右的直径符合要求比例
		if(top_bottom_l/left_right_l >= 3.06||top_bottom_l/left_right_l <= 2.06){
			return 0;
		}
	}
	if(width == 5.6){
		if(top_bottom_l/left_right_l >= 4.25||top_bottom_l/left_right_l <= 3.25){
			return 0;
		}
		printf("55555555555555\n")
	}
	//面积要吻合
	if(((left_right_l * top_bottom_l)/((double)ar -> area)) > 1.2 || ((left_right_l * top_bottom_l)/((double)ar -> area)) < 0.8){
		return 0;
	}
	return 1;
}

//颜色是否匹配->红
int color_match_red(int r,int g,int b){
	if((double)r >= 70 && (double)r <= 120){
		if(g>=10&&g<=40&&b<=40) return 1;
	}
	if((double)r >= 120){
		if(1.1 * (double)r - (double)g >= 80&&
		   1.1 * (double)r - (double)g <= 160&&
	       1.2 * (double)r - (double)b >= 100&&
	       1.2 * (double)r - (double)b <= 200) return 1;
	}
	return 0;
}

//颜色是否匹配->绿
int color_match_green(int r,int g,int b){
	if((double)g/(double)b>1&&(double)g/(double)r>1){
		double k = (double)r + (double)b;
		if((double)k/(double)g<1.7&&(double)k/(double)g>1.15){
			if((double)r/(double)b<2.5&&(double)r/(double)b>0.8){
				return 1;
			}
		}
	}
	return 0;
}

//颜色是否匹配->蓝
int color_match_blue(int r,int g,int b){
	return 0;
}



/*递归计算连通图大小，上下左右边界点并返回
**flags为呆测矩阵,二维用一维形式表示flags[(i * height + j)]，0表示没有走过，1表示走过
*/
area_infor* deal_area(unsigned char* flags,int height,int width,int start_x,int start_y){
	area_infor* ainfor = (area_infor*)malloc(sizeof (area_infor));
	area_infor* temp = NULL;
	//初始化
	ainfor -> area = 0;
	//如果已经走过
	if(flags[start_x * height + start_y]){
		return ainfor;
	}else{
		ainfor -> area = 1;
		ainfor -> left_x = start_x;
		ainfor -> left_y = start_y;
		ainfor -> right_x = start_x;
		ainfor -> right_y = start_y;
		ainfor -> top_x = start_x;
		ainfor -> top_y = start_y;
		ainfor -> bottom_x = start_x;
		ainfor -> bottom_y = start_y;
		flags[start_x * height + start_y] = 1;
	}
	//上走
	if(start_x != 0 && flags[(start_x - 1) * height + start_y] == 0){
		temp = deal_area(flags,height,width,start_x - 1,start_y);
		ainfor -> area = ainfor -> area + temp -> area;
		compare_two_ainfor(ainfor,temp);
	}
	//下走
	if(start_x != width - 1 && flags[(start_x + 1) * height + start_y] == 0){
		temp = deal_area(flags,height,width,start_x + 1,start_y);
		ainfor -> area = ainfor -> area + temp -> area;
		compare_two_ainfor(ainfor,temp);
	}
	//左走
	if(start_y != 0 && flags[start_x * height + start_y - 1] == 0){
		temp = deal_area(flags,height,width,start_x,start_y - 1);
		ainfor -> area = ainfor -> area + temp -> area;
		compare_two_ainfor(ainfor,temp);
	}
	//右走
	if(start_y != height - 1 && flags[start_x * height + start_y + 1] == 0){
		temp = deal_area(flags,height,width,start_x,start_y + 1);
		ainfor -> area = ainfor -> area + temp -> area;
		compare_two_ainfor(ainfor,temp);
	}
	return ainfor;
}

//两个area_infor比较参数，并将需要的保存在第一个里
void compare_two_ainfor(area_infor* a,area_infor* b){
	if(b -> left_y < a -> left_y){
		a -> left_x = b -> left_x;
		a -> left_y = b -> left_y;
	}
	if(b -> right_y > a -> right_y){
		a -> right_x = b -> right_x;
		a -> right_y = b -> right_y;
	}
	if(b -> top_x < a -> top_x){
		a -> top_x = b -> top_x;
		a -> top_y = b -> top_y;
	}
	if(b -> bottom_x > a -> bottom_x){
		a -> bottom_x = b -> bottom_x;
		a -> bottom_y = b -> bottom_y;
	}
}


#endif
