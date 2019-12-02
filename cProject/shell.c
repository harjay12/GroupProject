
main(){
	char line[80];
	char command[20];
	char buffer[13312];
	char file[80];
	char file2[80];
	char dir[512];
	char map[512];
	int sectorsRead;
	int i = 5;
	int j = 0;
	int x = 0;
	int y = 1;
	int isit =0;

	syscall(0,"<Arbitor> ");
	syscall(1,line);

	if((line[0]=='t' && line[1]=='y' && line[2]=='p' && line[3]=='e') || (line[0]=='e' && line[1]=='x' && line[2]=='e'&& line[3]=='c') || (line[0]=='d' && line[1]=='i' && line[2]=='r') || (line[0]=='d' && line[1]=='e' && line[2]=='l' && line[3]=='e') || (line[0]=='w' && line[1]=='r' && line[2]=='i' && line[3]=='t') || (line[0]=='c' && line[1]=='o' && line[2]=='p' && line[3]=='y') || (line[0]=='c' && line[1]=='r' && line[2]=='e' && line[3]=='a') || (line[0] == 'h' && line[1] == 'e' && line[2] == 'l' && line[3] == 'p')){

		while(line[i] != '\0'){

		command[j]=line[i];
		i++;
		j++;
		}
//------------------------------------------------------
	i=0;
        j=0;
         while(line[i+5] != ' '){

                file[j]=line[i+5] ;
                i++;
                j++;
                }
	file[j+1] = '\0';
        j=0;
         while(line[i+6] != '\0'){

                file2[j]=line[i+6] ;
                i++;
                j++;
                }
	//file2[j+1] = '\0';



//---------------------------------------------------
		if(line[0]=='t' && line[1]=='y' && line[2]=='p' && line[3]=='e'){

			syscall( 3, command, buffer, &sectorsRead);   //read the file into buffer

			if (sectorsRead>0){
				syscall(0, buffer);
				syscall(0," \r");   //print out the file 
			}
			else {
				syscall(0,"file not found\n\r");
			}
		}

//---------------------------------------------------------------
		else if(line[0]=='e' && line[1]=='x' && line[2]=='e' && line[3]=='c'){
 

			syscall( 3, command, buffer, &sectorsRead);   //read the file into buffer

			if (sectorsRead>0){
			syscall(4, command);
			}

			else{
				syscall(0, "EXEC file not found\n\r");
			}
		}
//-------------------------------------------------------------------------
		 else if(line[0]=='d' && line[1]=='i' && line[2]=='r'){
			syscall(2,dir,2);
			for(x=0; x<512 ;x=x+32){
			if(dir[x+y]!=0 ){
				dir[x+6]='\0';
				syscall(0, &dir[x]);
                                         syscall(0, "\n\r");
			}
			}
                }

//---------------------------------------------------------------------
	else if(line[0]=='d' && line[1]=='e' && line[2]=='l' && line[3]=='e'){
        	        syscall( 3, command, buffer, &sectorsRead);   //read the file into buffer
			syscall(2,dir,2);
                        syscall(2,map,1);
			if (sectorsRead>0){
	               // syscall(7, command);

			syscall(0, "File was DELETED\n\r");
			}
			for(x=0; x<512 ;x=x+32){
			        if(dir[x]!=0 ){

					if(command[0] == dir[x] && command[1] == dir[x+1] && command[2] == dir[x+2] && command[3] == dir[x+3] && command[4] == dir[x+4] && command[5] == dir[x+5]){
              					
						dir[x] = '\0';
        				        syscall(0,&dir[x]);
						syscall(6,dir,2);
						for(isit = 0;y!='\0';isit++){
						y=dir[x+6+isit];
						map[y]='\0';
						}
					syscall(6,map,1);
               				 }
       				   }
			 }

                	}

//-------------------------------------------------------------------
 else if(line[0]=='w' && line[1]=='r' && line[2]=='i' && line[3]=='t'){
	syscall(0,"File Created\n\r");
	syscall(8,"hello this is it\n\r",file,1);
}
//-------------------------------------------------------------------
 else if(line[0]=='c' && line[1]=='o' && line[2]=='p' && line[3]=='y'){
syscall( 3, command, buffer, &sectorsRead);

 			if (sectorsRead>0){
 			syscall(8,buffer,file2,1);
                        }
                        else {
                                syscall(0,"Source file not found\n\r");
                        }

}
//---------------------------------------------------------------------
else if(line[0]=='c' && line[1]=='r' && line[2]=='e' && line[3]=='a'){
	i =0;
	j =0;
	while(line[0] != '\r'){
		buffer[i+1] = '\n';
		buffer[i+2] = '\r';
		syscall(0,"Enter next line: \n\r");
		syscall(1,line);
		j =0;
		while(line[j] != '\0'){
		buffer[i] =line[j];
		i++;
		j++;
		}
	}
	 syscall(8,buffer,file,1);
}
//------------------------------------------------------
else if(line[0]=='h' && line[1]=='e' && line[2]=='l' && line[3]=='p'){
syscall(0,"The functions you can use are: \n\r");
syscall(0,"type 'FILENAME' - this prints out the contents of 'FILENAME'\n\r");
syscall(0,"exec 'FILENAME' - this executes 'FILENAME' \n\r");
syscall(0,"dir - this prints out the file directory \n\r");
syscall(0,"dele 'FILENAME' - deletes 'FILENAME' \n\r");
syscall(0,"writ 'FILENAME' - writes 'FILENAME' \n\r");
syscall(0,"copy 'FILENAME1' 'FILENAME2' - copys the contents of 'FILENAME1' to 'FILENAME2'(creates a new file called 'FILENAME2') \n\r");
syscall(0,"crea 'FILENAME' - creates a text file with the name 'FILENAME' and allows you to write into the file \n\r");


}
//------------------------------------------------------
}
//--------------------------------------------------------

	else{
		syscall(0, "ER0R 404 COMMAND NOT FOUND\n\r");
	}
}

