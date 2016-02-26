#ifndef _SERVICE_H
#define _SERVICE_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include <time.h>
#include <sys/syscall.h>
#include <errno.h>

#define MAX_NUM_OF_HANDLERS 6
#define FTOK_PATH "/home/"
#define FTOK_INT 'r'
#define MSGLEN 50
#define FIFOPATH1 "/home/kirill/kurs/dhcp/fifo1"
#define FIFOPATH2 "/home/kirill/kurs/dhcp/fifo2"
#define NUM_OF_CLIENTS 1
#define NUM_OF_CANALS 2

//�������� ��� ������
struct Request
{
    pid_t pid;
    char msg[MSGLEN];
    int req_num;
};
//�������� ��� ������������ ������
struct HandledRequest
{
    pid_t pid;
    char msg[MSGLEN];
};
//��������� ��� ��������� � �������
struct Message
{
    long mtype;
    struct Request request;
};


//-----------�������������� ������---------------
void InitDistributor(void); //+/������� ����������� ������, ������� �������, ������ ��������� ������
void EndDistributor(void); //+/������� ��� ��������
int AddHandler(void); //+/�������� ����������, ���������� ��� �����
int DeleteHandler(int num); //+/������� ���������� �� ������
void StartDistributor(void); //+/������� ����
void SigIntHandler(int);

//-----------���������� ������-------------------
void* ReqHandlerProc(void *arg); //+/������� ������ - ���������� Request � HandledRequest
int SendBack(struct HandledRequest *hreq); //+/������� ������������ ������ �����

//-----------����� �������� ������---------------
int CreateCanal(char *path); //+/������� ����� �� ���������� ����, 0/-1 ������/��������, ���������� ����� ������
int ConnectCanal(char *path, int num); //+/������������ � ������ �� ����, -1 ��� ������
void DisconnectCanal(int num);   //+/����������� �� ������ � ��������� �������
int GetRequest(int num,struct Request* request);  //+/�������� ������ �� ������ c � �������, �� ������ ������
int PutHandledRequest(int num, struct HandledRequest*hreq); //+/��������� ������������ ������ � �����

//----------������ � �������� ������-------------------
int StartQueue(void); //+/������� �������, � ������� ����� �������� ��������� ���� Message, -1 ������
int EndQueue(void); //+/������� �������
int GetFromQueue(struct Request *request); //+/������� ������ �� �������
int PutToQueue(struct Request *request); //+/�������� ������ � �������

//----------��������� ������--------------
int PutRequest(int num,struct Request *request); //+/��������� ������ � ����� �� ������
int GetHandledRequest(int num, struct HandledRequest*hreq); //+/������� ������������ ������ �� ������
void* PutThr(void *arg); //+/
void* GetThr(void *arg); //+/


#endif
