//Code to produce the strings that will be consumed by the consumer
#include<stdio.h>
#include<fcntl.h>
#include<assert.h>
#include<string.h>
#include<time.h>

void write_line(int fp)
{
     char buf[10]; 
     int iterator=0;
        while(1)
		{ 		
			
			memset(buf, 0, 10);
			for(iterator=0; iterator< 10; iterator++)
			buf[iterator] = 'A' + rand() % 26;//randomly generated strings
			buf[iterator]='\0';
			printf("Produced string:");
			printf("%s\n",buf);//print the string
			write(fp, buf, 10);//write it to the file opened
			sleep(1);
		}

}



int main(int argc, char *argv[])
{
        srand(time(NULL));
        
        int fp = open("/dev/mypipe", O_WRONLY);//open the character device to put the strings produced 
                write_line(fp);
		
        close(fp);
	return 0;
}








































