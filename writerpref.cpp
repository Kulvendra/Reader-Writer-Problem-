#include<semaphore.h>
#include<pthread.h>
#include<bits/stdc++.h>
#include<unistd.h>
using namespace std;
#define rcount 5
#define wcount 3

int readCount=0,writeCount=0,sharedData=5;

sem_t wlock,rlock,mlock,readtry,writetry;

void* reader(void *n){

    while(1){

    sem_wait(&readtry);    
    sem_wait(&rlock);
    readCount++;
    if(readCount==1)
        sem_wait(&writetry);   
    sem_post(&rlock);
    sem_post(&readtry);

    sem_wait(&mlock);
    cout<<"📖 \033[31;1mReader "<< *(int*)n<<" is reading."<<"   [ Shared data read  : "<<sharedData<<" ]"<<endl;
    sem_post(&mlock);

    sem_wait(&rlock);
    readCount--;
    if(readCount==0)
        sem_post(&writetry);
    sem_post(&rlock);

    sleep(1 + rand() % 5);
    }

}

void *writer(void *n){
    while(1){

    sem_wait(&wlock);
    writeCount++;
    if(writeCount==1)
        sem_wait(&readtry);
    sem_post(&wlock);

    sem_wait(&writetry);    
    sharedData +=5;
    cout<<"✍️  \033[33;1mWriter "<< *(int*)n<<" is writing."<<"   [ Shared data write : "<<sharedData<<" ]"<<endl;
    sem_post(&writetry);

    sem_wait(&wlock);
    writeCount--;
    if(writeCount==0)
        sem_post(&readtry);
    sem_post(&wlock);
    sleep(1 + rand() % 3);
    
    }

}
int main(){

    sem_init(&wlock,0,1);
    sem_init(&rlock,0,1);
    sem_init(&mlock,0,1);
    sem_init(&readtry,0,1);
    sem_init(&writetry,0,1);



    pthread_t r[rcount],w[wcount];

    int rid[rcount],wid[wcount];
    for(int i=0;i<rcount;i++){        
             rid[i]=i;
        pthread_create(&r[i], NULL, reader, &rid[i]);
        
    }

    for(int j=0;j<wcount;j++){   
         wid[j]=j;
        pthread_create(&w[j], NULL, writer,&wid[j]);
    }
        
    for(int i=0;i<rcount;i++){
        pthread_join(r[i],NULL);
    }

    for(int i=0;i<wcount;i++){
        pthread_join(w[i],NULL);

    }

    return 0;

}