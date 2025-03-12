#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue_ops.h"
#include "linked_lists_op.h"




/*---------------------------------TYPE DEFINITIONS------------------------------*/



/*structure of a linked lists cell will be used  as a queue of consultations inserted based on priority*/

typedef struct cell typeCell;   /*type of an element in the list*/   

struct cell {          
    consultation conslt;            
    typeCell *addr;                 /*address of next*/
};



/*dynamic implementation of the queue*/
typedef struct typeQueue typeQueue ;  /*define queue as a type */

struct typeQueue
{
    typeCell *h ;                   /*pointer to the head of the queue*/
    typeCell *t ;                   /*pointer to the head of the queue*/
}; 





/*Consultation type definitin - elements inside the queue -*/

/* define a type of consultation reasons each */
typedef enum {
    work_accident,
    occupational_disease,
    return_to_work,
    pre_employment,
    periodic_examination
}  reason;



typedef struct consultation consultation ;
 struct consultation
 {
    char Employee_ID[8];
    char Employee_Name[50];            
    char Consultation_Time[5];         /*in the format HH:MM*/
    reason Consultation_Reason ;     /* one of the reasons in reason type*/
 };
 
/*---------------------------------------------------------------------------------*/




/*-------------------------the linked lists model implementation----------------------------*/
/*same linked list model , the type of the element in the cell is a consultation*/

void Allocate(typeCell **p){
    *p = malloc(sizeof(typeCell));
}

typeCell* Next(typeCell *p){
    return p->addr ;
}


void Ass_consultation(typeCell **k , consultation c){
    int i , j ;         /*to iterate through strings*/

    /*copy each field from c consultation to the cell */
    (*k)->conslt.Consultation_Reason = c.Consultation_Reason ;
    strcpy((*k)->conslt.Employee_Name, c.Employee_Name);
    strcpy((*k)->conslt.Consultation_Time , c.Consultation_Time);
    strcpy((*k)->conslt.Employee_ID, c.Employee_ID ) ;
    
}


void Ass_addr(typeCell **p , typeCell *q){
    (*p)->addr = q ;

}




void display_consultation(typeCell *k){
    char *name = k->conslt.Employee_Name ; 
    
    printf("Employee ID : %s \n", k->conslt.Employee_ID);

    printf("Employee Name : ");

    while(*name != '\0'){            /*display the name letter by letter*/
        printf("%c",*name);
        name++ ;
    }
    printf("\n");

    printf("Consultation Time : %s \n", k->conslt.Consultation_Time);
    printf("Concultation Reason : %s" , k->conslt.Consultation_Reason);

}


/*------------------------------------------------------------------------------------------------*/


/*-----------------------------------The Queue Model Implementation ------------------------------*/
/*this operations are restricted to the queue of consultations */

typeQueue createQueue(){
    typeQueue Q ;
    Q.h =  NULL;
    Q.t =  NULL;            /*initialize the queue as empty */

    return  Q ;
}

int emptyQueue(typeQueue Q){
    return (Q.h == NULL) ;       /*returns 1 if the list is empty and 0 otherwise*/
}


void enqueue(typeQueue *Q , consultation new_conslt){   /*inserts based on priority ( the queue is ordered)*/
    typeCell *new ;
    Ass_addr(new,NULL);
    Ass_consultation(new,new_conslt);

    int priority = new_conslt.Consultation_Reason ; /*reason is stored as an integer can be used for priority*/
    
    //insertion
    if (!emptyQueue)
    {
        if (priority > Q->h->conslt.Consultation_Reason)
        {
            
        }
        
        
    }
    else     /*new is the first element inserted*/
    {
       Q->h = new ;
       Q->t = new ;  
    }
    
    





    
} 


void dequeue(typeQueue *Q , consultation *dequeued_conslt ){
    typeCell *temp ;

    if (!emptyQueue(*Q))
    {
        temp =  (*Q).h ;  /*temporary save for the head*/


        /*copy each field from the head  consultation to the dequeued consultation*/
        (*dequeued_conslt).Consultation_Reason = Q->h->conslt.Consultation_Reason ;
        strcpy((*dequeued_conslt).Employee_Name , Q->h->conslt.Employee_Name);
        strcpy((*dequeued_conslt).Consultation_Time , Q->h->conslt.Consultation_Time);
        strcpy((*dequeued_conslt).Employee_ID, Q->h->conslt.Employee_ID ) ;
    
        
        Q->h = Next(Q->h);   /*move headto next */
        free(temp);

    }
    else {
        printf("ERROR : QUEUE IS EMPTY CANNOT DEQUEUE \n");
    }

}





