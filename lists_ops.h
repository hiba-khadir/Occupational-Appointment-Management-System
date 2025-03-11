#ifndef lists_ops
#define lists_ops


/*type definition : structure of a linked lists cell*/

typedef struct cell cell;     /*type of an element in the list*/

struct cell {     
    char id[8];
    int consult_num;
    int last_consult[8];
    int return_work[8];
    cell *addr;
};
/* lists tae history and full name mzl */


/*linked list abstract machine definitions  { Allocate , Free , Ass_val , Ass_adr , Value , Next }*/

/*1*/
void Allocate(cell **p);   /*allocate a memory space and returns its address in p */
/*2*/
cell* Next(cell* p);     /*returns a pointer to the next cell (after p)*/
/*3*/
void Ass_val(cell *p , int val);  /*assigns the value val to the cell pointed by p*/
/*4*/
void Ass_addr(cell *p , cell *q);  /*assigns the value val to the cell pointed by p*/
/*5*/
int value(cell *p);                /*returns the value in the cell pointed by p*/




/*queue abstract machine definition */





#endif
