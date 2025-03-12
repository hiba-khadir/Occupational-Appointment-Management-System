#include "lists_ops.h"
#include <stdio.h>
#include <stdlib.h>

/*contains the linked lists abstract machine and related implemented functions*/



/*------------------TYPE DEFINITIONS------------------------------ */

/*structure of a linked lists elmnt*/
typedef struct elmnt elmnt;     

struct elmnt {              /*here and in the model replace the "int value " with the tpe employee (maram)*/
    int value;              /*btw this is named elmnt because i already named cell name it what you want*/
    elmnt *addr;
};





/*-------------------------the linked lists model implementation----------------------------*/
void Allocate(elmnt **p){
    *p = malloc(sizeof(elmnt));
}

elmnt* Next(elmnt *p){
    return p->addr ;
}

void Ass_val(elmnt *p , int val){
    p->value = val ;
}

void Ass_addr(elmnt *p , elmnt *q){
    p->addr = q ;
}

int value(elmnt *p){
    return p->value ;
}







/*implemented functions*/