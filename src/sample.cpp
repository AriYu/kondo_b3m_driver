#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>

#include <iostream>
#include <cmath>

#define NORMAL_MODE 0x00
#define FREE_MODE 0x02
#define HOLD_MODE 0x03

#define POSITION_CTRL_MODE 0x00
#define SPEED_CTRL_MODE 0x01
#define TORQU_CTRL_MODE 0x02
#define FEEDFORWARD_CTRL_MODE 0x03

#define EQUAL_DIVISION 0x01

// unsigned char : 1 Byte
// short : 2 Byte

// とりあえず1個だけ動かしてみよう
int set_servo_position (int fd,  unsigned char id , short pos, short target_time, unsigned char option)
{
  unsigned char data[9];
  short sum = 0;

  data[0]  = (unsigned char)sizeof(data); // SIZE
  data[1]  = (unsigned char)0x06;  // コマンド
  data[2]  = (unsigned char)0x00; // OPTION
  data[3]  = (unsigned char)id; //id
  data[4]  = (unsigned char)(pos&0x00FF); // POS_L
  data[5]  = (unsigned char)((pos&0xFF00)>>8); // POS_H
  data[6]  = (unsigned char)(target_time&0x00FF); // TIME_L
  data[7]  = (unsigned char)((target_time&0xFF00)>>8); // TIME_H

  // チェックサム
  for(int i = 0; i < 8; ++i ){
	  sum += data[i];
  }
  data[8] = (unsigned char)(sum&0x00FF); // SIZE~TIMEまでの総和の下位1Byte

  for(int i = 0; i < 9; ++i)
	{
	  printf("data[%d]:%x\n",i, data[i]);
	}


  // 送信
  return write(fd, data, 9);
}

int set_trajectry_mode (int fd,  unsigned char id , unsigned char trajectry_mode, unsigned char option)
{
  unsigned char data[8];
  short sum = 0;

  data[0]  = (unsigned char)sizeof(data); // SIZE
  data[1]  = (unsigned char)0x04;  // コマンド
  data[2]  = (unsigned char)0x00; // OPTION
  data[3]  = (unsigned char)id; //id
  data[4]  = (unsigned char)trajectry_mode; // DATA
  data[5]  = (unsigned char)0x29; // ADDRESS
  data[6]  = (unsigned char)1; // CNT

  // チェックサム
  for(int i = 0; i < 7; ++i ){
	  sum += data[i];
  }
  data[7] = (unsigned char)(sum&0x00FF); // SIZE~TIMEまでの総和の下位1Byte

  for(int i = 0; i < 8; ++i)
	{
	  printf("data[%d]:%x\n",i, data[i]);
	}


  // 送信
  return write(fd, data, 8);
  
}


int change_servo_status(int fd,  unsigned char id , unsigned char status, unsigned char option)
{
  unsigned char data[8];
  short sum = 0;

  data[0]  = (unsigned char)sizeof(data); // SIZE
  data[1]  = (unsigned char)0x04;  // コマンド
  data[2]  = (unsigned char)0x00; // OPTION
  data[3]  = (unsigned char)id; //id
  data[4]  = (unsigned char)status; // DATA
  data[5]  = (unsigned char)0x28; // ADDRESS
  data[6]  = (unsigned char)1; // 指定するデバイスの数 CNT

  // チェックサム
  for(int i = 0; i < 7; ++i ){
	  sum += data[i];
  }
  data[7] = (unsigned char)(sum&0x00FF); // SIZE~TIMEまでの総和の下位1Byte

  for(int i = 0; i < 8; ++i)
	{
	  printf("data[%d]:%x\n",i, data[i]);
	}


  // 送信
  return write(fd, data, 8);
  
}

int change_ctrl_mode(int fd,  unsigned char id , unsigned char ctrl_mode, unsigned char option)
{
  unsigned char data[8];
  short sum = 0;

  data[0]  = (unsigned char)sizeof(data); // SIZE
  data[1]  = (unsigned char)0x04;  // コマンド
  data[2]  = (unsigned char)0x00; // OPTION
  data[3]  = (unsigned char)id; //id
  data[4]  = (unsigned char)ctrl_mode; // DATA
  data[5]  = (unsigned char)0x28; // ADDRESS
  data[6]  = (unsigned char)1; // 指定するデバイスの数 CNT

  // チェックサム
  for(int i = 0; i < 7; ++i ){
	  sum += data[i];
  }
  data[7] = (unsigned char)(sum&0x00FF); // SIZE~TIMEまでの総和の下位1Byte

  for(int i = 0; i < 8; ++i)
	{
	  printf("data[%d]:%x\n",i, data[i]);
	}


  // 送信
  return write(fd, data, 8);

}

int set_servo_gain(int fd,  unsigned char id , unsigned char gain_param, unsigned char option)
{
  unsigned char data[8];
  short sum = 0;

  data[0]  = (unsigned char)sizeof(data); // SIZE
  data[1]  = (unsigned char)0x04;  // コマンド
  data[2]  = (unsigned char)0x00; // OPTION
  data[3]  = (unsigned char)id; //id
  data[4]  = (unsigned char)gain_param; // DATA
  data[5]  = (unsigned char)0x5c; // ADDRESS
  data[6]  = (unsigned char)1; // 指定するデバイスの数 CNT

  // チェックサム
  for(int i = 0; i < 7; ++i ){
	  sum += data[i];
  }
  data[7] = (unsigned char)(sum&0x00FF); // SIZE~TIMEまでの総和の下位1Byte

  for(int i = 0; i < 8; ++i)
	{
	  printf("data[%d]:%x\n",i, data[i]);
	}


  // 送信
  return write(fd, data, 8);
}


int main(void)
{
	struct termios oldtio, newtio;
	
	int servo_fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY);
	if(servo_fd < 0)
	{
		std::cout << "Error : fail to open" << std::endl;
	}
	ioctl(servo_fd, TCGETS, &oldtio);
	newtio = oldtio;
	
	newtio.c_cc[VMIN] = 1;
	newtio.c_cc[VTIME] = 0;
	
	newtio.c_cflag = CS8 | CREAD | CLOCAL;

	newtio.c_iflag = IGNBRK | IGNPAR;
	int ret = cfsetspeed(&newtio, B115200);
	if(ret < 0)
	{
		std::cout << "Error in cfsetspeed" << std::endl;
		close(servo_fd);
		return 1;
	}
	ioctl(servo_fd, TCSETS, &newtio);


	// 1. サーボの状態をFreeモードにする
	printf("サーボの状態をFreeモードにする\n");
	change_servo_status(servo_fd,  0, FREE_MODE, 0);
	// 2. 制御モードを変更する
	printf("制御モードを変更する\n");
	change_servo_status(servo_fd,  0, FREE_MODE | POSITION_CTRL_MODE, 0);
	set_trajectry_mode(servo_fd,  0, EQUAL_DIVISION, 0);
	// 3. ゲインを設定する
	printf("ゲインを設定する\n");
	set_servo_gain(servo_fd, 0, 0x00, 0);
	// 4. サーボの状態をNormalモードにする
	printf("サーボの状態をNormalモードにする\n");
	change_servo_status(servo_fd, 0, NORMAL_MODE, 0);
	// 5. 目標値を設定する
	printf("目標値を設定する\n");
	set_servo_position (servo_fd, 0, 50*100, 3*1000, 0);	
	// 6. Freeモードに戻す
	printf("サーボの状態をFreeモードにする\n");
	change_servo_status(servo_fd,  0, FREE_MODE, 0);

	return 0;
}
