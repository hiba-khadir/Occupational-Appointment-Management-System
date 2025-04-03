#ifndef EMP_RECORD_MANIP_H
#define EMP_RECORD_MANIP_H

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
    typeCell *t ;                          /*pointer to the tail of the queue*/
}; 
typedef struct typeQueue typeQueue ;       /*define queue as a type */
//TYPE DEFINITION 

struct emp {  //type of a cell in the linked list
    char id[9];
    int consult_num;
    char last_consult[11];
    char return_work[11];
    char name[35];
    char history[5][25];
    struct emp *adr;
};
typedef struct emp emp ;


//functions prototypes
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