#include "lists_ops.h"
#include <stdio.h>
#include <stdlib.h>

/*contains the linked lists abstract machine and related implemented functions*/



/*------------------TYPE DEFINITIONS------------------------------ */


/*structure of a linked lists cell will be used  as a queue*/
/*type of an element in the list*/
typedef struct cell cell;     


struct cell {     
    consultation conslt;
    cell *addr;
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
    reason Consultation_Reason ;     /*takes one of the reasons in the reason enum type*/
 };
 








 
/*-------------------------the linked lists model implementation----------------------------*/
void Allocate(cell **p){
    *p = malloc(sizeof(cell));
}

cell* Next(cell *p){
    return p->addr ;
}

void Ass_val(cell *p , int val){
    p->value = val ;
}

void Ass_addr(cell *p , cell *q){
    p->addr = q ;
}

int value(cell *p){
    return p->value ;
}







/*implemented functions*/