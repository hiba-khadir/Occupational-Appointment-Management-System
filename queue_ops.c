#include <stdio.h>
#include <stdlib.h>
#include "queue_ops.h"




/*---------------------------------TYPE DEFINITIONS------------------------------*/



/*structure of a linked lists cell will be used  as a queue*/
/*type of an element in the list*/
typedef struct cell cell;     

struct cell {          
    consultation conslt;
    cell *addr;
};



/*dynamic implementation of the queue*/
typedef struct typeQueue typeQueue ;  /*define queue as a type */

struct typeQueue
{
    cell *h ;            /*pointer to the head of the queue*/
    cell *t ;            /*pointer to the head of the queue*/
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

void Allocate(cell **p){
    *p = malloc(sizeof(cell));
}

cell* Next(cell *p){
    return p->addr ;
}


void Ass_consultation(cell *p , consultation c){
    (p->conslt).
}


void Ass_addr(cell *p , cell *q){
    p->addr = q ;
}

/*
int value(cell *p){
    return p->value ;
}*/


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
    cell *new ; 
    Allocate(new);                     /*allocate a new cell in the queue (list)*/
    new->conslt = new_conslt ;         /*assign the consultation*/


    if (!emptyQueue(*Q))   
    {   
        Ass_addr((*Q).t , new);         /*link new to tail*/
        (*Q).t = (*Q).t -> addr ;       /*move the tail one element left*/
        
    }
    
} 


