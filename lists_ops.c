#include "lists_ops.h"
#include <stdio.h>
#include <stdlib.h>

/*contains the linked lists abstract machine and related implemented functions*/

/*type definition : structure of a linked lists cell*/

typedef struct cell cell;     /*type of an element in the list*/

struct cell {     
    int value;
    cell *addr;
};




/*the linked lists model implementation*/
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