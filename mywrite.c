 /* write0.c
 *
 *purpose: send messages to another terminal
 *	method: open the other terminal for output then
 *	copy from stdin to that terminal
 *	shows: a terminal is just a file supporting regular i/o
 *	usage: write0 ttyname 
 *	Example usage: ./write0 username
 */
#include	<utmp.h>
#include	<stdio.h>
#include 	<stdlib.h>
#include	<fcntl.h>
#include 	<unistd.h>
#include 	<string.h>
#include 	<errno.h>
#include	<stdbool.h>
#define SHOWHOST /*include remote machine on output*/

bool show_info(struct utmp *utbufp, char input[], char *fd);

int main(int ac, char *av[])
{
	
	int	fd;
	char buf[BUFSIZ];

	struct utmp current_record;/*read infointo here*/
	int utmpfd;/*read from this desciptor*/
	int reclen = sizeof(current_record);
	char input[255*sizeof(char)];
	char file[255];
	bool foundUser = 0;	
	
	if (ac != 2)
	{
		fprintf(stderr,"usage: write0 ttyname\n");
		exit(1);
	}

	if ((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1)
	{
		perror(UTMP_FILE); /*UTMP_FILE is in utmp.h*/
		exit(1);
	}

	while(read(utmpfd, &current_record, reclen)==reclen)
	{
		foundUser = show_info(&current_record, av[1],file);
		if(foundUser == 1)
			break;
	}
	printf("Found the User: %s",file );
	close(utmpfd);

	/* check args */

	/* open devices */
	fd = open( file, O_WRONLY );
	if ( fd == -1 ){
		perror(av[1]); 
                exit(1);
	}

	/* loop until EOF on input */
	while( fgets(buf, BUFSIZ, stdin) != NULL )
		if ( write(fd, buf, strlen(buf)) == -1 )
			break;

	close(fd);

	return 0;
}

bool show_info(struct utmp *utbufp, char input[], char *file)
{
	int test;
	char user[255*sizeof(char)];
	printf("Looking for:%s \n", input);
	sprintf(user,"%s", utbufp->ut_name);
	printf("Found: %s\n", user);
	test = strcmp(input, user);
	printf("Compairing = %d\n",test);
	if(strcmp(user, input) == 0)
	{
		sprintf(file,"/dev/%s", utbufp->ut_line);
		printf("user found!");
		return 1;
	}
	else
	{
		return 0;
	}
	
//#ifdef SHOWHOST
	//printf("(%s)", utbufp->ut_host);
//#endif
//	printf("\n");
	
}

