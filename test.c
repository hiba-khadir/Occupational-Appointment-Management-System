#include <stdio.h>
#include <stdlib.h>

/*reads data from text file given by a pointer into a queue*/


void read_file_to_queue(FILE *file){
    char name[50] , time[5] , reason[21];
    int ID ;
    char line[95] ;

    if (fgets(line,95,file) == NULL)   /*reading failed*/
    {
        fclose(file);
        printf("incorrect line format .") ;
    }
    
    else{
        while (!feof(file) && fgets(line,95,file))         /*fgets not NULL -stops at the EOF*/
        {
            
            if (sscanf(line,"%d;%49[^;];%4[^;];%20[^\r\n]\r\n",&ID,name,time,reason) == 4 )  /*ensure reading 4 fileds*/
            {
                

                printf("ID : %d , name : %s , time: %s , reason : %s  " ,ID,name,time,reason);
                printf("\n");
            }
            else
            {
                printf("DEBUGGING : %s\n",line);
                printf("Invalid line format \n");
            }
            
        }
    }
    

}


int main(){
    FILE  *cons_file =  fopen("Consultations.txt","r");  /*open the file for reading. define a pointer */
    if (cons_file == NULL)
    {
        printf("Can't open file error!");    
        return 1;    
    }
    else{
        read_file_to_queue(cons_file);
    }

    fclose(cons_file);
    return 0 ;
}