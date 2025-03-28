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



//global variables
int processed_count = 0 ;
int maximum = 5 ;
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
            
            fields = sscanf(line,"%8[^;];%49[^;];%5[^;];%20[^\n]\n",
                temp.Employee_ID,
                temp.Employee_Name,
                temp.Consultation_Time,
                temp.Consultation_Reason );

            if (fields == 4 )  /*ensure reading 4 fields*/
            {
               enqueue(Q,temp); /*add to priority queue*/
               printf("consultation reason : %s\n",temp.Consultation_Reason);
               
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



int size_of_queue(typeQueue Q){ //counts the number of appointments in the queue for the day (rescheduled = 0)

    int size = 0 ;  //number of appointments 
    typeCell *p = Q.h;  

    while (p!=NULL)   //traverse the list 
    {
        size++;
        p = Next(p);
        
    }

    return size; 
}


int full_queue_day(typeQueue Q){                     //return 1 if the queue has more than max elements
    return   (size_of_queue(Q) + processed_count >= maximum) ;          //  and 0 else 
    }



//reschedules by moving least priority appointment to next day  

void reschedule(typeQueue *Q,typeQueue *Next_day_Q,consultation c){  

    consultation rescheduled  ;
    typeCell *b = NULL, *a = Q->h;

    if (emptyQueue(*Q))
    {
        if (processed_count >= maximum)
        {
            enqueue(Next_day_Q,c);
            
        }
        else
        {
            printf("no need to reschedule\n");
        }
        

    }
    else  // queue not empty 
    {
       
        if (reason_priority(c.Consultation_Reason) > Q->t->priority)            //the tail has the lowest priority
        {
            //remove the lowest priority appointment from the main queue (the tail)

            while (Next(a) != NULL)
            {
                b = a ;                           //b the previous of a 
                a = Next(a);                     //a will be the tail
            }
            
            if (b == NULL) {      //only one element in the queue

                Q->h =  NULL;
                Q->t = NULL ; 
            } 

            else {
                Ass_addr(&b,NULL);
                Q->t = b;   //  new tail
            }
                                    
            enqueue(Next_day_Q,a->conslt);      //reschedule it
            free(a);

            enqueue(Q,c);                       //the highest priority is scheduled for the day


        }

        else  //the new appointment has the lowest priority
        {
            enqueue(Next_day_Q,c);  //schedule for next day and leave the main queue as is 
        }
                
    }
    

    
    


}

char* get_date(int *day ,int *month , int *year){

    static char str_date[11];
    
    time_t current_time  ;   //pointer to time since 1970
    current_time = time(NULL);

    struct tm date = *localtime(&current_time);


    *day = date.tm_mday ;
    *month = date.tm_mon + 1 ;
    *year = date.tm_year + 1900 ;

    sprintf(str_date,"%d/%d/%d",*day,*month,*year);

    return str_date;

}


char* get_time(){

    static char current_time[6];

    time_t int_time = time(NULL); //seconds since 1970
    struct tm time_tm = *localtime(&int_time);

    sprintf(current_time,"%dh%d",time_tm.tm_hour,time_tm.tm_min);  

    return current_time ;
}


//returns in p the address of the last element with priority greater or equal than prio , and its previous in q
void access_by_priority(typeQueue Q , int prio , typeCell **p ,typeCell **q){   

    //initialization
    *p = Q.h ;   
    *q = NULL;


    //move to next until we find a lower priority 
    while (*p != NULL &&   (prio <= (*p)->priority ))
    {
       *q = *p ;                      /* b is the previous of a */
       *p = Next(*p);        
    }

    
}


//assigns the available visit time based on priority and returns the interval of available time 
void assign_time(typeQueue Q , consultation *c ,int *min_time , int *max_time , int current_time){  

    //variables
    typeCell   *succ , *p;
    int pred_time , succ_time ,time_slot;
    int priority = reason_priority(c->Consultation_Reason);
    int found_time = 0 ;


    if (emptyQueue(Q))
    {
        printf("The queue is empty \n");
        strcpy(c->Consultation_Time,"8h30"); //first time slot

        *min_time = 830; //8h30
        *max_time = 1830; //18h30 

    }

    else
    {

        access_by_priority(Q,priority,&p,&succ);  //find the position by priority 

        
        if( p != NULL && Next(p) != NULL)
        {
            while (!found_time && Next(p) !=NULL)
            {
                succ = Next(p);
            
                pred_time = time_int(p->conslt.Consultation_Time);
                succ_time = time_int(succ->conslt.Consultation_Time);
                time_slot = (pred_time - succ_time)/2;
            
                if ( time_slot >= 20 && current_time < pred_time && current_time >= succ_time + time_slot) //assume a visit takes 20min
                {
                    found_time = 1 ;
                    *min_time = pred_time + time_slot; 
                    *max_time = succ_time - time_slot;
                }
                
                p = Next(p);
            }
            
        }

        if (Next(p) == NULL)  //p is the tail or c has the lowest priority
        {
            pred_time = time_int(Q.t->conslt.Consultation_Time) ;
            *min_time = pred_time + 20 ;
            *max_time = 1830 ;  //end of the day 
        }
        
    }


}

void add_appointment(typeQueue *Q , typeQueue *Next_day_Q , char* current_time){
    
    //variables 
    consultation temp ;  /*store data temporarly */
    int valid_choice = 0 , option ,choice;
    int min_time , max_time ;

    printf("---------------New Appointement----------------\n\n");

    //employee's ID
    printf("Employee's ID : ");
    scanf("%s",temp.Employee_ID) ;   
    printf("\n");
    clear();


    //employee's name
    printf("Employee's Name : "); 

    fgets(temp.Employee_Name, sizeof(temp.Employee_Name), stdin);    // handles the spaces in the name

    int len = strlen(temp.Employee_Name);      //lentgh of the name 

    if (len > 0 && temp.Employee_Name[len - 1] == '\n') {   //remove trailing new lines
        temp.Employee_Name[len - 1] = '\0';
    }
    


    //consultation reason
    do
    {
        
        printf("Consultation Reason : \n");
        printf("    choose one of the following reasons :\n");
        printf("        1- Work Accident\n");
        printf("        2- Occupational Disease\n");
        printf("        3- Pre-employment Visit\n");

        

        if (scanf("%d", &option) != 1)   //if a non integer is entered
        {
            printf("Invalid choice : please choose from the above (1 , 2 or 3)\n");
            //reset reason
            clear();

        }

        else{
         //assign the reason based on user'a choice 
        switch (option)
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

       clear();
    } while (!(valid_choice));

    
    

    //consultation time
    do
    {
        printf("Consultation Time (ex : 9h20) : \n");
        printf("choose an option : \n");
        printf("    1-manual time scheduling \n");
        printf("    2-automatic time scheduling (show suggestion based on priority) \n"); 
        


        if (scanf("%d", &option) != 1)   //if a non integer is entered
        {
            printf("Invalid choice : please choose from the above (1 , 2 or 3)\n");
            //reset reason
            clear();

        }

        else
        {
            switch (option)
            {
            case 1:

                printf("Enter the Consultation Time ( in the format 9h30) : ");
                scanf("%s",temp.Consultation_Time);
                valid_choice = 1 ;
                break;

            case 2:

                assign_time(*Q,&temp,&min_time,&max_time,time_int(current_time));
                printf("Closest available Consultation Time :   ");
                printf("From  %s  to  %s \n",time_string(min_time),time_string(max_time));

                printf("to change the time manually press 1 ,else press any key :\n");
                scanf("%d",&choice);

                switch (choice)
                {
                case 1:
                    printf("Enter the Consultation Time ( in the format 9h30) : ");
                    scanf("%s",temp.Consultation_Time);
                    break;
                
                default:
                    break;
                }
                
                valid_choice = 1 ;
                break;

            default:

                printf("Invalid choice : please choose from the above (1 , 2 or 3)\n");
                //reset reason
                clear();
                break;
            }
        }
        clear();

    } while (!valid_choice);

    if (!full_queue_day(*Q))
    {
        enqueue(Q,temp);      // add based on priority 
    }
    
    else{
        reschedule(Q,Next_day_Q,temp);
    }   
 

}

//free Queue
void free_Q(typeQueue *Q){
    typeCell *p = Q->h ,*temp;
    while (p != NULL)
    {
        temp = p ;
        free(temp);
        p = Next(p);
    }
    
}


void write_queue_to_file(FILE *file , typeQueue Q){

    char line[256];
    typeCell *p = Q.h ;

    printf("writing queue to file : \n");
    
    if (!emptyQueue(Q))
    {
        while (p != NULL)
        {
            sprintf(line, "%s;%s;%s;%s\n",
            p->conslt.Employee_ID,
            p->conslt.Employee_Name,
            p->conslt.Consultation_Time,
            p->conslt.Consultation_Reason );

            printf("%s\n",line);
            fputs(line,file);  //print the line to file 

            p = Next(p);
        }
        
    }
    else
    {
        printf("queue is empty : nothing to write .");
    }
    
    
    
}





int main(){


    //variables 

    typeQueue queue = createQueue();  //main queue
    typeQueue Next_Queue = createQueue();  //to handle > max appointment

    consultation appointement ;
    int i ,d ,m ,y ;
    char 
    min_time[6] , max_time[6] , 
    current_time[6] ,
    current_date[11] ;

    //initialize the time and date
    strcpy(current_date,get_date(&d,&m,&y)); 
    strcpy(current_time,get_time());



    FILE  *cons_file_in =  fopen("Consultations.txt","r");  
    if (!cons_file_in)
    {
        printf("Error cannot open file \n");
        return 1 ;
    }
    else
    {
        read_file_to_queue(cons_file_in,&queue);
        display_queue(queue);
        add_appointment(&queue,&Next_Queue,current_time);


    }
    
    fclose(cons_file_in);

    FILE *cons_file_out = fopen("output.txt","w");
    if (!cons_file_out)
    {
        printf("Error cannot open file \n");
        return 1 ;
    }
    else
    {
        write_queue_to_file(cons_file_out,Next_Queue);

    }
    fclose(cons_file_out);
    

    return 0 ;
}