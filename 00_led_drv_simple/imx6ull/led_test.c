#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// Usage: ledtest /dev/myled <on|off>

int main(int argc, char **argv)
{
	int fd;
	char status = 0;
	
	if(argc != 3)
	{
		printf("Usage: %s /dev/myled [on/off]\n", argv[0]);
		return -1;
	}
	//open
	fd = open(argv[1], O_RDWR);
	if(fd < 0)
	{
		printf("can not open %s\n", argv[1]);
		return -1;
	}
	
	//write
	if(strcmp(argv[2], "on") == 0)
	{
		status = 1;
	}
	
	write(fd, &status, 1);
	
	return 0;
}
