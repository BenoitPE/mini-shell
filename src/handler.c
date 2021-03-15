#include "handler.h"

// handler used when command is in background and SIGCHLD is emitted
void bgWaitChildren(int sig) {
	pid_t pid;
	while ((pid = waitpid(-1, NULL, WNOHANG | WUNTRACED)) > 0);
}