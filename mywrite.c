/* write0.c
 *    se: send messages to another terminal
 *    method: open the other terminal for output then
 *    copy from stdin to that terminal
 *    shows: a terminal is just a file supporting regular i/o
 *    usage: write0 ttyname
 *    Example usage: ./write0 username
 */
#include    <utmp.h>
#include    <stdio.h>
#include     <stdlib.h>
#include    <fcntl.h>
#include     <unistd.h>
#include     <string.h>
#include     <errno.h>
#include    <stdbool.h>
#include    <sys/stat.h>
#include    <time.h>

#define SHOWHOST /*include remote machine on output*/

bool show_info(struct utmp *utbufp, char input[], char ttyInput[], char *file, bool getTTY, int lowest);

int main(int ac, char *av[])
{
   
    int fd;
    char buf[BUFSIZ];
    struct utmp current_record;/*read infointo here*/
    int utmpfd;/*read from this desciptor*/
    int reclen = sizeof(current_record);
    char input[255*sizeof(char)];
    char file[255];
    char ttyList[20][255*sizeof(char)];
    bool foundUser = 0;    
  	bool getTTY = 0;
  	int i = 0;
	int lowest = 0;
        char openMessage[237*sizeof(char)]; 
	char *mytty = ttyname(STDIN_FILENO);
	char myTTY[255*sizeof(char)];
	char username[255];
        getlogin_r(username,255);
	time_t currentTime = time(0);
        struct tm *hm;
	char timeBuffer[50];
	time (&currentTime);
	hm = localtime(&currentTime);
	
	strftime(timeBuffer, 50, "%H:%M", hm);
	
	
    if (ac == 2)
    {
        lowest = show_info(&current_record, av[1], av[2], file, getTTY, lowest); 
	printf("%s",file);  
        close(utmpfd);

		/* open devices */
        fd = open(file, O_WRONLY);
        if ( fd == -1 )
        {
            perror(av[1]);
            exit(1);
        }
		printf("%s",timeBuffer);
		sprintf(openMessage, "Message from %s@%s on yourtty at %s ...", username, mytty, timeBuffer);
		write(fd, openMessage, strlen(buf));
		
        while(fgets(buf, BUFSIZ, stdin) != NULL)
        {
            if (write(fd, buf, strlen(buf)) == -1)
		{
                break;
		}
        }
		
        close(fd);
        return 0;
    }
    
    else if(ac == 3)
    {
        getTTY = 1;
        lowest = show_info(&current_record, av[1], av[2], file, getTTY, lowest); 
		
		close(utmpfd);
        
		/* open devices */
        fd = open(file, O_WRONLY );
        if ( fd == -1 )
        {
            perror(av[1]);
            exit(1);
        }

        while( fgets(buf, BUFSIZ, stdin) != NULL )
        {
            if ( write(fd, buf, strlen(buf)) == -1 )
                break;
        }
        close(fd);

        return 0;
    }
        
    else
    {
        printf("Improper usage.");
    }
}
bool show_info(struct utmp *utbufp, char input[],char ttyInput[], char *file, bool getTTY, int lowest)
{
    struct stat infobuf;
    char user[255*sizeof(char)];
	int idle;
    
    
    if(getTTY == 0)
    {
               
        sprintf(user,"%s", utbufp->ut_name);
        
        if(strcmp(user, input) == 0)
        {
            sprintf(file, "/dev/%s", utbufp->ut_line);
        
			if(stat(input, &infobuf) == -1)
			{
				perror(input);
			}
			else
			{
				time_t currentTime = time(0);
				idle = currentTime - infobuf.st_atime;
			}
			
			if(idle < lowest)
			{
				lowest = idle;
				printf("dev/%s",utbufp->ut_line);
				sprintf(file, "/dev/%s", utbufp->ut_line);
			}
        }
    }
    
    else if(getTTY == 1)
    {
        sprintf(user,"%s", utbufp->ut_line);
        if((strcmp(user, ttyInput) == 0) && (strcmp((utbufp->ut_name), input) == 0)) 
        {
			
            sprintf(file,"/dev/%s", utbufp->ut_line);
            return 1;
        }
		else
		{
			printf("Username and tty mismatch!");
			perror(input);
		}
    }
    
    else
    {
        return 0;
    }
    
}



