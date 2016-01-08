#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>

#include <iostream>
#include <cmath>

// unsigned char : 1 Byte
// short : 2 Byte

// とりあえず1個だけ動かしてみよう
int send_to_b3m (int fd,  unsigned char id , short pos, unsigned char option)
{
  unsigned char data[8];
  short sum = 0;

  data[0]  = (unsigned char)sizeof(data); // SIZE
  data[1]  = (unsigned char)0x06;  // コマンド
  data[2]  = (unsigned char)0; // OPTION
  data[3]  = (unsigned char)id; //id
  data[4]  = (unsigned char)(pos&0x00FF); // POS_L
  data[5]  = (unsigned char)((pos&0xFF00)>>8); // POS_H
  data[6]  = (unsigned char)0x00; // TIME_L
  data[7]  = (unsigned char)0x00; // TIME_H

  // チェックサム
  for(int i = 0; i < 8; ++i ){
	  sum += data[i];
  }
  data[8] = (unsigned char)(sum&0x00FF); // SIZE~TIMEまでの総和の下位1Byte

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

	send_to_b3m (servo_fd,  0, 10000, 0);

	return 0;
}
