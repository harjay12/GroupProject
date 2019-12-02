//kernel.c

void printString(char*);
void readString(char*);
void readSector(char*, int);
void handleInterrupt21(int, int, int, int);
void readFile(char*,char*,int*);
void printChar(char);
void executeProgram(char*);
void terminate();
void writeSector(char*, int);
void deleteFile(char*);
void writeFile(char*, char*, int);


void main(){

	//char buffer[13312];   //this is the maximum size of a file
	//int sectorsRead;

	//char line[80];
	//char buffer[512];
	//readSector(buffer, 2);
	//printString(buffer);
while(1){
	makeInterrupt21(); 
	interrupt(0x21,4,"shell",0,0);
}

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

void writeFile(char* buffer, char* filename, int numberOfSectors) {
char dir[512];
char map[512];
char whole[512];
int x =0;
int y = 0;
int j = 0;
int isit =0;
int t = 0;

for( t = 0;t<512;t++){
whole[t] =buffer[t];
}

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
		 y =3;
	         for( j =0; j<numberOfSectors;j++){
	               while(map[y] != 0x0){
			y++;
			}
                map[y] = 0xff;
                dir[x+6+j] = y;
		writeSector(whole,y);
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

void executeProgram(char* name){

	int i;
	char in[13312];
	int sectors;

	readFile(name,in,&sectors); 
	if(sectors == 0){return;}
	

	for(i = 0; i<13312; i++){
	putInMemory(0x2000,i,in[i]);

	}
	launchProgram(0x2000);

}

void terminate(){
	//printChar('H');

	char shellname[6];
	
	

	shellname[0]='s'; 
	shellname[1]='h'; 
	shellname[2]='e'; 
	shellname[3]='l'; 
	shellname[4]='l';  
	shellname[5]='\0'; 
	executeProgram(shellname);
 
	

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
	executeProgram(bx);
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
