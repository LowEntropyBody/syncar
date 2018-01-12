#ifndef __CAR_H
#define __CAR_H
//////////////////////////////////////
#include "usart.h"

class Car{
	int fd;
	public:
		Car(int fd_temp);
		~Car();
		void move_frist_start();
		void move_rotate(int degree);
		
}

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
	write(fd, ch, sizeof(ch));
	tcflush(fd, TCIFLUSH);//���in������
    tcflush(fd, TCOFLUSH);//���out������
}

void Car::move_frist_start(){
	char ch[1] = {0x00};
	int re = usart_init();
	if(re < 0) exit(-1);
	write(fd, ch, sizeof(ch));
	tcflush(fd, TCIFLUSH);//���in������
    tcflush(fd, TCOFLUSH);//���out������
}

Car::Car(int fd_temp){
	fd = fd_temp;
}
Car::~Car(){
	usart_close();
}

#endif