#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue_ops.h"





/*---------------------------------TYPE DEFINITIONS------------------------------*/



/*structure of a linked lists cell will be used  as a queue of consultations inserted based on priority*/

typedef struct cell typeCell;   /*type of an element in the list*/   

struct cell {          
    consultation conslt;            
    typeCell *addr;                 /*address of next*/
};



/*dynamic implementation of the queue*/
typedef struct typeQueue typeQueue ;       /*define queue as a type */

struct typeQueue
{
    typeCell *h ;                          /*pointer to the head of the queue*/
    typeCell *t ;                          /*pointer to the head of the queue*/
}; 





/*Consultation type definitin - elements inside the queue -*/

/* define a type of consultation reasons ordered by priority 
typedef enum {
    work_accident,
    occupational_disease,
    return_to_work,
    pre_employment,
    periodic_examination
}  reason;

*/

typedef struct consultation consultation ;
 struct consultation
 {
    char Employee_ID[8];
    char Employee_Name[50];            
    char Consultation_Time[5];         /*in the format HH:MM*/
    char Consultation_Reason[21] ;   
    int priority ;
 };
 
/*---------------------------------------------------------------------------------*/






/*-------------------------the linked lists model implementation for priority queue----------------------------*/
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
    strcpy((*k)->conslt.Consultation_Reason, c.Consultation_Reason);    
    strcpy((*k)->conslt.Employee_Name, c.Employee_Name);
    strcpy((*k)->conslt.Consultation_Time , c.Consultation_Time);
    strcpy((*k)->conslt.Employee_ID, c.Employee_ID ) ;
    (*k)->conslt.priority = reason_priority(c.Consultation_Reason);
    
}


void Ass_addr(typeCell **p , typeCell *q){
    (*p)->addr = q ;

}




void display_consultation(typeCell *k){
    char *name = k->conslt.Employee_Name ; 
    printf("Employee's ID : %s \n",k->conslt.Employee_ID);
    printf("Employee's name : %s \n",k->conslt.Employee_Name);
    printf("Consultation Time : %s \n", k->conslt.Consultation_Time);
    printf("Concultation Reason : %s \n" , k->conslt.Consultation_Reason);
    printf("Priority : %d",k->conslt.priority);

}

consultation consultation_info(typeCell *k){
    consultation c ;

    strcpy(c.Consultation_Reason , k->conslt.Consultation_Reason) ;
    strcpy(c.Employee_Name , k->conslt.Employee_Name);
    strcpy(c.Consultation_Time , k->conslt.Consultation_Time);
    strcpy(c.Employee_ID, k->conslt.Employee_ID ) ;
    c.priority = k->conslt.priority;

    return c ;
}

/*-----------------------------------------------------------------------------------------------*/







/*-------------------------some needed list  and queues functions----------------------------------*/


int reason_priority(char reason[21]){ /*returns the priority of consultation reasons from 1 to 3 . returns -1 for invalid reasons*/

    if(strcmp(reason,"work_accident") == 0 ){       /*compare the reason*/
        return 3 ;
    }          
    else if (strcmp(reason,"occupational_disease") == 0)
    {
        return 2 ;
    }
    else if (strcmp(reason,"pre-employement_visit") == 0 || strcmp(reason,"return_to_work") == 0)
    {
        return 1 ;
    }
    else{
        return -1 ;
    }
 
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
    //variables
    typeCell *new ;
    typeCell *a = Q->h  ,*b=NULL ;             /*a and b are used to traverse the queue*/


    //initialization
    Allocate(new);
    Ass_addr(new,NULL);
    Ass_consultation(new,new_conslt);         

    
    //insertion
    if (!emptyQueue(*Q))
    {
        //find isert position
        while (a != NULL &&   new->conslt.priority <= Q->h->conslt.priority )
        {
            b = a ;                      /* b is the previous of a */
            a = Next(a);
        
        }

        //linking
        if (b == NULL)                   
        {
            new->addr = Q->h ;  ;          
            Q->h = new  ; 
             
        }
        else                             /*list is not empty*/
        {
            Ass_addr(&new,a); 
            Ass_addr(&b,new);      
            
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
        temp =  (*Q).h ;                      /*temporary save for the head*
         /*copy each field from the head  consultation to the dequeued consultation*/ 
        *dequeued_conslt = consultation_info(Q->h);
        Q->h = Next(Q->h);                        /*move head to next */
        free(temp);                               /*free the dequeued element*/

    }
    else {
        printf("ERROR : QUEUE IS EMPTY. CANNOT DEQUEUE \n");
    }

}





