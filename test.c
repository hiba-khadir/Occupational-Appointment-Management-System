#include <stdio.h>
#include <stdlib.h>

/*reads data from text file given by a pointer into a queue*/


void read_file_to_queue(FILE *file){
    char name[50] , time[5] , reason[21];
    int ID ;
    while (fscanf(file,"%d;%49[^;];%4[^;];%20[^\n]",&ID,&name,&time,&reason) == 4)
    {
        printf("ID : %d , name : %s , time: %s , reason : %s  " ,ID,name,time,reason);
        printf("\n");
    }
    
}


int main(){
    FILE  *cons_file =  fopen("Consultations.txt","r");  /*open the file for reading. define a pointer */
    if (cons_file == NULL)
    {
        printf("Can't open file error!");        
    }
    else{
        read_file_to_queue(cons_file);
    }

    
}