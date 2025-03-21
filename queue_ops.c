#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue_ops.h"





/*---------------------------------TYPE DEFINITIONS------------------------------*/



/*structure of a linked lists cell will be used  as a queue of consultations inserted based on priority*/

typedef struct consultation consultation ;
 struct consultation
 {
    char Employee_ID[8];
    char Employee_Name[50];            
    char Consultation_Time[5];         /*in the format HH:MM*/
    char Consultation_Reason[21] ;   
    
 };



typedef struct cell typeCell;           /*type of an element in the list*/   

struct cell {          
    consultation conslt;                 
    typeCell *addr;                      /*address of next*/
    int priority ;                       /*priority of the appointement*/
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


/*allocate a memory space return its address in p*/
void Allocate(typeCell **p){
    *p = malloc(sizeof(typeCell));
}



/*gives the address of the next cell after p*/
typeCell* Next(typeCell *p){
    return p->addr ;
}



/*assigns the consultation field into *k an  element of the list */
void Ass_consultation(typeCell **k , consultation c){
    int i , j ;         /*to iterate through strings*/

    /*copy each field from c consultation to the cell */
    strcpy((*k)->conslt.Consultation_Reason, c.Consultation_Reason);    
    strcpy((*k)->conslt.Employee_Name, c.Employee_Name);
    strcpy((*k)->conslt.Consultation_Time , c.Consultation_Time);
    strcpy((*k)->conslt.Employee_ID, c.Employee_ID ) ;
    (*k)->priority = reason_priority(c.Consultation_Reason);
    
}






/*assigns the address field into *k an  element of the list */
void Ass_addr(typeCell **p , typeCell *q){
    (*p)->addr = q ;

}



/*displays an element of type consultation*/
void display_consultation(typeCell k){
    char *name = k.conslt.Employee_Name ; 
    printf("Employee's ID : %s \n",k.conslt.Employee_ID);
    printf("Employee's name : %s \n",k.conslt.Employee_Name);
    printf("Consultation Time : %s \n", k.conslt.Consultation_Time);
    printf("Concultation Reason : %s \n" , k.conslt.Consultation_Reason);
    printf("Priority : %d",k.priority);

}


/*displays the queue*/
void display_queue(typeQueue Q){
    typeCell *p = Q.h ;
    int cpt = 1 ;

    while (p != NULL)
    {   
        printf("\n");
        printf("appointment %d : \n ", cpt);
        display_consultation(*p);
        p = Next(p);
        cpt++ ;
    }


}


/*returns the consultation field inside the list element k*/
consultation consultation_info(typeCell *k){
    consultation c ;

    strcpy(c.Consultation_Reason , k->conslt.Consultation_Reason) ;
    strcpy(c.Employee_Name , k->conslt.Employee_Name);
    strcpy(c.Consultation_Time , k->conslt.Consultation_Time);
    strcpy(c.Employee_ID, k->conslt.Employee_ID ) ;

    return c ;
}

/*-----------------------------------------------------------------------------------------------*/







/*-------------------------list and queues functions----------------------------------*/

/*returns the priority of consultation reasons from 1 to 3 . returns -1 for invalid reasons*/
/*note to me : remember to fix this later to manage upper and lower case :) */

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



/*reads data from text file given by a pointer into a queue*/      
void read_file_to_queue(FILE *file,typeQueue *Q){
    
    char line[90];
    int fields ;    /*{ID, name , time , reason ..}*/
    consultation temp ;   /*store te retrieved data temporarly*/
    

    while (fgets(line,sizeof(line),file) != NULL)         /*fgets not NULL -stops at the EOF*/
        {
            
            if (fields = sscanf(line,"%8[^;];%49[^;];%5[^;];%20[^\n]\n",temp.Employee_ID,temp.Employee_Name,temp.Consultation_Time,temp.Consultation_Reason ) == 4 )  /*ensure reading 4 fields*/
            {
               enqueue(Q,temp); /*add to priority queue*/
            }

           else
            {
                printf("Invalid line format \n");
                printf("%d",fields);   /*how many fields were read */
            }
            
        }
    }







//returns in p and q the address of the cell contationing consultation c  and th eprevious cell respectivly 

void access_consultation(typeQueue Q ,consultation c, typeCell* *q, typeCell* *p){
    *q=NULL;                         /*pointer to the previous cell*/
    *p= Q.h;                       /* pointer to the current cell*/
    int found = 0 ;                 /*control boolean*/

    while (*p != NULL && found == 0 ){        /*traverse the list*/

        if (strcmp((*p)->conslt.Employee_ID,c.Employee_ID) == 0 )        /*test if the appointement is found*/
        {
            found = 1;                          /*update the boolean*/
        }
        else{
            *q = *p ;                      /*move to next cell*/
            *p = (*p)->addr ;
        }
        
    }
    
    printf("found : %d",found);

}






/* deletes an appointment of the queue given the consultation info */
//remember to fix this later to delete given only one field (name , id or time )

void cancel_appointment(typeQueue *Q , consultation appointment){
    typeCell *b, *a ;

    access_consultation(*Q,appointment,&a,&b);

    if (b != NULL)             /*then the value was found and the list is not empty  */
    {  
         if( a == NULL){       /*the Cell to delete is the head*/
            Q->h = b->addr ;  
        } 
        
        else                  
        {
            a->addr = b->addr ;  /*link previous to next */
        }
        
        free(b);
     
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




/*returns 1 if the list is empty and 0 otherwise*/
int emptyQueue(typeQueue Q){
    return (Q.h == NULL) ;       
}





/*inserts based on priority ( the queue is ordered)*/

void enqueue(typeQueue *Q , consultation new_conslt){   /*inserts based on priority ( the queue is ordered)*/
    //variables
    typeCell *new ;
    typeCell *a = Q->h ,*b=NULL ;             /*a and b are used to traverse the queue*/


    //initialization
    Allocate(&new);
    Ass_addr(&new,NULL);
    Ass_consultation(&new,new_conslt);  

    
    //insertion
    //find isert position
    while (a != NULL &&   (new->priority <= a->priority ))
    {
        b = a ;                      /* b is the previous of a */
        a = Next(a);        
    }
    

    //linking 

    if (b != NULL)  //insert at the middle or end
    {
        Ass_addr(&b,new);
        Ass_addr(&new,a);   

        if (a == NULL)   //insert at the end
        {
           Q->t = new ;  //update the tail 
        }
        
    }

    else       // b = NIL
    {
        if (a == NULL)   //the queue was initially empty
        {
            /*new is the first element inserted*/
            Q->h = new ;
            Q->t = new ;  
            
        }

        else{        //insert at the head 
            Ass_addr(&new,Q->h);
            Q->h = new ;    //update the head

        }
        
    }
}
        











