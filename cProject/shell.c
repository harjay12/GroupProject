
main(){
	char line[80];
	char command[10];
	char buffer[13312];
	char ldirectory[512];
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

	if((line[0]=='t' && line[1]=='y' && line[2]=='p' && line[3]=='e') || (line[0]=='e' && line[1]=='x' && line[2]=='e'&& line[3]=='c') || (line[0]=='d' && line[1]=='i' && line[2]=='r') || (line[0]=='d' && line[1]=='e' && line[2]=='l' && line[3]=='e') || (line[0]=='w' && line[1]=='r' && line[2]=='i' && line[3]=='t')){

		while(line[i] != '\0'){

		command[j]=line[i] ;
		i++;
		j++;
		}

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
	syscall(0,"TEST3\n\r");
	syscall(8,"hello this is it","Rayans",1);
}
//-------------------------------------------------------------------
}
//--------------------------------------------------------

	else{
		syscall(0, "ER0R 404 COMMAND NOT FOUND\n\r");
	}
}

