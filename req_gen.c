#include "service.h"

int fd[2];
extern int nput,nget;

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
    char str[]="happy birthday";
    nput=*((int*)arg);
    req1.pid=getpid();
    strcpy(req1.msg,str);
    for(cnt=0;cnt<200;cnt++)
    {
        usleep(rand()%500000);
        req1.req_num=rand()%3;
        PutRequest(nput,&req1);
        printf("requst %d sent\n",cnt);
    }
}

void* GetThr(void *arg)
{
    int nget,chk;
    nget=*((int*)arg);
    struct HandledRequest hreq1;
    while(1)
    {
        chk=GetHandledRequest(nget,&hreq1);
        if(chk==-1) sleep(1);
        else
        printf("get: %s\n",hreq1.msg);
    }
}
