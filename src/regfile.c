#include "regfile.h"

clock_t begintime;
char *regfile;
FILE *fp;

void initRegister(){
    char inputfile[50];
    begintime=clock();
    regfile = getenv("LOG_FILENAME");

    if (regfile==NULL){
        printf("No log file defined!\nInsert file:\n");
        scanf("%s",inputfile);
        printf("export LOG_FILENAME='filename.txt' to set environment variable and log actions always in the same file\n");

        setenv("LOG_FILENAME",inputfile,1);
        regfile = getenv("LOG_FILENAME");
    }
    fp=fopen(regfile,"w");
    if(fp == NULL) {
        perror("Error opening file");
        exit(1);
    }
}

void regCommand(int argc,char const *argv[]){
    clock_t actiontime=clock();
    double instant;
    char command[100] = "";

    for(int i = 1; i < argc;i++){
		strcat(command,argv[i]);
        if(i!=argc-1)
            strcat(command," ");
	}

    instant = (actiontime - begintime)/(CLOCKS_PER_SEC / (double) 1000.0);
    fprintf(fp, "%.2f - %.8d - CREATE - %s\n", instant, getpid(), command);
}

pid_t regFork(flags *flags){
    clock_t actiontime=clock();
    double instant;
    char outflags[100]="";
    char depth[20],blocksize[20];

    if (flags->countLinks) strcat(outflags," -l");
    strcat(outflags,flags->dir);
    if (flags->all) strcat(outflags," -a");
    if(flags->bytes) strcat(outflags," -b");
    sprintf(blocksize," --block-size=%d",flags->blockSizeValue);
    if (flags->blockSize) strcat(outflags,blocksize);
    if(flags->dereference) strcat(outflags," -L");
    if(flags->separateDirs) strcat(outflags," -S");
    sprintf(depth," --max-depth=%d",flags->maxDepthValue);
    if(flags->maxDepth) strcat(outflags,depth);

    instant = (actiontime - begintime)/(CLOCKS_PER_SEC / (double) 1000.0);
    
    fprintf(fp, "%.2f - %.8d - CREATE - %s\n", instant, getpid(),outflags);

    return fork();
}

void regExit(int status){
    clock_t actiontime=clock();
    double instant;

    instant = (actiontime - begintime)/(CLOCKS_PER_SEC / (double) 1000.0);

    fprintf(fp, "%.2f - %.8d - EXIT - %d\n", instant, getpid(),status);
    exit(status);
}

int regSendSignal(pid_t dpid, int signo){
    clock_t actiontime=clock();
    double instant;

    instant = (actiontime - begintime)/(CLOCKS_PER_SEC / (double) 1000.0);

    fprintf(fp, "%.2f - %.8d - SEND_SIGNAL - %d to process: %d\n", instant, getpid(),signo, dpid);
    
    return kill(dpid,signo);
}

void regReceiveSignal(int signal){ //handler dos sinais
    clock_t actiontime=clock();
    double instant;

    instant = (actiontime - begintime)/(CLOCKS_PER_SEC / (double) 1000.0);

    fprintf(fp, "%.2f - %.8d - RECV_SIGNAL - %d\n", instant, getpid(),signal);
}

int regSendMessage(int fd, void *buf, size_t n){
    clock_t actiontime=clock();
    double instant;

    instant = (actiontime - begintime)/(CLOCKS_PER_SEC / (double) 1000.0);

    fprintf(fp, "%.2f - %.8d - SEND_PIPE -", instant, getpid());

    for(int i=0;i<n;i++){ //print bytes of the message for now
        fprintf(fp, " %"PRIu8, ((uint8_t *)buf)[i]);
    }
    fprintf(fp, "\n");

    return write(fd,buf,n);
}

int regReceiveMessage(int fd, void *buf, size_t n){
    clock_t actiontime=clock();
    double instant;

    instant = (actiontime - begintime)/(CLOCKS_PER_SEC / (double) 1000.0);
    
    fprintf(fp, "%.2f - %.8d - RECV_PIPE -", instant, getpid());

    for(int i=0;i<n;i++){ //print bytes of the message for now
        fprintf(fp, " %"PRIu8, ((uint8_t *)buf)[i]);
    }
    fprintf(fp, "\n");

    return read(fd,buf,n);
}