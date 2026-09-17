#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

static double now(void) {
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) _Exit(125);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1000000000.0;
}

int main(int argc, char **argv) {
    if (argc < 3) return 125;
    char *end = NULL;
    long seconds = strtol(argv[1], &end, 10);
    if (!end || *end || seconds < 1 || seconds > 120) return 125;
    pid_t child = fork();
    if (child < 0) return 125;
    if (child == 0) {
        if (setsid() < 0) _Exit(125);
        execvp(argv[2], &argv[2]);
        perror("execvp");
        _Exit(127);
    }
    double deadline = now() + (double)seconds;
    int sent_term = 0;
    for (;;) {
        int status;
        pid_t got = waitpid(child, &status, WNOHANG);
        if (got == child) {
            if (sent_term) return 124;
            if (WIFEXITED(status)) return WEXITSTATUS(status);
            if (WIFSIGNALED(status)) return 128 + WTERMSIG(status);
            return 125;
        }
        if (got < 0 && errno != EINTR) return 125;
        if (now() >= deadline) {
            if (!sent_term) {
                fprintf(stderr, "BOUNDED_TIMEOUT seconds=%ld child=%ld\n", seconds, (long)child);
                (void)kill(-child, SIGTERM);
                sent_term = 1;
                deadline = now() + 1.0;
            } else {
                (void)kill(-child, SIGKILL);
                while (waitpid(child, &status, 0) < 0 && errno == EINTR) {}
                return 124;
            }
        }
        struct timespec delay = {0, 100000000};
        (void)nanosleep(&delay, NULL);
    }
}
