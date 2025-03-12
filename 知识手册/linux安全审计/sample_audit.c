// audispd调研 https://blog.csdn.net/luofengmacheng/article/details/135091099



#include <stdio.h>
#include <libaudit.h>
#include <errno.h>

int main() {
    pid_t pid = getpid();
    printf("current pid=%d\n", pid);

    int fd = audit_open();
    int ret = 0;
    ret = audit_set_pid(fd, pid, WAIT_YES);
    if(ret<0) {
        perror("audit set_pid failed: ");
    }
    ret = audit_set_backlog_limit(fd, 8192);
    if(ret<0) {
        perror("audit_set_backlog_limit failed: ");
    }

    struct audit_reply audit_rep;

    struct sockaddr_nl local_addr;
    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.nl_family = AF_NETLINK;

    do {
        fd_set read_mask;
        FD_ZERO(&read_mask);
        FD_SET(fd, &read_mask);
        ret = select(fd + 1, &read_mask, NULL, NULL, NULL);
        if(ret < 0 ) {
            break;
        }
        ret = audit_get_reply(fd, &audit_rep,
                              GET_REPLY_NONBLOCKING, 0);
        audit_rep.msg.data[audit_rep.len] = '\0';
        printf("recv audit msg: %s\n", audit_rep.msg.data);
    } while(1);

    audit_close(fd);
}
