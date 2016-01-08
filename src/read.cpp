#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>

#include <iostream>
#include <cmath>


#define BAUDRATE B115200           /* 通信速度の設定 */
// unsigned char : 1 Byte
// short : 2 Byte

int read_from_b3m (int fd,  unsigned char id)
{
  unsigned char data[7];
  short sum = 0;

  data[0]  = (unsigned char)sizeof(data); // SIZE
  data[1]  = (unsigned char)0x03;  // コマンド
  data[2]  = (unsigned char)0x00; // OPTION : CLEAR
  data[3]  = (unsigned char)id; //id
  data[4]  = (unsigned char)0xA2; // ADDRESS
  data[5]  = (unsigned char)0x04; // LENGTH

  // チェックサム
  for(int i = 0; i < 6; ++i ){
	  sum += data[i];
  }
  data[6] = (unsigned char)(sum&0x00FF); // SIZE~TIMEまでの総和の下位1Byte

  for(int i = 0; i < 7; ++i)
	{
	  printf("data[%d]:%x\n",i, data[i]);
	}

  // 送信
  int ret = write(fd, data, sizeof(data));
  printf("write ret : %d\n", ret);
  printf("返信データ\n");
  unsigned char reply[9];

  int res = 0;
  while(res != 9){
	printf("res : %d\n", res);
	res = read(fd, reply, 9);
  }
  
  for(int i = 0; i < 9; ++i)
	{
	  printf("reply[%d]:%x\n",i, reply[i]);
	}
}

int main(void)
{
	struct termios oldtio, newtio;
	
	int servo_fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY);
	if(servo_fd < 0)
	{
		std::cout << "Error : fail to open" << std::endl;
	}

	tcgetattr(servo_fd, &oldtio);         /* 現在のシリアルポートの設定を退避させる */
	bzero(&newtio, sizeof(newtio));  /* 新しいポートの設定の構造体をクリア */

	// ioctl(servo_fd, TCGETS, &oldtio);
	// newtio = oldtio;
	
	newtio.c_cc[VMIN] = 1;
	newtio.c_cc[VTIME] = 0;
	newtio.c_cflag = BAUDRATE | CS8 | CREAD | CLOCAL;
	newtio.c_iflag = IGNPAR;
	newtio.c_oflag = 0; // rawモード
	newtio.c_lflag = 0; // 非カノニカル入力

	// int ret = cfsetspeed(&newtio, B115200);
	// if(ret < 0)
	// {
	// 	std::cout << "Error in cfsetspeed" << std::endl;
	// 	close(servo_fd);
	// 	return 1;
	// }
	// ioctl(servo_fd, TCSETS, &newtio);
tcflush(servo_fd,TCIFLUSH);           /* ポートのクリア */
tcsetattr(servo_fd, TCSANOW, &newtio); /* ポートの設定を有効にする */
	

	read_from_b3m(servo_fd,  0);

	tcsetattr(servo_fd, TCSANOW, &oldtio);
	return 0;
}
