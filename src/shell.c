/*
 * Copyright (C) 2002, Simon Nieuviarts
 */

#include <stdio.h>
#include <stdlib.h>
#include "readcmd.h"
#include "csapp.h"
#include "util.h"
#include "handler.h"

int main()
{
	// SIGCHLD linked to the handler bgWaitChildren for background processes
	Signal(SIGCHLD, bgWaitChildren);

	while (1) {
		struct cmdline *l;
		int i = 0;
		int fd = 0;
		pid_t pid = 0;
		pid_t childpid = 0;

		printf("shell> ");
		l = readcmd();

		/* If input stream closed, normal termination */
		if (!l) {
			printf("exit\n");
			exit(0);
		}

		if (l->err) {
			/* Syntax error, read another command */
			printf("error: %s\n", l->err);
			continue;
		}

		// If first command exists and is quit, normal termination
		if (l->seq[0] && strcmp(l->seq[0][0], "quit") == 0) {
			exit(EXIT_SUCCESS);
		}

		// Counts the number of pipes in the command line
		int nPipes = countPipesInCommandLine(l);
		// Creates the array of pipes
		int pipes[nPipes][2];
		createPipes(pipes, nPipes);

		// Iterates on the sequences separated by pipes
		while(l->seq[i] != 0) {
			// Current command
			char **cmd = l->seq[i];
			// Creates process to handle command
			if ((childpid = fork()) == 0) {

				// Redirects the input
				redirectCmdIn(l, fd);

				// Redirects the output
				redirectCmdOut(l, fd);

				// Redirects pipes
				redirectPipe(l, pipes, i);

				// Close all the pipes
				closePipes(pipes, nPipes);

				// Executes the command
				if (execvp(cmd[0], cmd) < 0) {
					perror(cmd[0]);
					exit(errno);
				}
				exit(EXIT_SUCCESS);
			} else if (childpid  < 0) {
				perror("Error! Could not create child!\n");
				exit(EXIT_FAILURE);
			}
			i++;
		}

		// Close all the pipes
		closePipes(pipes, nPipes);
		
		// If command is not in background, father waits its children
		if (!l->bg) {
			while ((pid = waitpid(-1, NULL, 0)) > 0);
		}
	}
}
