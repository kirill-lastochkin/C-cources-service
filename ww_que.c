#include "service.h"

int qid;
extern int end;
key_t key;

int StartQueue(void)
{
    key=ftok(FTOK_PATH,FTOK_INT);
    qid=msgget(key,IPC_CREAT|0666|IPC_EXCL);
    if(qid==-1)
        {
            if(errno==EEXIST)
            {
                qid=msgget(key,0);
                msgctl(qid,IPC_RMID,NULL);
                qid=msgget(key,IPC_CREAT|0666|IPC_EXCL);
            }
            else
            {
                perror("queue creation\n");
                return -1;
            }

        }
    qid=msgget(key,0);
    return qid;
}

int EndQueue(void)
{
    int chk;
    chk=msgctl(qid,IPC_RMID,NULL);
    if(chk==-1)
    {
        perror("queue remove");
        return -1;
    }
    printf("queue removed\n");
    return 0;
}

int GetFromQueue(struct Request *request)
{
    int chk;
    chk=msgrcv(qid,request,sizeof(struct Request),0,0);
    if(chk==-1)
    {
        if(end==0)
        {
            perror("msg receive\n");
        }
        return -1;
    }
    return 0;
}

int PutToQueue(struct Request *request)
{
    int chk;
    chk=msgsnd(qid,request,sizeof(struct Request),0);
    if(chk==-1)
    {
        if(end==0)
        {
            perror("msg send\n");
        }
        return -1;
    }
    return 0;
}
