#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* func(void* message){
char* line = (char*)message;
for(int i = 0; i<100000; i++){
	printf("%d %s",i,line);
	usleep(1000000);
}
}

int main(){


//this holds the data for the thread(making the thread)
pthread_t thread1;
pthread_t thread2;

//this starts the thread so it runs
//pthread_create(Thread Object, attr, function, arguments)
pthread_create(&thread1,NULL, func, (void*)"hello from thread1\n");
pthread_create(&thread2,NULL, func, (void*)"HELLO FROM THREAD2\n");

//dont let main thread  end before the func thread finishes but is back uses up alot of cpu
//while(1);

pthread_join(thread1, NULL); // sleep until thread finishes.
pthread_join(thread2, NULL);
return 0;
}
