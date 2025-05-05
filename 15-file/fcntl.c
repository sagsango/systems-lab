// TODO

/*

    TODO: Figureout why none of the file lock operations are failing
*/



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include <sys/file.h>
#include <sys/fcntl.h>
#include <pthread.h>

#define FILENAME "/home/ssing214/systems-lab/15-file/fcntl.c"


void * tf(void *arg) {
    int fd = (int)arg;
    struct flock fl;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 10;
    fl.l_type = F_RDLCK;
    fl.l_pid = getpid()+1;

    int status = fcntl(fd, F_LOCK,  &fl);
    if (status < 0) {
        perror("F_LOCK failed");
    }
    pthread_exit(NULL);
}
int main() {
    int status;
    int fd = open(FILENAME, O_RDONLY);
    if (fd < 0) {
        perror("Cant open directory");
    }

    /*
        We can do many opedations with fcntl; some of the ops
        # define F_RDLCK		0	/ Read lock.  /
        # define F_WRLCK		1	/ Write lock.  /
        # define F_UNLCK		2	/ Remove lock.  /
    
        #define F_DUPFD		0	/ Duplicate file descriptor.  /
        #define F_GETFD		1	/ Get file descriptor flags.  /
        #define F_SETFD		2	/ Set file descriptor flags.  /
        #define F_GETFL		3	/ Get file status flags.  /
        #define F_SETFL		4	/ Set file status flags.  /
    */

    //fcntl(int fd, int cmd, ...)

    int flags = fcntl(fd, F_GETFL);
    printf("Falgs: %s| %s\n", flags & O_RDONLY ? "O_RDONLY" : "0", flags & __O_DIRECTORY ? "__ODIRECTORY" : "0" );

    struct flock fl;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 10;
    fl.l_type = F_RDLCK;
    fl.l_pid = getpid();

    status = fcntl(fd, F_LOCK,  &fl);
    if (status < 0) {
        perror("F_LOCK failed");
    }

    switch (fork()) {
        case -1: {
            perror("Fork failed");
            break;
        }
        case 0: {
            fl.l_pid = getpid();
            status = fcntl(fd, F_LOCK,  &fl);
            if (status < 0) {
                perror("F_LOCK failed");
            }
            exit(0);
        }
    }

    pthread_t t;
    pthread_create (&t, 0, tf, (void*)fd);
    pthread_join (t, NULL);

    status = close(fd); // destroy & unlock the lock
    if (status < 0) {
        perror("Cant close the dir fd");
    }




   
}