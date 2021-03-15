#ifndef __UTIL_H__
#define __UTIL_H__

#include "csapp.h"
#include "readcmd.h"

#define PIPEREAD 0
#define PIPEWRITE 1

void redirectCmdIn(struct cmdline *l, int fd);

void redirectCmdOut(struct cmdline *l, int fd);

int countPipesInCommandLine(struct cmdline *l);

void createPipes(int pipes[][2], int nPipes);

void redirectPipe(struct cmdline *l, int pipes[][2], int i);

void closePipes(int pipes[][2], int nPipes);

#endif