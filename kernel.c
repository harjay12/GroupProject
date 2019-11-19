//kernel.c

void printString(char*);
void readString(char*);
void readSector(char*, int);
void handleInterrupt21(int, int, int, int);

void main(){
/*
	char line[80];
	
	readString(line);
	printString(line);

	char buffer[512];
	readSector(buffer,30);
	printString(buffer);
*/


	char line[80];
	char buffer[512];
	printString("Enter a line: ");

	readSector(buffer,30);
	
	makeInterrupt21();

	interrupt(0x21,1,line,0x0,0x0);
	interrupt(0x21,0,line,0x0,0x0);
	interrupt(0x21,2,line,0x0,0x0);
	printString(buffer);
	
	while(1);

}

void handleInterrupt21(int ax, int bx, int cx, int dx){

	if(ax==0){
	printString(bx);
		}

	else if(ax == 1){
	readString(bx);
		}
	
	else if(ax == 2){
	interrupt(0x10,0xe*0x100+'\n',0x0,0x0,0x0);
	readSector(bx,cx);
		}

	else{

	readString("Error ",'\0');

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

	int count = 0;

	while(*(letters + count)!= '\0'){
	
	char al = *(letters + count);
	char ah = 0xe;
	int ax = ah * 256 + al;
	
	interrupt(0x10, ax, 0x0, 0x0, 0x0);
	count++;

	}

}
