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
    int rescheduled ;    //to mark rescheduled appointments 
    
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



//global variables 
int processed_count = 0 ;        //to keep track of the number of appointments processed during the day  
int maximum ;                    //maximum number of appointments per day (to allow the user to modify it )
 
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
    (*k)->conslt.rescheduled = c.rescheduled ;
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
    printf("rescheduled %d",k.conslt.rescheduled);

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
    c.rescheduled = k->conslt.rescheduled ;

    return c ;
}

/*-----------------------------------------------------------------------------------------------*/







/*-------------------------list and queues functions----------------------------------*/


//-------------------------------helper functions --------------------------



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


 //clear input buffer
void clear() 
{
    while ( getchar() != '\n' );
}



char* time_string(int time) {
    static char str_time[6];  

    // Validate time format
    if (time >= 0 && time <= 2359) {
        sprintf(str_time, "%dh%d", time / 100, time % 100);  
    } else {
        strcpy(str_time, "ERROR");
    }

    return str_time;
}
    



int time_int(char* str_time) {
    int min, hour;

    // extract hours and minutes
    if (sscanf(str_time, "%dh%d", &hour, &min) != 2) {
        return -1;  // Incorrect format
    }

    // validate houra and minutes
    if (hour < 0 || hour > 23 || min < 0 || min > 59) {
        return -1;
    }

    return (hour * 100 + min);
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
    


}


int size_of_day_queue(typeQueue Q){ //counts the number of appointments in the queue for the day (rescheduled = 0)

    int size = 0 ;  //number of appointments 
    typeCell *p = Q.h;  

    while (p!=NULL)   //traverse the list 
    {
        if (!p->conslt.rescheduled)  //count only non rescheduled appointments 
        {
            size++;
        }
        
        p = Next(p);
        

    }

    return size; 
}


int full_queue_day(typeQueue Q){                     //return 1 if the queue has more than max elements
    return   (size_of_queue(Q) + processed_count >= maximum) ;          //  and 0 else 
    }


//returns in p the address of the last element with priority greater or equal than prio , and its previous in q

void access_by_priority(typeQueue Q , int prio , typeCell **p ,typeCell **q){   

    //initialization
    *p = Q.h ;   
    *q = NULL;


    //move to next until we find a lower priority 
    while (p != NULL &&   (prio <= (*p)->priority ))
    {
       *q = *p ;                      /* b is the previous of a */
       *p = Next(*p);        
    }
    
}

//assigns to c the the available visit time based on priority and returns the interval of available time 
void assign_time(typeQueue Q , consultation *c ,char **min_time , char **max_time){  

    //variables
    typeCell *pred , *succ ;
    int priority = reason_priority(c->Consultation_Reason);
    access_by_priority(Q,priority,&pred,&succ);
}


//--------------------------------------------------------------------------------------------------------





//---------------------------------------QUEUE FUNCTIONALITIES----------------------------------------------




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



//remember to fix this to handle time allocation 
void add_appointment(typeQueue *Q){  //add an appointment to the queue from user input 
    
    consultation temp ;  /*store data temporarly */

    int reason ,valid_choice = 0;

    printf("---------------New Appointement----------------\n\n");



    printf("Employee's ID : ");
    scanf("%s",temp.Employee_ID) ;   
    printf("\n");
    clear(); //clear the input buffer from previous scanf


    printf("Employee's Name : "); 

    fgets(temp.Employee_Name, sizeof(temp.Employee_Name), stdin);    // handles the spaces in the name

    int len = strlen(temp.Employee_Name);      //lentgh of the name 

    if (len > 0 && temp.Employee_Name[len - 1] == '\n') {   //remove trailing new lines

        temp.Employee_Name[len - 1] = '\0';
    }
    
    printf("Consultation Time (ex : 9h20) : ");
    scanf("%s",temp.Consultation_Time) ;    
    printf("\n");
    clear();


    //reapeat until a valide coice is entered

    do
    {
        
        printf("Consultation Reason : \n");
        printf("    choose one of the following reasons :\n");
        printf("        1- Work Accident\n");
        printf("        2- Occupational Disease\n");
        printf("        3- Pre-employment Visit\n");

        

        if (scanf("%d", &reason) != 1)   //if a non integer is entered
        {
            printf("Invalid choice : please choose from the above (1 , 2 or 3)\n");
            //reset reason
            clear();

        }

        else{
         //assign the reason based on user'a choice 
        switch (reason)
        {
        case 1:
            strcpy(temp.Consultation_Reason,"Work-accident");
            valid_choice = 1 ; 
            break;
    
        case 2:
            strcpy(temp.Consultation_Reason,"Occupational-disease"); 
            valid_choice = 1 ;   
            break;
    
        case 3:
            strcpy(temp.Consultation_Reason,"Pre-employement"); 
            valid_choice = 1 ;  
            break;

        default:
            printf("Invalid choice : please choose from the above (1 , 2 or 3)\n");
            break;
        }

        }        

       
    } while (!(valid_choice));
    



    if (!full_queue_day(*Q))     //if less than 10 appointments were scheduled 
    {
        temp.rescheduled = 0 ;    // the appointment is scheduled for the day
 
    }
    
    else{                   //more than 10 were scheduled
        reschedule(Q); 
  
    }
    enqueue(Q,temp);      // add based on priority 

 

}



//reschedules by moving least priority appointment to next day  ( marks it as rescheduled )

typeCell* reschedule(typeQueue *Q){  

    //find least prioritized appointment 

    typeCell *p = Q->h;
    typeCell *min = Q->h ;

    int min_prio = Q->h->priority;
    
    while (p != NULL)
    {
        if (p->priority <= min_prio)
        {
            min_prio = p->priority ;
            min = p ;
        }
        

        p = Next(p);
    }
    
    //mark as rescheduled 
    min->conslt.rescheduled = 1 ;

    return min ;
}


void close_appointment(typeQueue *Q){

    consultation temp ;
    dequeue(Q,&temp);                    //retrieve highest priority element and store it in temp
    updateHistory(temp.Employee_ID,temp.Consultation_Reason);  //update the history of last 5 visits in EmpRecord

    if (strcmp(temp.Consultation_Reason,"Pre-employement") == 0 ) //if it is a pre-employment visit 
    {
        addNewEmp(temp) ; //add the new employee to the record 
    }
    

}

/*---------------------------------------------------------------------------------------------------------------*/








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
    access_by_priority(*Q,new->priority,&b,&a);
    

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


//dequeue the highest priority element in the queue and store the information in dequeued_consultation
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
        











