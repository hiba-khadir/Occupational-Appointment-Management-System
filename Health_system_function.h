#ifndef QUEUE_OPS_H
#define QUEUE_OPS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*-------------------------------- TYPE DEFINITIONS --------------------------------*/
typedef struct consultation {
    char Employee_ID[9];
    char Employee_Name[50];
    char Consultation_Time[6];         /*in the format HH:MM*/
    char Consultation_Reason[21];
} consultation;

typedef struct cell typeCell;           /*type of an element in the list*/
struct cell {
    consultation conslt;
    typeCell *addr;                      /*address of next*/
    int priority;                       /*priority of the appointement*/
};

struct typeQueue {
    typeCell *h;                          /*pointer to the head of the queue*/
    typeCell *t;                          /*pointer to the tail of the queue*/
};
typedef struct typeQueue typeQueue;       /*define queue as a type */

struct emp {
    char id[9];
    int consult_num;
    char last_consult[11];
    char return_work[11];
    char name[35];
    char history[5][25];
    struct emp *adr;
};
typedef struct emp emp;

/*-------------------------------- GLOBAL VARIABLES --------------------------------*/
int processed_count = 0;        //to keep track of the number of appointments processed during the day
int maximum = 10;                    //maximum number of appointments per day

/*-------------------------------- FUNCTION PROTOTYPES --------------------------------*/
/* Utility Functions */
int unique_ID(emp* head ,char *ID);
void clear();
char* time_string(int time);
int time_int(char* str_time);
char* get_date(int *day, int *month, int *year);
char* get_time();
int reason_priority(char reason[21]);

/* Queue Operations */
void Allocate(typeCell **p);
typeCell* Next(typeCell *p);
void Ass_consultation(typeCell **k, consultation c);
void Ass_addr(typeCell **p, typeCell *q);
typeQueue createQueue();
int emptyQueue(typeQueue Q);
consultation consultation_info(typeCell *k);
void display_consultation(typeCell k);
void display_queue(typeQueue Q);
int size_of_queue(typeQueue Q);
int full_queue_day(typeQueue Q);
void access_by_priority(typeQueue Q, int prio, typeCell **p, typeCell **q);
void access_consultation(typeQueue Q, char* ID, typeCell **q, typeCell **p);
void free_Q(typeQueue *Q);

/* Queue Functionality */
void enqueue(typeQueue *Q, consultation new_conslt);
void dequeue(typeQueue *Q, consultation *dequeued_conslt);
void read_file_to_queue(FILE *file, typeQueue *Q);
void write_queue_to_file(FILE *file, typeQueue Q);
void assign_time(typeQueue Q, consultation c, char **min_time, char **max_time, int current_time);
void cancel_appointment(typeQueue *Q, char* ID);
void add_appointment(typeQueue *Q, typeQueue *Next_day_Q);
void reschedule(typeQueue *Q, typeQueue *Next_day_Q, consultation c);
void reschedule_manual(typeQueue *Q, typeQueue *Next_day_queue);
void schedule_periodic_return(emp *head, typeQueue *Next_day_Q, char* current_date, int current_time);
void close_appointment(typeQueue *Q, emp **empHead);

/* Employee Record Operations */
struct emp* createEmp();
struct emp* findEmp(struct emp *h, char id[]);
void updateHistory(struct emp *h, char id[], char reason[]);
void changeConsultNum(int n, char id[], struct emp *h);
void changeLastConsult(char lastConsult[], char id[], struct emp *h);
void changeReturnWork(char returnWork[], char id[], struct emp *h);
void readLine(struct emp *p, char *line);
struct emp* loadEmp(FILE *f);
void printEmp(struct emp *p);
void printInGrp(struct emp *h);
void saveEmp(struct emp *h, FILE *f);
struct emp* addEmp(struct emp *h);
void addNewEmp(consultation *q, struct emp **h);
void updateEmp(struct emp *h, char id[]);
void deleteEmp(struct emp **h, char deleted_id[]);
void subAutoUpdate(struct emp *h, char id[], char reason[], char date[]);
void updateSingleEmp(struct emp **h, typeQueue *q, char id[], char date[]);

#endif
