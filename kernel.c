//kernel.c


int waitProcess[8];
int processActive[8];
int processStackPointer[8];
int currentProcess = 0;
void printString(char*);
void readString(char*);
void readSector(char*, int);
void handleInterrupt21(int, int, int, int);
void readFile(char*,char*,int*);
void printChar(char);
void executeProgram(char*,int*);
void terminate();
void writeSector(char*, int);
void deleteFile(char*);
void writeFile(char*, char*, int);
void handleTimerInterrupt(int,int);
void returnFromTimer(int,int);
int setKernelDataSegment();
void restoreDataSegment(int);
void initializeProgram(int);
void wait(int);

void main(){

int k;
	for(k=0; k<=7; k++){ 
	processActive[k] = 0;
	processStackPointer[k] = 0xff00; 
	
}
	currentProcess = -1;

	


	//char buffer[13312];   //this is the maximum size of a file
	//int sectorsRead;

	//char line[80];
	//char buffer[512];
	//readSector(buffer, 2);
	//printString(buffer);

	makeInterrupt21(); 
	interrupt(0x21,4,"shell",0,0);
	makeTimerInterrupt();

/*
	interrupt(0x21, 3, "messag", buffer, &sectorsRead);   //read the file into buffer
	if (sectorsRead>0)
		interrupt(0x21, 0, buffer, 0, 0);   //print out the file 
	else
		interrupt(0x21, 0, "messag not found\r\n", 0, 0);  //no sectors read? then print an error
	interrupt(0x21, 4, "tstpr2", 0, 0);
*/
	while(1);   //hang up 

}

void handleTimerInterrupt(int segment, int sp){
	int dataseg;
	int i;


	dataseg = setKernelDataSegment();


//------------------------------------------------------------------------


 for(i=0; i<8; i++)
        {
                putInMemory(0xb800,60*2+i*4,i+0x30);
                if(processActive[i]==1)
                        putInMemory(0xb800,60*2+i*4+1,0x20);
                else
                        putInMemory(0xb800,60*2+i*4+1,0);
        }


//-----------------------------------------------------------------------

	if(currentProcess != -1){
		processStackPointer[currentProcess] = sp;
	}
//-----------------------------------------------------------------------

	while(1){

		currentProcess++;

	if(currentProcess == 8){

		currentProcess = 0;

	}

	if(processActive[currentProcess] == 1){
		break;
	}

}

//------------------------------------------------------------------------
	segment = (currentProcess + 2)*0x1000;
	sp = processStackPointer[currentProcess];

	restoreDataSegment(dataseg);	

/*
	printChar('T');
	printChar('i');
	printChar('c');
*/
	returnFromTimer(segment,sp);


}	

void writeFile(char* buffer, char* filename, int numberOfSectors) {
char dir[512];
char map[512];
int x =0;
int y = 0;
int j = 0;
int isit =0;
int free = 0;

readSector(dir,2);
readSector(map,1);


	for(x=0; x<512 ;x=x+32){
		if(dir[x]== 0 ){
		dir[x] =filename[0];
		dir[x+1] =filename[1];
		dir[x+2] =filename[2];
		dir[x+3] =filename[3];
		dir[x+4] =filename[4];
		dir[x+5] =filename[5];
		break;
		}
	}

	         for( j =0; j<numberOfSectors;j++){
	                for( isit=3; y != 0;isit++){
        	                y =map[isit];
                	        free = isit;
               	
			 }
                map[free] = 0xff;
                dir[x+6+j] = free;
        }


	writeSector(dir, 2);
	writeSector(map, 1);
}

void deleteFile(char* filename){
char dir[512];
char map[512];
int x =0;
}



void writeSector(char* buffer, int sector){
        int AH = 3;
        int AL = 1;

        int CH = 0;
        int CL = sector+1;
        int DH = 0;
        int DL = 0x80;

        int AX = AH * 0x100 + AL;
        int CX = CH * 0x100 + CL;
        int DX = DH * 0x100 + DL; 

        interrupt(0x13,AX,buffer,CX,DX);
}


void readFile(char* name, char* buffer, int* sectorsRead){
	int j = 0;
	int y = 6;
	char dir[512];
	*sectorsRead = 0;
	readSector(dir,2);

	for(j = 0; j < 512; j=j+32){
/*
	printChar(dir[j]);
	printChar(dir[j+1]);
	printChar(dir[j+2]);
	printChar(dir[j+3]);
	printChar(dir[j+4]);
	printChar(dir[j+5]);
*/
	if(name[0] == dir[j+0] && name[1] == dir[j+1] && name[2] == dir[j+2] && name[3] == dir[j+3] && name[4] == dir[j+4] && name[5] == dir[j+5]){

		
				
		for(y = j+6; dir[y] !=0; y+=1){
	
			readSector(buffer,dir[y]);
			
			buffer = buffer + 512;
			*sectorsRead = *sectorsRead +1;

			}

		}


	}

}

void executeProgram(char* name, int* pid){
	int dataseg;
	int i;
	int s;
	char in[13312];
	int sectors;


	readFile(name,in,&sectors); 

	dataseg = setKernelDataSegment();
	//if(sectors == 0){return;};


	
	for(s=0; s<8; s++){
	
	if(processActive[s]==0){
		break;
	}
		

}
	if(sectors>0){
	restoreDataSegment(dataseg);
	//freeSegment = (s+2) *0x1000;
	for(i = 0; i<13312; i++){
	putInMemory((s+2)*0x1000,i,in[i]);

	}

	initializeProgram((s+2)*0x1000);

	dataseg = setKernelDataSegment();
	
	processActive[s] = 1;
	processStackPointer[s] = 0xff00;

	restoreDataSegment(dataseg);
	}
	if(pid!=0){
	*pid =s;
}
	

}

void terminate(){
int dataseg;
int j;


	dataseg = setKernelDataSegment();

	processActive[currentProcess]=0;

	for(j=0; j<8; j++){

	if(waitProcess[j]==currentProcess && processActive[j]==2){

		processActive[j]=1;

		}
	}
		restoreDataSegment(dataseg);
		
	//
	
	while(1);

/*
	char shellname[6];	

	shellname[0]='s'; 
	shellname[1]='h'; 
	shellname[2]='e'; 
	shellname[3]='l'; 
	shellname[4]='l';  
	shellname[5]='\0'; 

	executeProgram(shellname);
 */

}

void killProcess(int pid){
int dataseg = 0;

	dataseg = setKernelDataSegment();

	

	if(pid<=7){
		processActive[pid]=0;
		
	}	

	restoreDataSegment(dataseg);


}

void wait(int id){

	int dataseg;

	
	dataseg = setKernelDataSegment();

	processActive[currentProcess] = 2;
	waitProcess[currentProcess] = id;
	while(processActive[currentProcess] == 2){}

	restoreDataSegment(dataseg);


}

void handleInterrupt21(int ax, int bx, int cx, int dx){
//printString("Hello1\0");

	if(ax==0){
	printString(bx);
		}

	else if(ax == 1){
	readString(bx);
		}
	
	else if(ax == 2){
	//interrupt(0x10,0xe*0x100+'\n',0x0,0x0,0x0);
	readSector(bx,cx);
		}
	else if(ax==3){
	readFile(bx,cx,dx);
	}

	else if(ax==4){
	executeProgram(bx,cx);
	}
	else if(ax==5){
	terminate();
	}
	else if(ax==6){
	writeSector(bx,cx);
	}
	else if(ax==7){
	deleteFile(bx);
        }
	else if(ax ==8){
	writeFile(bx,cx,dx); 
	}

	else if(ax ==9){
	killProcess(bx); 
	}

	else if(ax ==10){
	printChar(bx); 
	}

	else if(ax ==11){
	wait(bx); 
	}

	else{

	printString("Error\0");

	} 
}

void readSector(char* buffer, int sector){

	int AH = 2;
	int AL = 1;

	int CH = 0;
	int CL = sector+1;
	int DH = 0;
	int DL = 0x80;

	int AX = AH * 0x100 + AL;
	int CX = CH * 0x100 + CL;
	int DX = DH * 0x100 + DL; 

	interrupt(0x13,AX,buffer,CX,DX);

} 

void readString(char* line){

	char start = 0x0;
	int j = 0;

	while(start != 0xd){

	start = interrupt(0x16,0x0,0x0,0x0,0x0);

	*(line + j) = start;

	if (start == 0xd && j > 0x0) {

	interrupt(0x10,0xe*0x100+'\n',0x0,0x0,0x0);
	interrupt(0x10,0xe*0x100+start,0x0,0x0,0x0);
     	j++;
	
	}

	else if (start == '\b' && j > 0x0) {
     		j--;
	interrupt(0x10,0xe*0x100+'\b',0x0,0x0,0x0);
	interrupt(0x10,0xe*0x100+' ',0x0,0x0,0x0);
	interrupt(0x10,0xe*0x100+'\b',0x0,0x0,0x0);
		}
	else if(start != '\b'){

    	interrupt(0x10,0xe*0x100+start,0x0,0x0,0x0);
     	j++;

		}

	
	}	

  	*(line + j) = 0x0;
	
}

void printString(char* letters){
int i = 0;

	while(letters[i] !='\0'){
	printChar(letters[i]);
	i++;
	
}


/*
	int count = 0;
	while(*(letters + count)!= '\0'){
	
	char al = *(letters + count);
	char ah = 0xe;
	int ax = ah * 256 + al;
	
	interrupt(0x10, ax, 0x0, 0x0, 0x0);
	count++;
	}
*/

}
void printChar(char chars){
	char items = chars;

	interrupt(0x10,0xe*0x100+items,0x0,0x0,0x0);
}
