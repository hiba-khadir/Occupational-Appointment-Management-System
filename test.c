#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 
#include "queue_ops.h"





/*---------------------------------TYPE DEFINITIONS------------------------------*/



/*structure of a linked lists cell will be used  as a queue of consultations inserted based on priority*/

typedef struct consultation consultation ;
 struct consultation
 {
    char Employee_ID[9];
    char Employee_Name[50];            
    char Consultation_Time[6] ;     //Time is stored as an integer in the format hhmm
    char Consultation_Reason[21] ;   
 };



typedef struct cell typeCell;           /*type of an element in the list*/   

struct cell {          
    consultation conslt;                 
    typeCell *addr;                      /*address of next*/
    int priority ;               /*priority of the consultation*/
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



void Ass_consultation(typeCell **k , consultation c){

    /*copy each field from c consultation to the cell */
    strcpy((*k)->conslt.Consultation_Reason, c.Consultation_Reason);    
    strcpy((*k)->conslt.Employee_Name, c.Employee_Name);
    strcpy((*k)->conslt.Consultation_Time , c.Consultation_Time);
    strcpy((*k)->conslt.Employee_ID, c.Employee_ID ) ;
    (*k)->priority = reason_priority(c.Consultation_Reason); //assign priority based on the reason
    
}

consultation consultation_info(typeCell *k){
    consultation c ;

    strcpy(c.Consultation_Reason , k->conslt.Consultation_Reason) ;
    strcpy(c.Employee_Name , k->conslt.Employee_Name);
    strcpy(c.Consultation_Time , k->conslt.Consultation_Time);
    strcpy(c.Employee_ID, k->conslt.Employee_ID ) ;

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

void display_consultation(typeCell k){
    char *name = k.conslt.Employee_Name ; 
    printf("Employee's ID : %s , ",k.conslt.Employee_ID);
    printf("Employee's name : %s , ",k.conslt.Employee_Name);
    printf("Consultation Time : %s , ", k.conslt.Consultation_Time);
    printf("Concultation Reason : %s , " , k.conslt.Consultation_Reason);
    printf("Priority : %d \n",k.priority);

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



void clear()   //clear input buffer
{
    while ( getchar() != '\n' );
}


void add_appointment(typeQueue *Q){
    
    consultation temp ;  /*store data temporarly */

    int reason ,valid_choice = 0;

    printf("---------------New Appointement----------------\n\n");

    printf("Employee's ID : ");
    scanf("%s",temp.Employee_ID) ;   
    printf("\n");
    clear();


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
    


       
    enqueue(Q,temp);      // add based on priority  

}




int main(){
    typeQueue queue = createQueue();

    consultation appointement ;

    strcpy(appointement.Employee_ID,"23456789");
    printf("employee ID to delete : %s",appointement.Employee_ID);

    FILE  *cons_file =  fopen("Consultations.txt","r");  

    if (cons_file == NULL)
    {
        printf("Can't open file error!");    
        return 1;    
    }


    else{
        printf("read file to queue  : \n ");
        read_file_to_queue(cons_file,&queue);

        printf("\nQueue before adding appointment: \n");
        display_queue(queue);
        
        printf("Queue after : \n");
        add_appointment(&queue);
        display_queue(queue);

   
        
    }

    






    



    fclose(cons_file);
    return 0 ;
}