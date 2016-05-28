//Consumer code to consume the strings produced by the producer
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>


void read_char(int fp,char *buf,int no)
{
      int iterator1=0;
      for(;;)
	{	
		iterator1=0;		
		printf("\n");                
		read(fp, buf, 10);//read the string produced from the file
		printf("String read:");
		while(iterator1<10)
		{
		          printf("%c",buf[iterator1]);//print the consumed strings
                          iterator1++;
		}
		
		sleep(1);
        }


}



int main(int argc, char *argv[])
{
	
	int fp = open("/dev/mypipe", O_RDWR);//open the character deevice 
	char* buf = malloc(50);
        read_char(fp,buf,10);
        free(buf);//free the buff that contains the read string 
	return 0;
}
