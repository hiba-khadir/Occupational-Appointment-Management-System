#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue_ops.h"




/*---------------------------------TYPE DEFINITIONS------------------------------*/



/*structure of a linked lists cell will be used  as a queue*/

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

/* define a type of consultation reasons */
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
    char *Employee_Name;            /*will be allocated dynamically to handle names with diffrent length */
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
    (*k)->conslt.Employee_Name = c.Employee_Name ;

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


void enqueue(typeQueue *Q , consultation new_conslt){
    typeCell *new ; 


    Allocate(&new);                               /*allocate a new cell in the queue (list)*/
    Ass_consultation(&new , new_conslt);          /*assign the consultation*/
    Ass_addr(&new ,NULL);                         /*new is at the tail*/



    if (!emptyQueue(*Q))   
    {   
        Ass_addr((*Q).t , new);         /*link new to tail*/
        (*Q).t = (*Q).t -> addr ;       /*move the tail one element left*/
        
    }
    else       /*new is the first in the queue*/
    {
        Q->h = new ;
        Q->t = new ; 
    }
    
    
} 
void dequeue(typeQueue *Q , consultation *dequeued_conslt ){
    
}


