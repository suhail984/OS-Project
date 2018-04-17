#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#define MAXM 10
#define MAXN 10
int proccurr[3][3];
int temp[3][3];      /*temp array location*/
int available[10];
int max[3][3]={{10,10,10},{10,10,10},{10,10,10}};
int allocation[3][3]={{1,2,3},{3,2,1},{1,1,1,}};
int need [3][3];
int counti=0;          /*need[n][m]=max[n][m]-allocation[n][m] */
int countj=0;
int threadsi=3;
int threadsj=3;

void *inc_count(void *q);
void *watch_count(void *r);

pthread_mutex_t mutex;                        /*mutex id */
pthread_cond_t count_threshold_cv;

int main()
{
pthread_t proccurr[3][3];
pthread_attr_t attr;                         /*thread   id   */
int i,j;
long r1=1,r2=2,r3=3;

if(pthread_mutex_init(&mutex,NULL)<0)
{
perror("pthread_mutex_init error");
exit(1);
}
else                                                 /*pthread_mutex_init((&mutex,NULL)  */
pthread_cond_init(&count_threshold_cv,NULL);         
pthread_attr_init(&attr);                       /* get deafault atributes    */     
pthread_create(&proccurr[0][0],&attr, watch_count,(void *)r1);
pthread_create(&proccurr[1][0],&attr, inc_count,(void *)r2);
pthread_create(&proccurr[2][0],&attr, inc_count,(void *)r3);

for(i=0;i<=threadsi;i++)
{
for(j=0;j<=threadsj;j++)
{
                                          /*  pthrrad join  */
pthread_join(proccurr[i][j],NULL);

}
}


printf("main:waited on %d, %d threds. done.\n",threadsi,threadsj);
pthread_attr_destroy(&attr);
pthread_mutex_destroy(&mutex);
pthread_cond_destroy(&count_threshold_cv);
pthread_exit(NULL);
}

void *inc_count(void *q)
{                                  //process are running
int i,j,n,m;
long my_id=(long)q;                    //thread of process is initialize
for(i=0;i<10;i++)
{
for(j=0;j<10;j++)
{
need[n][m]=max[n][m]-allocation[i][j];
printf("allocation=%d,need= %d\n",allocation[i][j],need[n][m]);
}
pthread_mutex_lock(&mutex);

if(counti == MAXN && countj == MAXM)
{
pthread_cond_signal(&count_threshold_cv);
printf("inc_count: thread %d, neec=%d. threshol reachred. \n",my_id,need[n][m]);
}
printf("inc_count: thread %d, neec=%d. unlocking mutex. \n",my_id,need[n][m]);
pthread_mutex_unlock(&mutex);
sleep(1);
watch_count(q);
}
pthread_exit(NULL);
watch_count(q);

}

void *watch_count(void *r)
{
long my_id=(long)r;
int n,m;
printf("statrt watch count thread %ld\n",my_id);

while(counti < MAXN && countj < MAXM)
{
pthread_mutex_lock(&mutex);
available[n]=max[n][m]-allocation[counti++][countj++];
printf("available=%d\n",available[n]);
pthread_cond_wait(&count_threshold_cv,&mutex);
printf("wach_count: thread %ld, available= %d. conditional signal recv.\n",my_id,available[m]);
countj++;
printf("watch_count: thread %ld , need now = %d.\n",my_id,need[counti][countj]);
}
pthread_mutex_unlock(&mutex);
pthread_exit(NULL);
}











