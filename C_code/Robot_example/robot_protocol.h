/* ------------------------------------------------------------------------ */
/*                  KAIST BATTLE ROBOT CONTROL PROGRAM						*/
/*                   														*/
/*          Coding by YKH      												*/
/*          file : robot_protocol.h    										*/
/* ------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------ */
/*                  KAIST BATTLE ROBOT CONTROL PROGRAM            			*/
/*                   														*/
/*          Modified by Goon-Ho Choi, HANOOL ROBOTICS 2003.07.16     		*/
/* ------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------ */
#ifndef __ROBOT_PROTOCOL_H__
#define __ROBOT_PROTOCOL_H__
#define START_CODE    0xFF
#define START_CODE1   0x55
#define Hdata	      0x00
#define Hdata1        0xFF

////////////////////////////////////////////////////////////////////////////////
void DelayLoop(int delay_time);
void Send_Command(unsigned char Ldata, unsigned char Ldata1);
unsigned char Receive_Ack(void);
void init(void);
void InitSlow(void);
////////////////////////////////////////////////////////////////////////////////

#endif // __ROBOT_PROTOCOL_H__


