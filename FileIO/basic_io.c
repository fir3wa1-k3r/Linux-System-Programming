/* A simple C program which leverages open(), read(), write(), close() and lseek() to perform File operations.
This program need a text file with the name test.txt and "Hello World" as the contents of it. */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int fd;
	char buf[20];
	char *newdata = " World";
	ssize_t nr;
	off_t pos;

	//Open the file
	fd = open("test.txt", O_RDWR);
	if(fd == -1)
	{
		perror("open");
	}
	
	//Read data from the file
	nr = read(fd, buf, 11);
	if(nr == -1)
	{
		perror("read");
	}

	printf("Data read from file is: %s\n", buf);

	//Perform lseek and Write data
	printf("Altering the data in the file\n");
	pos = lseek(fd, (off_t)11, SEEK_SET);
	if(pos == (off_t)-1)
	{
		perror("lseek");
	}
	else
	{
		//Write the data
		nr = write(fd, newdata, 7);
		if(nr == -1)
			perror("write");
	
	}
	
	//Move the cursor back to the starting position
	pos = lseek(fd, (off_t)0, SEEK_SET);
	if(pos == (off_t)-1)
	{
		perror("Second lseek");
	}

	//Read the altered data
        nr = read(fd, buf, 17);
        if(nr == -1)
        {
                perror("Second read");
        }
	if(nr)
	{	
		buf[nr]='\0';		
	}

        printf("Data read from the altered file is: %s\n", buf);

	//Close the fd
	close(fd);


	return 0;
}
