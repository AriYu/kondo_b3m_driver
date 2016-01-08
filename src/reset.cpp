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

int reset (int fd,  unsigned char id)
{
  unsigned char data[6];
  short sum = 0;

  data[0]  = (unsigned char)sizeof(data); // SIZE
  data[1]  = (unsigned char)0x05;  // コマンド
  data[2]  = (unsigned char)0b01000000; // OPTION : CLEAR
  data[3]  = (unsigned char)id; //id
  data[4]  = (unsigned char)0x03; // TIME

  // チェックサム
  for(int i = 0; i < 5; ++i ){
	  sum += data[i];
  }
  data[5] = (unsigned char)(sum&0x00FF); // SIZE~TIMEまでの総和の下位1Byte

  for(int i = 0; i < 6; ++i)
	{
	  printf("data[%d]:%x\n",i, data[i]);
	}

  // 送信
  return write(fd, data, 6);
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

	reset(servo_fd,  0);

	return 0;
}
