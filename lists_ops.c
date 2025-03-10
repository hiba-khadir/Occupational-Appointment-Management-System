#include "lists_ops.h"
#include <stdio.h>
#include <stdlib.h>

/*contains the linked lists abstract machine and related implemented functions*/



/*------------------TYPE DEFINITIONS------------------------------ */

/*structure of a linked lists cell*/
typedef struct cell cell;     

struct cell {              /*here and in the model replace the "int value " with the tpe employee (maram)*/
    int value;
    cell *addr;
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