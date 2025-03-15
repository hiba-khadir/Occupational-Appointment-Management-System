#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue_ops.h"





/*---------------------------------TYPE DEFINITIONS------------------------------*/



/*structure of a linked lists cell will be used  as a queue of consultations inserted based on priority*/

typedef struct consultation consultation ;
 struct consultation
 {
    char Employee_ID[9];
    char Employee_Name[50];            
    char Consultation_Time[6];         /*in the format HH:MM*/
    char Consultation_Reason[21] ;   
    int priority ;
 };



typedef struct cell typeCell;           /*type of an element in the list*/   

struct cell {          
    consultation conslt;                 
    typeCell *addr;                      /*address of next*/
};



/*dynamic implementation of the queue*/


struct typeQueue
{
    typeCell *h ;                          /*pointer to the head of the queue*/
    typeCell *t ;                          /*pointer to the head of the queue*/
}; 
typedef struct typeQueue typeQueue ;       /*define queue as a type */




 
/*---------------------------------------------------------------------------------*/


/*-------------------------the linked lists model implementation for priority queue----------------------------*/
/*same linked list model , the type of the element in the cell is a consultation*/

void Allocate(typeCell **p){
    *p = malloc(sizeof(typeCell));
}

typeCell* Next(typeCell *p){
    return p->addr ;
}


/*remember to fix this later to manage upper and lower case :) */
int reason_priority(char reason[21]){ 

    if(strcmp(reason,"Work-accident") == 0 ){       /*compare the reason*/
        return 4 ;
    }          
    else if (strcmp(reason,"Occupational-disease") == 0)
    {
        return 3;
    }
    else if (strcmp(reason,"Pre-employement") == 0 || strcmp(reason,"Return-to-Work") == 0)
    {
        return 2 ;
    }
    else if (strcmp(reason,"Periodic") == 0 )
    {
        return 1 ;
    }
    
    else{
        return -1 ;
    }
 
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

consultation consultation_info(typeCell *k){
    consultation c ;

    strcpy(c.Consultation_Reason , k->conslt.Consultation_Reason) ;
    strcpy(c.Employee_Name , k->conslt.Employee_Name);
    strcpy(c.Consultation_Time , k->conslt.Consultation_Time);
    strcpy(c.Employee_ID, k->conslt.Employee_ID ) ;
    c.priority = k->conslt.priority;

    return c ;
}


void Ass_addr(typeCell **p , typeCell *q){
    (*p)->addr = q ;

}
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
    typeCell *a = Q->h ,*b=NULL ;             /*a and b are used to traverse the queue*/


    //initialization
    Allocate(&new);
    Ass_addr(&new,NULL);
    Ass_consultation(&new,new_conslt);         

    
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



/*reads data from text file given by a pointer into a queue*/    
void read_file_to_queue(FILE *file,typeQueue *Q){
    
    char line[90];
    int fields ;
    consultation temp , result ;   /*store te retrieved data temporarly*/
    

    while (fgets(line,sizeof(line),file) != NULL)         /*fgets not NULL -stops at the EOF*/
        {
            
            if (fields = sscanf(line,"%8[^;];%49[^;];%5[^;];%20[^\n]\n",temp.Employee_ID,temp.Employee_Name,temp.Consultation_Time,temp.Consultation_Reason ) == 4 )  /*ensure reading 4 fileds*/
            {
        
                printf("ID : %s,name : %s,time: %s,reason : %s  " ,temp.Employee_ID,temp.Employee_Name,temp.Consultation_Time,temp.Consultation_Reason );
            
                enqueue(Q,temp);
                printf(" , priority : %d",result.priority);
                printf("\n");
            }
           else
            {
                printf("DEBUGGING : %s\n",line);
                printf("Invalid line format \n");
                printf("%d",fields);   /*what field is missing */
            }
            
        }
    }



int main(){
    typeQueue queue = createQueue();
    FILE  *cons_file =  fopen("Consultations.txt","r");  /*open the file for reading. define a pointer */

    if (cons_file == NULL)
    {
        printf("Can't open file error!");    
        return 1;    
    }
    else{
        read_file_to_queue(cons_file,&queue);
    }

    fclose(cons_file);
    return 0 ;
}