#include <stdio.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include <errno.h>
#include <string.h>

#define MAX_MSG 10
#define BUFFER_SIZE 80

int main(void)
{
    struct mq_attr mqatrr;
    mqatrr.mq_flags = 0;
    mqatrr.mq_maxmsg = MAX_MSG;
    mqatrr.mq_msgsize = 80;

    mq_unlink("/test.mq");

    mqd_t mqd = mq_open("/test.mq", O_RDONLY | O_CREAT, 0666, &mqatrr);
    if (mqd == -1) {
        perror("mq_open");
        return 1;
    }

    char msg[BUFFER_SIZE];
    memset(msg, 0, sizeof(char) * BUFFER_SIZE);

    int res = mq_receive(mqd, msg, BUFFER_SIZE, NULL);
    if (res == -1) {
        perror("mq_receive");
        mq_close(mqd);
        return 1;
    }

    if (res < BUFFER_SIZE) {
        msg[res] = 0;
    }

    int fd = open("/home/box/message.txt", O_CREAT | O_WRONLY | O_TRUNC, 0666);
    if (fd == -1) {
        perror("open message.txt");
        return 1;
    }

    write(fd, msg, res);
    if (fd == -1) {
        perror("write");
        close(fd);
        mq_close(mqd);
        return 1;
    }
    close(fd);

    mq_close(mqd);

    return 0;
}
