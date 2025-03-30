#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 
#include "queue_ops.h"




int main(){


    //variables 

    typeQueue queue = createQueue();  //main queue
    typeQueue Next_Queue = createQueue();  //to handle > max appointment

    consultation appointement ;


    FILE  *cons_file_in =  fopen("Consultations.txt","r");  
    if (!cons_file_in)
    {
        printf("Error cannot open file \n");
        return 1 ;
    }
    else
    {
        read_file_to_queue(cons_file_in,&queue);      
        add_appointment(&queue,&Next_Queue);
        reschedule_manual(&queue,&Next_Queue);
        display_queue(queue);

    }
    
    fclose(cons_file_in);

    FILE *cons_file_out = fopen("output.txt","w");
    if (!cons_file_out)
    {
        printf("Error cannot open file \n");
        return 1 ;
    }
    else
    {
        write_queue_to_file(cons_file_out,Next_Queue);

    }
    fclose(cons_file_out);
    

    return 0 ;
}