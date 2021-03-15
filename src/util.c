#include "util.h"

void redirectCmdIn(struct cmdline *l, int fd) {
	// If there is a redirection of input
	if (l->in) {
		// Tries redirecting input with correct filename
		if ((fd = open(l->in, O_RDONLY)) != -1) {
			dup2(fd, 0);
			close(fd);
		} else {
			// Permission denied on in
			perror(l->in);
			exit(errno);
		}
	}
}

void redirectCmdOut(struct cmdline *l, int fd) {
	// If there is a redirection of output
	if (l->out) {
		// Tries redirecting output with correct filename
		if ((fd = open(l->out, O_CREAT | O_WRONLY, 0744)) != -1) {
			dup2(fd, 1);
			close(fd);
		} else {
			// Permission denied on out
			perror(l->out);
			exit(errno);
		}
	}
}

int countPipesInCommandLine(struct cmdline *l) {
	int nPipes = 0;
	// Counts the number of pipes in command line
	while (l->seq[nPipes] != 0) {
		nPipes++;
	}
	// Returns number - 1 because the count begins at the sequence at index 0
	return nPipes - 1;
}

void createPipes(int pipes[][2], int nPipes) {
	// Creates the correct number of pipes
	int i = 0;
	for (i = 0; i < nPipes; i++) {
		if (pipe(pipes[i]) < 0) {
			perror("Error! Could not create pipe!\n");
			exit(EXIT_FAILURE);
		}
	}
}

void redirectPipe(struct cmdline *l, int pipes[][2], int i) {
	// if not first command, redirects read end of pipe
	if (i != 0) {
		if (dup2(pipes[i-1][PIPEREAD], PIPEREAD) < 0) {
			perror("Error! Could not redirect pipe!\n");
			exit(EXIT_FAILURE);
		}
	}

	// if not last command, redirects write end of pipe
	if (l->seq[i + 1] != 0) {
		if (dup2(pipes[i][PIPEWRITE], PIPEWRITE) < 0) {
			perror("Error! Could not redirect pipe!\n");
			exit(EXIT_FAILURE);
		}
	}
}

void closePipes(int pipes[][2], int nPipes) {
	// Closes all the pipes
	int i = 0;
	for (i = 0; i < nPipes; i++) {
		close(pipes[i][PIPEWRITE]);
		close(pipes[i][PIPEREAD]);
	}
}