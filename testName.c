#include <stdlib.h>
#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>

#define SHOWHOST /*include remote machine on output*/

int show_info(struct utmp *utbufp);

int main(){
	struct utmp current_record;/*read infointo here*/
	int utmpfd;/*read from this desciptor*/
	int reclen = sizeof(current_record);
	
	if ((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1)	{
		perror(UTMP_FILE); /*UTMP_FILE is in utmp.h*/
		exit(1);
	}

	while(read(utmpfd, &current_record, reclen)==reclen){
		show_info(&current_record);
	}
	close(utmpfd);
	return 0; /*went ok*/
}

int show_info(struct utmp *utbufp){
	printf("%-8.8s", utbufp->ut_name);
	printf(" ");
	printf("%-8.8s", utbufp->ut_line);
	printf(" ");
//	printf("%101d", utbufp->ut_time);
//	printf(" ");
#ifdef SHOWHOST
	printf("(%s)", utbufp->ut_host);
#endif
	printf("\n");
}

