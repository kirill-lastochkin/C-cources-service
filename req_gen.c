#include "service.h"

int fd[2];
extern int nput,nget;
extern pthread_t getproc;

int PutRequest(int num,struct Request *request)
{
    int chk;
    chk=write(fd[nput],request,sizeof(struct Request));
    if(chk==-1)
    {
        perror("read err\n");
        return -1;
    }
    return 0;
}

int GetHandledRequest(int num,struct HandledRequest *hreq)
{
    int chk;
    chk=read(fd[num],hreq,sizeof(struct HandledRequest));
    if(chk==-1)
    {
        perror("read err\n");
        return -1;
    }
    return 0;
}

void* PutThr(void *arg)
{
    int nput,cnt;
    struct Request req1;
    int c;
    char str1[]="happy birthday",str2[]="c dnjuhoy man",str3[]="privet kak dela";
    //c=rand()%3;
    c=2;
    switch(c)
    {
        case 0:
            strcpy(req1.msg,str1);
            break;
        case 1:
            strcpy(req1.msg,str2);
            break;
        case 2:
            strcpy(req1.msg,str3);
            break;
    }
    nput=*((int*)arg);
    req1.pid=getpid();
    for(cnt=0;cnt<200;cnt++)
    {
        //usleep(rand()%500000);
        usleep(100000);
        req1.req_num=rand()%3;
        PutRequest(nput,&req1);
        printf("requst %d sent\n",cnt);
    }
    sleep(5);
    pthread_cancel(getproc);
}

void* GetThr(void *arg)
{
    int nget,chk;
    nget=*((int*)arg);
    struct HandledRequest hreq1;
    pid_t pid;
    pid=getpid();
    while(1)
    {
        chk=GetHandledRequest(nget,&hreq1);
        if(chk==-1) sleep(1);
        else
        {
            if(hreq1.pid==pid)
            {
                printf("get: %s\n",hreq1.msg);
            }
        }
    }
}
