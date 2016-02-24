#include "service.h"

extern int putr;

void* ReqHandlerProc(void *arg)
{
    int num,i;
    struct Request req1;
    struct HandledRequest hreq1;
    num=*((int*)arg);
    while(1)
    {
        GetFromQueue(&req1);
        switch(req1.req_num)
        {
            case 0:
                for(i=0;req1.msg[i]!='\0';i++)
                {
                    if(req1.msg[i]>='a'&&req1.msg[i]<='z')
                    req1.msg[i]-=32;
                }
                break;
            case 1:
                for(i=0;req1.msg[i]!='\0';i++)
                {
                if(req1.msg[i]>='A'&&req1.msg[i]<='Z')
                    req1.msg[i]+=32;
                }
                break;
            case 2:
                for(i=0;req1.msg[i]!='\0';i++)
                {
                    req1.msg[i]++;
                }
                break;
        }
        hreq1.pid=req1.pid;
        strncpy(hreq1.msg,req1.msg,MSGLEN);
        SendBack(&hreq1);
    }

}

int SendBack(struct HandledRequest *hreq)
{
    PutHandledRequest(putr,hreq);
}
