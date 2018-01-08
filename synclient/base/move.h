#ifndef __MOVE_H
#define __MOVE_H
//////////////////////////////////////
#include "usart.h"

/*����ָ��*/
int move_start();

/*����ָ��*/
void move_end();

/*ԭ����ת
21 = 1��
1260==60��
2520==120��
7590==360��
*/
void move_rotate(int degree);

/*��ǰֱ��
*/
void move_forward(int degree,int distance);

//////////////////////////////////////

/*��ǰֱ��
*/
void move_forward(int degree,int distance){
	int high,low;
	char ch[10] = {0xff,0xfe,2,0,0,20,0,20,0,0x07};
	
	if(degree >= 0){
		ch[9] = 0x07;
		//A�ֲ���
		low = degree%256;
		high = degree/256;
		ch[3] = high;
		ch[4] = low;
		//C����ת
		int distancec = degree + distance;
		low = distancec%256;
		high = distancec/256;
		ch[7] = high;
		ch[8] = low;
		//B�ַ�ת
		int distanceb = degree - distance;
		if(distanceb >= 0){
			low = distanceb%256;
			high = distanceb/256;
			ch[5] = high;
			ch[6] = low;
		}else{
			distanceb = -distanceb;
			low = distanceb%256;
			high = distanceb/256;
			ch[5] = high;
			ch[6] = low;
			ch[9] = 0x05;
		}
	}else{
		degree = -degree;
		ch[9] = 0x00;
		//A�ֲ���
		low = degree%256;
		high = degree/256;
		ch[3] = high;
		ch[4] = low;
		
		//B�ַ�ת
		int distanceb = degree + distance;
		low = distanceb%256;
		high = distanceb/256;
		ch[5] = high;
		ch[6] = low;
		
		//C����ת
		int distancec = degree - distance;
		if(distancec >= 0){
			low = distancec%256;
			high = distancec/256;
			ch[7] = high;
			ch[8] = low;
		}else{
			distancec = -distancec;
			low = distancec%256;
			high = distancec/256;
			ch[7] = high;
			ch[8] = low;
			ch[9] = 0x01;
		}
		
	}
	
	write(usart_fd, ch, sizeof(ch));
	tcflush(usart_fd, TCIFLUSH);//���in������
    tcflush(usart_fd, TCOFLUSH);//���out������
	
}


/*����ָ��*/
void move_end(){
	usart_close();
}
int move_start(){
	char ch[1] = {0x00};
	int re = usart_init();
	if(re < 0)
		return -1;
	write(usart_fd, ch, sizeof(ch));
	tcflush(usart_fd, TCIFLUSH);//���in������
    tcflush(usart_fd, TCOFLUSH);//���out������
	return 0;
}

/*ԭ����ת*/
void move_rotate(int degree){
	int high,low;
	char ch[10] = {0xff,0xfe,2,20,0,20,0,20,0,0x07};
	if(degree>=0){
		low = degree%256;
		high = degree/256;
		ch[3] = ch[5] = ch[7] = high;
		ch[4] = ch[6] = ch[8] = low;
	}else{
		degree = -degree;
		low = degree%256;
		high = degree/256;
		ch[3] = ch[5] = ch[7] = high;
		ch[4] = ch[6] = ch[8] = low;
		ch[9] = 0x00;
	}
	write(usart_fd, ch, sizeof(ch));
	tcflush(usart_fd, TCIFLUSH);//���in������
    tcflush(usart_fd, TCOFLUSH);//���out������
}
#endif