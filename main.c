/*
 * генератор заявок компилировать с -D genrator -lpthread
 * обработчик компилировать с -D handler -lpthread
 * тесты функций с -D test -lpthread
 * */

#include "service.h"


#ifdef handler
int main(void)
{
    InitDistributor();
    StartDistributor();
    EndDistributor();
    return 0;
}
#endif
#ifdef generator

pthread_t putproc,getproc;
int nput,nget;

int main(void)
{
    nput=0,nget=1;
    ConnectCanal(FIFOPATH1,nput);
    ConnectCanal(FIFOPATH2,nget);
    pthread_create(&putproc,NULL,PutThr,&nput);
    pthread_create(getproc,NULL,GetThr,&nget);
    pthread_join(putproc,NULL);
    pthread_join(getproc,NULL);
    DisconnectCanal(nput);
    DisconnectCanal(nget);
    return 0;
}
#endif
#ifdef test
int main(void)
{
    return 0;
}
#endif
