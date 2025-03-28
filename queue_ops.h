#ifndef queue_ops.h
#define queue_ops.h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>



/*---------------------------------TYPE DEFINITIONS------------------------------*/



/*structure of a linked lists cell will be used  as a queue of consultations inserted based on priority*/

typedef struct consultation consultation ;
 struct consultation
 {
    char Employee_ID[9];
    char Employee_Name[50];            
    char Consultation_Time[6];         /*in the format HH:MM*/
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



//global variables 
int processed_count = 0 ;        //to keep track of the number of appointments processed during the day  
int maximum ;                    //maximum number of appointments per day (to allow the user to modify it )
int appointment_duration ;



//temp 

struct emp {
    char id[9];
    int consult_num;
    char last_consult[11];
    char return_work[11];
    char name[35];
    char history[5][25];
    struct emp *adr;
};
typedef struct emp emp ;
 
/*-------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------Functions Headers----------------------------------------------------------*/

/*---------------------linked lists model---------------------*/

/*allocate a memory space return its address in p*/
void Allocate(typeCell **p);

/*gives the address of the next cell after p*/
typeCell* Next(typeCell *p);

/*assigns the consultation field into *k an  element of the list */
void Ass_consultation(typeCell **k , consultation c);


/*assigns the address field into *k an  element of the list */
void Ass_addr(typeCell **p , typeCell *q);

/*displays an element of type consultation*/
void display_consultation(typeCell k);


/*returns the consultation field inside the list element k*/
consultation consultation_info(typeCell *k);

/*-----------------helper functions ------------------------------*/

/*returns the priority of consultation reasons from 1 to 3 . returns -1 for invalid reasons*/
int reason_priority(char reason[21]);

/*displays the queue*/
void display_queue(typeQueue Q);

/*reads data from text file given by a pointer into a queue*/      
void read_file_to_queue(FILE *file,typeQueue *Q);

//prints the content of Q into file 
void write_queue_to_file(FILE *file , typeQueue Q);

 //clear input buffer
 void clear();
 
/*converts time from int to string */
char* time_string(int time);

/*converts time from a string to an int HHMM format*/
int time_int(char* str_time);

//gets the current date from the system and converts it into a string DD/MM/YYYY
char* get_date(int *day ,int *month , int *year);

//gets the current time from the system and converts it into a string HHhMM
char* get_time();

//free Queue
void free_Q(typeQueue *Q);

//returns in p and q the address of the cell contationing consultation c  and th eprevious cell respectivly 
void access_consultation(typeQueue Q ,consultation c, typeCell* *q, typeCell* *p);

//counts the number of appointments in the queue for the day
int size_of_queue(typeQueue Q);

//return 1 if the queue has more than max elements
int full_queue_day(typeQueue Q);        

//returns in p the address of the last element with priority greater or equal than prio , and its previous in q
void access_by_priority(typeQueue Q , int prio , typeCell **p ,typeCell **q);

//assigns the available visit time based on priority and returns the interval of available time 
void assign_time(typeQueue Q , consultation c ,char **min_time , char **max_time , int current_time);


/*---------------------------------Queue's functionalities-------------------*/

/* deletes an appointment of the queue given the consultation info */
void cancel_appointment(typeQueue *Q , consultation appointment);

//add an appointment from the user's input 
void add_appointment(typeQueue *Q , typeQueue *Next_day_Q);


//reschedules by moving least priority appointment to next day  
void reschedule(typeQueue *Q,typeQueue *Next_day_Q,consultation c);

//automatically schedules return to work appointments and periodic examinations to the next day 
void schedule_periodic_return(emp *head , typeQueue *Next_day_Q ,char* current_date ,int current_time);

//closes an appointment and update the corresponding employee record
void close_appointment(typeQueue *Q);


/*-------------------------------Queue's abstract machine------------------*/

//initialize the queue 
typeQueue createQueue();

/*returns 1 if the list is empty and 0 otherwise*/
int emptyQueue(typeQueue Q);

/*inserts based on priority ( the queue is ordered)*/
void enqueue(typeQueue *Q , consultation new_conslt);

//dequeue the highest priority element in the queue and store the information in dequeued_consultation
void dequeue(typeQueue *Q , consultation *dequeued_conslt );
/*------------------------------------------------------------------------------------------------*/


#endif