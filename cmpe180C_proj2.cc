/*
Banker's Algorithm to avoid deadlock and race condition

*/
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<time.h>
#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 3

/* the available amount of each resource */
int available[NUMBER_OF_RESOURCES];

/*the maximum demand of each customer */
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

/* the amount currently allocated to each customer */
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

/* the remaining need of each customer */
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
/*mutex*/
pthread_mutex_t mutex;

/*set maximum demand*/
void setMaximum(int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES]);

/*set allocation*/
void setAllocation(int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES]);

/*calculate Need*/
void calculateNeed();

/*calculate available*/
void calculateAvailable(int temp[NUMBER_OF_RESOURCES]);

/*print out the result*/
void print();

/*check if the request is safe*/
int safetyCheck();


int request_resources(int customer_num, int request[]);

int release_resources(int customer_num, int release[]);

/*prevent race conditions*/
void* thread_control(void* arg);

int main(int argc, char const *argv[]) {
    int i;
    int temp[NUMBER_OF_RESOURCES];
    pthread_t tid[NUMBER_OF_CUSTOMERS];
    pthread_mutex_init(&mutex,NULL);
    for(i=0;i<argc-1;i++)
    {
        temp[i] = atoi(argv[i+1]);
    }
    setAllocation(allocation);
    setMaximum(maximum);
    calculateNeed();
    calculateAvailable(temp);
    print();
    /*create thread*/
    for (i=0; i<NUMBER_OF_CUSTOMERS; i++)
    {

        pthread_create(&tid[i],NULL,thread_control,(void*)(size_t)i);
    }
    /*join thread */
    for (i=0; i<NUMBER_OF_CUSTOMERS; i++)
    {
        pthread_join(tid[i],NULL);
    }
    return 0;
}

void setMaximum(int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES])
{
    printf("Please enter the maximum:\n");
    int i,j;
    for (i=0;i<NUMBER_OF_CUSTOMERS;i++) {
        for (j=0; j<NUMBER_OF_RESOURCES;j++) {
            scanf("%d",&maximum[i][j]);
        }
    }
}

void setAllocation(int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES])
{
    printf("Please enter the allocation:\n");
    int i,j;
    for (i=0;i<NUMBER_OF_CUSTOMERS;i++) {
        for (j=0; j<NUMBER_OF_RESOURCES;j++) {
            scanf("%d",&allocation[i][j]);
        }
    }
}

void calculateNeed()
{
    int i,j;
    for (i=0;i<NUMBER_OF_CUSTOMERS;i++) {
        for (j=0; j<NUMBER_OF_RESOURCES;j++) {
            need[i][j] = maximum[i][j]-allocation[i][j];
        }
    }
}

void calculateAvailable(int temp[NUMBER_OF_RESOURCES])
{
    int i,j;
    int sum[NUMBER_OF_RESOURCES]={0};
    for (i=0;i<NUMBER_OF_RESOURCES;i++) {
        for (j=0; j<NUMBER_OF_CUSTOMERS;j++) {
            sum[i]+=allocation[j][i];
        }

    }
    for (i=0;i<NUMBER_OF_RESOURCES; i++) {
        available[i]=temp[i]-sum[i];
    }
}

void print()
{
    int i,j;
    printf("-----Allocation-----Maximum-----\n");

    for (i=0;i<NUMBER_OF_CUSTOMERS;i++) {
        for (j=0;j<NUMBER_OF_RESOURCES;j++) {
            printf(" %2d",allocation[i][j]);
        }
        printf("     ");
        for (j=0;j<NUMBER_OF_RESOURCES;j++) {
            printf("%2d ",maximum[i][j]);
        }
    printf("\n");
    }
    printf("-----Available-----\n");
    for (i=0; i<NUMBER_OF_RESOURCES; i++) {
        printf(" %2d",available[i]);
    }
    printf("\n");
    printf("-----Need-----\n");
    for (i=0; i<NUMBER_OF_CUSTOMERS; i++) {
        for (j=0; j<NUMBER_OF_RESOURCES; j++) {
            printf(" %2d",need[i][j]);
        }
        printf("\n");
    }
}

int safetyCheck()
{
    int i,j,t;
    int result=1;
    int work[NUMBER_OF_RESOURCES];
    int flag[NUMBER_OF_CUSTOMERS]={0};
    int finish[NUMBER_OF_CUSTOMERS] ={0};
    for (i=0; i<NUMBER_OF_RESOURCES;i++) {
        work[i]=available[i];
    }
    for (t=0; t<NUMBER_OF_CUSTOMERS; t++) {
        for (i=0;i<NUMBER_OF_CUSTOMERS;i++) {
            if(finish[i]!=1)
            {
                for (j=0;(j<NUMBER_OF_RESOURCES);j++) {
                    if(need[i][j]>work[j])
                    {
                        break;
                    }
                    else
                        flag[i]++;


                }
                if(flag[i]==NUMBER_OF_RESOURCES)
                {
                    printf("work is: ");
                    for (j=0; j<NUMBER_OF_RESOURCES; j++) {
                        work[j]+=allocation[i][j];
                        printf(" %d ",work[j]);
                    }
                    printf("\n");
                    finish[i]=1;
                    printf("Process %d is finished!\n",i);
                }
            }
        }
    }
    for (i=0;i<NUMBER_OF_CUSTOMERS;i++) {
        if(finish[i]==0)
        {
            result=0;
            printf("It is unsafe!\n");
            break;
        }
    }
   return result;
}

int request_resources(int customer_num, int request[])
{
    int j,flag;
    flag=0;
    for (j=0; j<NUMBER_OF_RESOURCES; j++) {
        if(request[j]>need[customer_num][j])
        {
            printf("Error! Process has exceeded its maximum claim! Request Denied.\n");
            flag = -1;
            break;
        }
    }
    if (flag == 0) {
        for (j=0;j<NUMBER_OF_RESOURCES;j++) {
            if(request[j]>available[j])
            {
                printf("Resources are not available now,P(%d)must wait.\n",customer_num);
                flag = -1;
            }
        }
        if(flag==0){
            for (j=0; j<NUMBER_OF_RESOURCES; j++) {
                available[j]-=request[j];
                allocation[customer_num][j]+=request[j];
                need[customer_num][j]-=request[j];
            }
            if(safetyCheck()==1)
                printf("It is safe!Resources have been allocated.\n");
            else
            {
                printf("It is unsafe!P(%d) has to wait.\n",customer_num);
                for (j=0; j<NUMBER_OF_RESOURCES; j++) {
                    available[j]+=request[j];
                    allocation[customer_num][j]-=request[j];
                    need[customer_num][j]+=request[j];
                }
                flag=-1;
            }

        }
    }
    return flag;
}

int release_resources(int customer_num, int release[])
{
    int j,flag;
    flag=0;
    for (j=0; j<NUMBER_OF_RESOURCES; j++) {
        if(allocation[customer_num][j]<release[j])
        {
            printf("Error!P(%d)don't have that many resources to release.\n",customer_num);
            flag = -1;
            break;
        }
    }
    for (j=0;j<NUMBER_OF_RESOURCES;j++) {
        if (flag==0) {
            available[j]+=release[j];
            allocation[customer_num][j]-=release[j];
            need[customer_num][j]+=release[j];
        }
    }
    if (flag==0) {
        printf("Yes! Resources have been released.\n");
    }
    else
    {
        printf("Error! Resources cannot be released.\n");
    }
    return flag;

}

void* thread_control(void* arg)
{
    int i = (int)(size_t)arg;
    int j;
    srand((unsigned)time(NULL));
    int lock_ret = 1;
    int request[NUMBER_OF_RESOURCES]={0, 1, 0};
    int release[NUMBER_OF_RESOURCES]={0, 1, 0};
    lock_ret = pthread_mutex_lock(&mutex);
    do{
        if (lock_ret) {
            printf("lock process %d failed...\n",i);
        }
        else
        {
            printf("lock process %d success!\n",i);
        }
    }while (lock_ret);

    printf("process %d request:\n",i);
    for (j=0; j<NUMBER_OF_RESOURCES; j++) {
        printf("%d ",request[j]);
    }
    printf("\n");
    request_resources(i,request);
    print();
    printf("process %d release:\n",i);
    for (j=0; j<NUMBER_OF_RESOURCES; j++) {
        printf("%d ",release[j]);
    }
    printf("\n");
    release_resources(i,release);
    lock_ret = pthread_mutex_unlock(&mutex);
    printf("Unlock process %d success!\n\n",i);
    return NULL;
}
