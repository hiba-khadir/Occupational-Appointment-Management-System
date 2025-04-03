#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Health_system_function.h"





int test(){

    //variables

    typeQueue queue = createQueue();  //main queue
    typeQueue Next_Queue = createQueue();  //to handle > max appointment
    emp *employee_list ;
    char ID[9];

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
        printf("Enter the ID to cancel :");
        scanf("%8s",ID);
        cancel_appointment(&queue,ID);
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
