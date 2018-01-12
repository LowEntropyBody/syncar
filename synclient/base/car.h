#ifndef __CAR_H
#define __CAR_H
//////////////////////////////////////
#include "usart.h"

class Car{
	int fd;
	public:
		Car();
		~Car();
		void move_frist_start();
		void move_rotate(int degree);
		
};

void Car::move_rotate(int degree){
	degree = degree * 21;
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
	tcflush(usart_fd, TCIFLUSH);//清空in缓冲区
    tcflush(usart_fd, TCOFLUSH);//清空out缓冲区
}

void Car::move_frist_start(){
	char ch[1] = {0x00};
	int re = usart_init();
	if(re < 0) exit(-1);
	write(usart_fd, ch, sizeof(ch));
	tcflush(usart_fd, TCIFLUSH);//清空in缓冲区
    tcflush(usart_fd, TCOFLUSH);//清空out缓冲区
}

Car::Car(){
}
Car::~Car(){
	usart_close();
}

#endif