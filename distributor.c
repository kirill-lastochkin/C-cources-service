#include "service.h"

int NumOfHandler,getr,putr,end;
pthread_t handlerr[MAX_NUM_OF_HANDLERS];
extern int canals;

//������������� ������ ��������������
//�������� �������, ����������� � ���, �������� �������, ���������� �������-������������
void InitDistributor(void)
{
    int chk;
    NumOfHandler=0;
    end=0;
    canals=0;
    getr=0; putr=1;
    signal(SIGINT,SigIntHandler);
    chk=CreateCanal(FIFOPATH1);
    if(chk==-1)
    {
        printf("error canal creating\n");
        exit(1);
    }
    chk=CreateCanal(FIFOPATH2);
    if(chk==-1)
    {
        printf("error canal creating\n");
        exit(1);
    }
    chk=ConnectCanal(FIFOPATH1,getr);
    if(chk==-1)
    {
        printf("error canal connecting");
        exit(1);
    }
    chk=ConnectCanal(FIFOPATH2,putr);
    if(chk==-1)
    {
        printf("error canal connecting");
        exit(1);
    }
    chk=StartQueue();
    if(chk==-1)
    {
        printf("error queue starting\n");
        exit(1);
    }
    for(chk=0;chk<MAX_NUM_OF_HANDLERS;chk++)
    {
        AddHandler();
    }
}

//���������� ������ �������
//���������� ������ �������-������������, ���������� �� �������, �������� �������
void EndDistributor(void)
{
    int i;
    for(i=0;i<NumOfHandler;i++)
    {
        DeleteHandler(i);
    }
    DisconnectCanal(putr);
    DisconnectCanal(getr);
    EndQueue();
}

//������� ������ ������-����������� � �������
//������� ���� ���������� �������
int AddHandler(void)
{
    int n;
    n=NumOfHandler;
    pthread_create(&handlerr[NumOfHandler],NULL,ReqHandlerProc,&n);
    NumOfHandler++;
    return NumOfHandler;
}

//������ ������ ������
int DeleteHandler(int num)
{
    pthread_cancel(handlerr[num-1]);
}

//����������� ������� ������ ��������������
//�������� ������� �� ������ � ������ �� � �������
void StartDistributor(void)
{
    struct Request req1;
    while(end==0)
    {
        GetRequest(getr,&req1);
        PutToQueue(&req1);
    }

}

void SigIntHandler(int num)
{
    EndDistributor();
    end=1;
}
