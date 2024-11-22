#include "../inc/server.h"

void mx_daemon_start(void) {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd failed");
        exit(EXIT_FAILURE);
    }

    if (daemon(0, 0) == -1) {
        perror("daemon failed");
        exit(EXIT_FAILURE);
    }

    if (chdir(cwd) == -1) {
        perror("chdir failed");
        exit(EXIT_FAILURE);
    }

    openlog("MyDaemon", LOG_PID, LOG_DAEMON);
    syslog(LOG_INFO, "Daemon started");
}

void mx_daemon_end(int signal) {
    syslog(LOG_INFO, "Daemon received termination signal");
    daemon_running = 0;
}

void set_signal(void) {
    struct sigaction act;

    sigemptyset(&act.sa_mask);
    act.sa_handler = mx_daemon_end;
    act.sa_flags = SA_RESTART;

    if (sigaction(SIGTERM, &act, NULL) == -1) {
        syslog(LOG_ERR, "sigaction failed: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }
}
