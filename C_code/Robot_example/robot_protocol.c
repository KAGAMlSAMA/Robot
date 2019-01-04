#include <stdio.h>
#include <string.h>
#include "robot_protocol.h"
#include "uart_api.h"

void DelayLoop(int delay_time)
{
	while(delay_time)
		delay_time--;
}

void Send_Command(unsigned char Ldata, unsigned char Ldata1)
{
	unsigned char Command_Buffer[6] = {0,};

	Command_Buffer[0] = START_CODE;	// Start Byte -> 0xff
	Command_Buffer[1] = START_CODE1; // Start Byte1 -> 0x55
        Command_Buffer[2] = Ldata;
	Command_Buffer[3] = Ldata1;
	Command_Buffer[4] = Hdata;  // 0x00
	Command_Buffer[5] = Hdata1; // 0xff

	uart1_buffer_write(Command_Buffer, 6);
}

#define ERROR	0
#define OK	1
void SendMotion(motion_index)
{
	//write your code here
	//将motion_index转化成16进制数用下面的Send_Command送出去即可
	//现在默认的Send_Command函数（见上面定义）只设置了低位8bit的数据，如果需要使用高位可以自行修改上面Send_Command函数，把Hdata作为输入函数加上
	//第二个参数其实就是低位数据的反。可以不输入，通过代码来自动转换
	Send_Command(0x01, 0xfe);

}


