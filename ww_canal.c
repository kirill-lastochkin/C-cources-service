#include "service.h"

int canals,fd[NUM_OF_CANALS];
extern int end;

//�������� �������, ������� �� �����
//�������: -1 ������, ����� ������ ��� ����� (� 0), NUM_OF_CANALS ��� ���������� �������
int CreateCanal(char *path)
{
    if(canals>=NUM_OF_CANALS)
    {
        printf("too many canals\n");
        return canals;
    }
    unlink(path);
    fd[canals]=mkfifo(path,0666);
    if(fd[canals]==-1)
    {
        perror("mkfifo\n");
        return -1;
    }
    canals++;
    return canals-1;
}

int ConnectCanal(char *path, int num)
{
    fd[num]=open(path,0666);
    if(fd[num]==-1)
    {
        perror("open err\n");
        return -1;
    }
    else
    {
        return 0;
    }

}

void DisconnectCanal(int num)
{
    close(fd[num]);
}

int GetRequest(int num,struct Request* request)
{
    int chk;
    chk=read(fd[num],request,sizeof(struct Request));
    if(chk==-1)
    {
        if(end==0)
        {
            perror("read error\n");
        }
        return -1;
    }
    return 0;
}

int PutHandledRequest(int num,struct HandledRequest* hreq)
{
    int chk;
    chk=write(fd[num],hreq,sizeof(struct HandledRequest));
    if(chk==-1)
    {
        if(end==0)
        {
            perror("write error\n");
        }
        return -1;
    }
    return 0;
}
