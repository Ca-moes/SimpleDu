#ifndef REGFILE_H
#define REGFILE_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <inttypes.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include "flags.h"

void initRegister();

/*
function to log the command in the file
*/
void regCommand(int argc,char const *argv[]);

/*
function to make a new process and log it in the file
*/
pid_t regFork(flags *flags);

/*
function to exit a process and log it in the file
*/
void regExit(int status);

/*
function to receive a signal and log it in the file
this function will be the signal_handler
*/
void regReceiveSignal(int signal);

/*
function to send a signal and log it in the file
*/
int regSendSignal(pid_t dpid,int signo);

/*
function to send a message using pipe and log it in the file
*/
int regSendMessage(int fd, void *buf, size_t n);

/*
function to receive a message using pipe and log it in the file
*/
int regReceiveMessage(int fd, void *buf, size_t n);

#endif /*REGFILE_H*/