#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>

#include "uart_api.h"
#include "robot_protocol.h"

#define WalkReady 1


int GetVision(void)
{
	//write your code here
	//进行图像处理，并根据分类结果送出决定的动作
	//需要分几类请自己思考~
	//关于如何读取FPGA图像和处理图像，请参考graphic_test代码
	
}


int Mainprogram(void)
{
   int Dicision;
   //Send_Motion函数在robot_protocol.c里。请自己完善
   //WalkReady其实只是一个数字，在第11行定义的，方便记忆
 	Send_Motion(WalkReady);
	while(1) {
 		
 		Dicision = GetVision();
		//这里请根据GetVision返回的结果做出相应动作。
		switch(Dicision) {
			case 'xx' : Send_Motion(xxx);				break;
			case 'xx' : Send_Motion(xxx);				break;
			default : break;
		}
	} 
	return 0;
}

#include <termios.h>
static struct termios inittio, newtio;

void init_console(void)
{
    tcgetattr(0, &inittio);
    newtio = inittio;
    newtio.c_lflag &= ~ICANON;
    newtio.c_lflag &= ~ECHO;
    newtio.c_lflag &= ~ISIG;
    newtio.c_cc[VMIN] = 1;
    newtio.c_cc[VTIME] = 0;
    cfsetispeed(&newtio, B115200);

    tcsetattr(0, TCSANOW, &newtio);
}

int main(void)
{
	int ret;

	init_console();

	ret = uart_open();
	if (ret < 0) return EXIT_FAILURE;

	uart_config(UART1, 57600, 8, UART_PARNONE, 1);
//////////////////////////////////////////////////////////////////
//这里的上下是UART的初始化，可以不管
	Mainprogram();
//////////////////////////////////////////////////////////////////
	uart_close();

	return 0;
}


