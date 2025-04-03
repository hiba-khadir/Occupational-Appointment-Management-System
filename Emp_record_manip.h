#ifndef EMP_RECORD_MANIP_H
#define EMP_RECORD_MANIP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "queue_ops.h"       //queue's operation library 

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