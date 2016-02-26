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

//стрктура под заявку
struct Request
{
    pid_t pid;
    char msg[MSGLEN];
    int req_num;
};
//структра под обработанную заявку
struct HandledRequest
{
    pid_t pid;
    char msg[MSGLEN];
};
//структура под сообщение в очереди
struct Message
{
    long mtype;
    struct Request request;
};


//-----------распределитель заявок---------------
void InitDistributor(void); //+/открыть необходимые канала, создать очередь, потоки обработки заявок
void EndDistributor(void); //+/закрыть все открытое
int AddHandler(void); //+/добавить обработчик, возвращает его номер
int DeleteHandler(int num); //+/удалить обработчик по номеру
void StartDistributor(void); //+/рабочий цикл
void SigIntHandler(int);

//-----------обработчик заявок-------------------
void* ReqHandlerProc(void *arg); //+/главная задача - переделать Request в HandledRequest
int SendBack(struct HandledRequest *hreq); //+/выслать обработанную заявку назад

//-----------канал движения заявок---------------
int CreateCanal(char *path); //+/создать канал по указанному пути, 0/-1 удачно/неудачно, возвращает номер канала
int ConnectCanal(char *path, int num); //+/подключиться к каналу по пути, -1 при ошибке
void DisconnectCanal(int num);   //+/отключиться от канала с указанным номером
int GetRequest(int num,struct Request* request);  //+/получить запрос из канала c с номером, на выходе заявка
int PutHandledRequest(int num, struct HandledRequest*hreq); //+/отправить обработанный запрос в канал

//----------работа с очередью заявок-------------------
int StartQueue(void); //+/создать очередь, в которую будут сыпаться сообщения типа Message, -1 ошибка
int EndQueue(void); //+/закрыть очередь
int GetFromQueue(struct Request *request); //+/забрать заявку из очереди
int PutToQueue(struct Request *request); //+/положить заявку в очередь

//----------генератор заявок--------------
int PutRequest(int num,struct Request *request); //+/отправить запрос в канал по номеру
int GetHandledRequest(int num, struct HandledRequest*hreq); //+/достать обработанный запрос из канала
void* PutThr(void *arg); //+/
void* GetThr(void *arg); //+/


#endif
