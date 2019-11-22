
#include <stdio.h>
#define SIZE 5
int queue[SIZE];
int first =0;
int last =0
int itemsInQueue = 0;
pthread_mutex_t queuelock

void putInQueue(int item){
pthread_mutex_lock(&queuelock);
if(itemsInQueue >= size){

printf"FUll");
}
else{
queue[first] = item;
first = (first+1)%SIZE;
itemsInQueue++;
}
pthread_mutex_unlock(&queuelock);

}

int takeFromQueue(){
pthread_mutex_lock(&queuelock);
if(itemsInQueue <= 0){
printf("EMPTY!");
}
else{
item=queue[last];
last=(last+1)%SIZE;
itemsInQueue--;
}
pthread_mutex_unlock(&queuelock);
return item;
}

void* producer(){
int i;
for(i=1; ;i++){
	 printf("Producing %d\n",i);
	while(itemsInQueue==SIZE){
	}
	putInQueue(i);
	usleep(rand()%20000000);
}
}

void* consumer(){
for( ; ; ){
	printf("Consuming %d\n",i);
	takeFromQueue();
	usleep(rand()%2000000);
}
}
int main(){

// gcc -=thread -opthre
pthread_t prodthread,consthread;
srand(time(0));


}
