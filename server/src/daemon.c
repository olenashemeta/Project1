#include "../inc/server.h"

void mx_daemon_start(void) {
    char current_dir[PATH_MAX];
    if (!getcwd(current_dir, sizeof(current_dir))) {
        perror("getcwd failed");
        exit(EXIT_FAILURE);
    }

    if (daemon(0, 0) == -1) {
        perror("daemon failed");
        exit(EXIT_FAILURE);
    }

    if (chdir(current_dir) == -1) {
        perror("chdir failed");
        exit(EXIT_FAILURE);
    }

    openlog("UchatServer", LOG_PID, LOG_DAEMON);
    syslog(LOG_INFO, "Daemon started in directory: %s", current_dir);
}

void mx_daemon_end(int signal, siginfo_t *info, void *context) {
    (void)info;

    if (context == NULL) {
        syslog(LOG_WARNING, "Received termination signal, but context is NULL");
        return;
    }

    t_server *server_ctx = (t_server *)context;
    syslog(LOG_INFO, "Daemon received termination signal %d", signal);
    server_ctx->is_running = false;
}

void set_signal(t_server *server) {
    if (server == NULL) {
        syslog(LOG_ERR, "Server context is NULL in set_signal");
        exit(EXIT_FAILURE);
    }

    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = mx_daemon_end;

    if (sigaction(SIGTERM, &act, NULL) == -1) {
        syslog(LOG_ERR, "sigaction failed: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }
}
