#include "regfile.h"

struct timespec start_time;
char *regfile;
FILE *fp;

void initRegister(){
    char inputfile[50];
    regfile = getenv("LOG_FILENAME");

    clock_gettime(CLOCK_MONOTONIC, &start_time);
    
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
    struct timespec action_time;
    char command[100] = "";

    clock_gettime(CLOCK_MONOTONIC, &action_time);

    for(int i = 1; i < argc;i++){
		strcat(command,argv[i]);
        if(i!=argc-1)
            strcat(command," ");
	}
    fprintf(fp, "%.2f - %.8d - CREATE - %s\n", getTimefromBeggining(action_time), getpid(), command);
}

pid_t regFork(flags *flags){
    struct timespec action_time;
    clock_gettime(CLOCK_MONOTONIC, &action_time);

    char outflags[100]="";
    char depth[20],blocksize[20];

    if (flags->countLinks) strcat(outflags,"-l ");
    strcat(outflags,flags->dir);
    if (flags->all) strcat(outflags," -a");
    if(flags->bytes) strcat(outflags," -b");
    sprintf(blocksize," --block-size=%d",flags->blockSizeValue);
    if (flags->blockSize) strcat(outflags,blocksize);
    if(flags->dereference) strcat(outflags," -L");
    if(flags->separateDirs) strcat(outflags," -S");
    sprintf(depth," --max-depth=%d",flags->maxDepthValue);
    if(flags->maxDepth) strcat(outflags,depth);
    
    fprintf(fp, "%.2f - %.8d - CREATE - %s\n", getTimefromBeggining(action_time), getpid(),outflags);
    fflush(fp);

    return fork();
}

void regExit(int status){
    struct timespec action_time;
    clock_gettime(CLOCK_MONOTONIC, &action_time);

    fprintf(fp, "%.2f - %.8d - EXIT - %d\n", getTimefromBeggining(action_time), getpid(),status);
    exit(status);
}

int regSendSignal(pid_t dpid, int signo){
    struct timespec action_time;
    clock_gettime(CLOCK_MONOTONIC, &action_time);

    fprintf(fp, "%.2f - %.8d - SEND_SIGNAL - %d to process: %d\n", getTimefromBeggining(action_time), getpid(),signo, dpid);
    
    return kill(dpid,signo);
}

void regReceiveSignal(int signal){ //handler dos sinais
    struct timespec action_time;
    clock_gettime(CLOCK_MONOTONIC, &action_time);

    fprintf(fp, "%.2f - %.8d - RECV_SIGNAL - %d\n", getTimefromBeggining(action_time), getpid(),signal);
}

int regSendMessage(int fd, void *buf, size_t n){
    struct timespec action_time;
    clock_gettime(CLOCK_MONOTONIC, &action_time);

    fprintf(fp, "%.2f - %.8d - SEND_PIPE -", getTimefromBeggining(action_time), getpid());

    for(int i=0;i<n;i++){ //print bytes of the message for now
        fprintf(fp, " %"PRIu8, ((uint8_t *)buf)[i]);
    }
    fprintf(fp, "\n");

    return write(fd,buf,n);
}

int regReceiveMessage(int fd, void *buf, size_t n){
    struct timespec action_time;
    clock_gettime(CLOCK_MONOTONIC, &action_time);
    
    fprintf(fp, "%.2f - %.8d - RECV_PIPE -", getTimefromBeggining(action_time), getpid());

    for(int i=0;i<n;i++){ //print bytes of the message for now
        fprintf(fp, " %"PRIu8, ((uint8_t *)buf)[i]);
    }
    fprintf(fp, "\n");

    return read(fd,buf,n);
}

double getTimefromBeggining(struct timespec action_time){
    return (action_time.tv_sec-start_time.tv_sec)*1000+((action_time.tv_nsec-start_time.tv_nsec)/10e6);
}
