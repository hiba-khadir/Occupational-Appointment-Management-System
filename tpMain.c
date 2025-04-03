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

/*------------------------------- UTILITY FUNCTIONS -------------------------------*/

// Checks if an ID is unique in the list
// Returns 1 if unique, 0 if not unique
int unique_ID(emp* head, char *ID) {
    emp *p = head; // to traverse the queue
    int unique = 1; // Assume ID is unique initially

    while (p != NULL && unique) {
        if (strcmp(p->id, ID) == 0) {
            unique = 0; // Set to not unique if match found
        }
        p = p->adr;
    }

    return unique;
}


//clear input buffer
void clear() {
    while (getchar() != '\n');
}

/*converts time from int to string */
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

    // validate hours and minutes
    if (hour < 0 || hour > 23 || min < 0 || min > 59) {
        return -1;
    }

    return (hour * 100 + min);
}

//gets the current date from the system and converts it into a string DD/MM/YYYY
char* get_date(int *day, int *month, int *year) {
    static char str_date[11];

    time_t current_time;   //pointer to time since 1970
    current_time = time(NULL);

    struct tm date = *localtime(&current_time);

    *day = date.tm_mday;
    *month = date.tm_mon + 1;
    *year = date.tm_year + 1900;

    sprintf(str_date, "%d/%d/%d", *day, *month, *year);

    return str_date;
}

char* get_time() {
    static char current_time[6];

    time_t int_time = time(NULL); //seconds since 1970
    struct tm time_tm = *localtime(&int_time);

    sprintf(current_time, "%dh%d", time_tm.tm_hour, time_tm.tm_min);

    return current_time;
}

/*returns the priority of consultation reasons from 1 to 4. returns -1 for invalid reasons*/
int reason_priority(char reason[21]) {
    if (strcmp(reason, "Work-accident") == 0) {
        return 4;
    } else if (strcmp(reason, "Occupational-disease") == 0) {
        return 3;
    } else if (strcmp(reason, "Pre-employement") == 0 || strcmp(reason, "Return-to-Work") == 0) {
        return 2;
    } else if (strcmp(reason, "Periodic") == 0) {
        return 1;
    } else {
        return -1;
    }
}

/*------------------------------- QUEUE BASIC OPERATIONS -------------------------------*/
/*allocate a memory space return its address in p*/
void Allocate(typeCell **p) {
    *p = malloc(sizeof(typeCell));
}

/*gives the address of the next cell after p*/
typeCell* Next(typeCell *p) {
    return p->addr;
}

/*assigns the consultation field into *k an element of the list */
void Ass_consultation(typeCell **k, consultation c) {
    /*copy each field from c consultation to the cell */
    strcpy((*k)->conslt.Consultation_Reason, c.Consultation_Reason);
    strcpy((*k)->conslt.Employee_Name, c.Employee_Name);
    strcpy((*k)->conslt.Consultation_Time, c.Consultation_Time);
    strcpy((*k)->conslt.Employee_ID, c.Employee_ID);
    (*k)->priority = reason_priority(c.Consultation_Reason);
}

/*assigns the address field into *k an element of the list */
void Ass_addr(typeCell **p, typeCell *q) {
    (*p)->addr = q;
}

typeQueue createQueue() {
    typeQueue Q;
    Q.h = NULL;
    Q.t = NULL;            /*initialize the queue as empty */
    return Q;
}

/*returns 1 if the list is empty and 0 otherwise*/
int emptyQueue(typeQueue Q) {
    return (Q.h == NULL);
}

/*returns the consultation field inside the list element k*/
consultation consultation_info(typeCell *k) {
    consultation c;
    strcpy(c.Consultation_Reason, k->conslt.Consultation_Reason);
    strcpy(c.Employee_Name, k->conslt.Employee_Name);
    strcpy(c.Consultation_Time, k->conslt.Consultation_Time);
    strcpy(c.Employee_ID, k->conslt.Employee_ID);
    return c;
}

/*displays an element of type consultation*/
void display_consultation(typeCell k) {
    printf("Employee's ID : %s \n", k.conslt.Employee_ID);
    printf("Employee's name : %s \n", k.conslt.Employee_Name);
    printf("Consultation Time : %s \n", k.conslt.Consultation_Time);
    printf("Concultation Reason : %s \n", k.conslt.Consultation_Reason);
    printf("Priority : %d\n", k.priority);
}

/*displays the queue*/
void display_queue(typeQueue Q) {
    typeCell *p = Q.h;
    int cpt = 1;

    printf("Hello from display queue \n");
    if (Q.h != NULL) {
        printf("the head is : %s \n", p->conslt.Employee_Name);
    } else {
        printf("the queue is empty \n");
    }

    while (p != NULL) {
        printf("\n");
        printf("appointment %d : \n ", cpt);
        display_consultation(*p);
        p = Next(p);
        cpt++;
    }
}

int size_of_queue(typeQueue Q) { //counts the number of appointments in the queue for the day
    int size = 0;
    typeCell *p = Q.h;

    while (p != NULL) {  //traverse the list
        size++;
        p = Next(p);
    }
    return size;
}

int full_queue_day(typeQueue Q) {
    //return 1 if the queue has more than max elements and 0 else
    return (size_of_queue(Q) + processed_count >= maximum);
}

//returns in p the address of the last element with priority greater or equal than prio, and its previous in q
void access_by_priority(typeQueue Q, int prio, typeCell **p, typeCell **q) {
    //initialization
    *p = Q.h;
    *q = NULL;

    //move to next until we find a lower priority
    while (*p != NULL && (prio <= (*p)->priority)) {
        *q = *p;                      /* q is the previous of p */
        *p = Next(*p);
    }
}

//returns in p and q the address of the cell containing consultation with ID and the previous cell respectively
void access_consultation(typeQueue Q, char* ID, typeCell **q, typeCell **p) {
    *q = NULL;                         /*pointer to the previous cell*/
    *p = Q.h;                          /*pointer to the current cell*/
    int found = 0;                     /*control boolean*/

    while (*p != NULL && found == 0) {         /*traverse the list*/
        if (strcmp((*p)->conslt.Employee_ID, ID) == 0) {        /*test if the appointment is found*/
            found = 1;                          /*update the boolean*/
        } else {
            *q = *p;                      /*move to next cell*/
            *p = (*p)->addr;
        }
    }
}

//free Queue
void free_Q(typeQueue *Q) {
    typeCell *p = Q->h, *temp;

    while (p != NULL) {
        temp = p;
        p = Next(p);
        free(temp);
    }

    Q->h = NULL;
    Q->t = NULL;
}

/*------------------------------- QUEUE FUNCTIONALITY -------------------------------*/
void enqueue(typeQueue *Q, consultation new_conslt) {   /*inserts based on priority (the queue is ordered)*/
    //variables
    typeCell *new;
    typeCell *a = Q->h, *b = NULL;             /*a and b are used to traverse the queue*/

    //initialization
    Allocate(&new);
    Ass_addr(&new, NULL);
    Ass_consultation(&new, new_conslt);

    //insertion
    //find insert position
    access_by_priority(*Q, new->priority, &a, &b);

    //linking
    if (b != NULL) {  //insert at the middle or end
        Ass_addr(&b, new);
        Ass_addr(&new, a);

        if (a == NULL) {  //insert at the end
            Q->t = new;  //update the tail
        }
    } else {      // b = NULL
        if (a == NULL) {  //the queue was initially empty
            /*new is the first element inserted*/
            Q->h = new;
            Q->t = new;
        } else {        //insert at the head
            Ass_addr(&new, Q->h);
            Q->h = new;    //update the head
        }
    }
}

//dequeue the highest priority element in the queue and store the information in dequeued_consultation
void dequeue(typeQueue *Q, consultation *dequeued_conslt) {
    typeCell *temp;

    if (!emptyQueue(*Q)) {
        temp = Q->h;                      /*temporary save for the head*/

        /*copy each field from the head consultation to the dequeued consultation*/
        strcpy(dequeued_conslt->Consultation_Reason, consultation_info(Q->h).Consultation_Reason);
        strcpy(dequeued_conslt->Employee_ID, consultation_info(Q->h).Employee_ID);
        strcpy(dequeued_conslt->Employee_Name, consultation_info(Q->h).Employee_Name);
        strcpy(dequeued_conslt->Consultation_Time, consultation_info(Q->h).Consultation_Time);

        Q->h = Next(Q->h);                        /*move head to next */

        // Update tail if queue becomes empty
        if (Q->h == NULL) {
            Q->t = NULL;
        }

        free(temp);                               /*free the dequeued element*/
    } else {
        printf("ERROR : QUEUE IS EMPTY. CANNOT DEQUEUE \n");
    }
}

/*reads data from text file given by a pointer into a queue*/
void read_file_to_queue(FILE *file, typeQueue *Q) {
    char line[90];
    int fields;    /*{ID, name, time, reason ..}*/
    consultation temp;   /*store the retrieved data temporarily*/

    while (fgets(line, sizeof(line), file) != NULL) {         /*fgets not NULL -stops at the EOF*/
        printf("%s", line);
        fields = sscanf(line, "%8[^;];%49[^;];%5[^;];%20[^\n]", temp.Employee_ID, temp.Employee_Name, temp.Consultation_Time, temp.Consultation_Reason);
        if (fields == 4) {  /*ensure reading 4 fields*/
            enqueue(Q, temp); /*add to priority queue*/
        } else {
            printf("Invalid line format \n");
            printf("%d", fields);   /*how many fields were read */
        }
    }
}

//prints the content of Q into file
void write_queue_to_file(FILE *file, typeQueue Q) {
    char line[90];
    typeCell *p = Q.h;

    printf("writing queue to file : \n");

    if (!emptyQueue(Q)) {
        while (p != NULL) {
            sprintf(line, "%s;%s;%s;%s\n",
                   p->conslt.Employee_ID,
                   p->conslt.Employee_Name,
                   p->conslt.Consultation_Time,
                   p->conslt.Consultation_Reason);

            printf("%s", line);
            fputs(line, file);  //print the line to file

            p = Next(p);
        }
    } else {
        printf("queue is empty : nothing to write.");
    }
}

//assigns the available visit time based on priority and returns the interval of available time
void assign_time(typeQueue Q, consultation c, char **min_time, char **max_time, int current_time) {
    //variables
    typeCell *succ, *p;
    int pred_time, succ_time, time_slot;
    int priority = reason_priority(c.Consultation_Reason);
    int found_time = 0;

    if (emptyQueue(Q)) {
        strcpy(c.Consultation_Time, "8h30"); //first time slot
        *min_time = "8h30";
        *max_time = NULL;
    } else {
        access_by_priority(Q, priority, &p, &succ);  //find the position by priority

        if (p != NULL && Next(p) != NULL) {
            while (!found_time && Next(p) != NULL) {
                succ = Next(p);

                pred_time = time_int(p->conslt.Consultation_Time);
                succ_time = time_int(succ->conslt.Consultation_Time);
                time_slot = (pred_time - succ_time) / 2;

                if (time_slot >= 20 && current_time < pred_time && current_time >= succ_time + time_slot) { //assume a visit takes 20min
                    found_time = 1;
                    *min_time = time_string(pred_time + time_slot);
                    *max_time = time_string(succ_time - time_slot);
                }
                p = Next(p);
            }
        }

        if (p == NULL || Next(p) == NULL) {  //p is the tail or c has the lowest priority
            pred_time = time_int(Q.t->conslt.Consultation_Time);
            *min_time = time_string(pred_time + 20);
            *max_time = NULL;
        }
    }
}

/* deletes an appointment of the queue given the consultation info */
void cancel_appointment(typeQueue *Q, char* ID) {
    typeCell *b, *a;

    access_consultation(*Q, ID, &b, &a);

    if (a != NULL) {           /*then the value was found and the list is not empty */
        if (b != NULL) {
            b->addr = a->addr;  /*link previous to next */

            // Update tail if needed
            if (a == Q->t) {
                Q->t = b;
            }
        } else {      /*the cell to delete is the head*/
            Q->h = a->addr;

            // Update tail if queue becomes empty
            if (Q->h == NULL) {
                Q->t = NULL;
            }
        }
        free(a);
    }
}

//add an appointment from the user's input
void add_appointment(typeQueue *Q, typeQueue *Next_day_Q) {
    consultation temp;  /*store data temporarily */
    int reason, valid_choice = 0;

    printf("---------------New Appointment----------------\n\n");

    printf("Employee's ID : ");
    scanf("%s", temp.Employee_ID);
    printf("\n");
    clear();

    printf("Employee's Name : ");
    fgets(temp.Employee_Name, sizeof(temp.Employee_Name), stdin);    // handles the spaces in the name

    int len = strlen(temp.Employee_Name);      //length of the name
    if (len > 0 && temp.Employee_Name[len - 1] == '\n') {   //remove trailing new lines
        temp.Employee_Name[len - 1] = '\0';
    }

    printf("Consultation Time (ex : 9h20) : ");
    scanf("%s", temp.Consultation_Time);
    printf("\n");
    clear();

    //repeat until a valid choice is entered
    do {
        printf("Consultation Reason : \n");
        printf("    choose one of the following reasons :\n");
        printf("        1- Work Accident\n");
        printf("        2- Occupational Disease\n");
        printf("        3- Pre-employment Visit\n");

        if (scanf("%d", &reason) != 1) {  //if a non integer is entered
            printf("Invalid choice : please choose from the above (1, 2 or 3)\n");
            //reset reason
            clear();
        } else {
            //assign the reason based on user's choice
            switch (reason) {
                case 1:
                    strcpy(temp.Consultation_Reason, "Work-accident");
                    valid_choice = 1;
                    break;
                case 2:
                    strcpy(temp.Consultation_Reason, "Occupational-disease");
                    valid_choice = 1;
                    break;
                case 3:
                    strcpy(temp.Consultation_Reason, "Pre-employement");
                    valid_choice = 1;
                    break;
                default:
                    printf("Invalid choice : please choose from the above (1, 2 or 3)\n");
                    break;
            }
        }
    } while (!valid_choice);

    if (!full_queue_day(*Q)) {
        enqueue(Q, temp);      // add based on priority
    } else {
        reschedule(Q, Next_day_Q, temp);
    }
}

//reschedules by moving least priority appointment to next day
void reschedule(typeQueue *Q, typeQueue *Next_day_Q, consultation c) {
    typeCell *b = NULL, *a = Q->h;

    if (emptyQueue(*Q)) {
        if (processed_count >= maximum) {
            enqueue(Next_day_Q, c);
        } else {
            printf("no need to reschedule\n");
        }
    } else {  // queue not empty
        if (reason_priority(c.Consultation_Reason) > Q->t->priority) {            //the tail has the lowest priority
            //remove the lowest priority appointment from the main queue (the tail)
            while (Next(a) != NULL) {
                b = a;                           //b the previous of a
                a = Next(a);                     //a will be the tail
            }

            if (b == NULL) {      //only one element in the queue
                Q->h = NULL;
                Q->t = NULL;
            } else {
                Ass_addr(&b, NULL);
                Q->t = b;   //  new tail
            }

            enqueue(Next_day_Q, a->conslt);      //reschedule it
            free(a);

            enqueue(Q, c);                       //the highest priority is scheduled for the day
        } else {  //the new appointment has the lowest priority
            enqueue(Next_day_Q, c);  //schedule for next day and leave the main queue as is
        }
    }
}

void schedule_periodic_return(emp *head, typeQueue *Next_day_Q, char* current_date, int current_time) {
    //variables
    char date_buffer[11], min_time[6], max_time[6];
    int d, m, y, current_d, current_m, current_y;
    emp *p = head;  // Initialize p to head
    consultation temp;
    char *min_time_ptr = min_time, *max_time_ptr = &max_time[0];

    //traverse the list to find periodics and return to work
    while (p != NULL) {
        if (strcmp(p->return_work, current_date) == 0) { //compare the return to work date with the current date
            //initialize the appointment info
            strcpy(temp.Consultation_Reason, "Return-to-Work");
            strcpy(temp.Employee_ID, p->id);
            strcpy(temp.Employee_Name, p->name);
            assign_time(*Next_day_Q, temp, &min_time_ptr, &max_time_ptr, current_time);
            strcpy(temp.Consultation_Time, min_time_ptr);

            //schedule it for the next day
            enqueue(Next_day_Q, temp);
        }

        strcpy(date_buffer, p->last_consult);
        sscanf(date_buffer, "%d/%d/%d", &d, &m, &y); //parse the date string and get int values
        sscanf(current_date, "%d/%d/%d", &current_d, &current_m, &current_y);

        if (d == current_d && m == current_m && y == current_y - 1) {
            strcpy(temp.Consultation_Reason, "Periodic");
            strcpy(temp.Employee_ID, p->id);
            strcpy(temp.Employee_Name, p->name);
            assign_time(*Next_day_Q, temp, &min_time_ptr, &max_time_ptr, current_time);
            strcpy(temp.Consultation_Time, min_time_ptr);

            //schedule it for the next day
            enqueue(Next_day_Q, temp);
        }

        p = p->adr;
    }
}

//closes an appointment and update the corresponding employee record
void close_appointment(typeQueue *Q, emp **empHead) {
    char choice;
    consultation temp;

    if (emptyQueue(*Q)) {
        printf("No appointments to close.\n");
        return;
    }

    dequeue(Q, &temp);       //remove highest priority element and store it in temp

    // Find employee and update history or add new employee
    emp *existingEmp = findEmp(*empHead, temp.Employee_ID);

    if (existingEmp != NULL) {
        // Update existing employee
        updateHistory(*empHead, temp.Employee_ID, temp.Consultation_Reason);
        existingEmp->consult_num++;

        // Get current date for last consultation date
        int day, month, year;
        char *dateStr = get_date(&day, &month, &year);
        changeLastConsult(dateStr, temp.Employee_ID, *empHead);
    } else if (strcmp(temp.Consultation_Reason, "Pre-employement") == 0) {
        printf("Do you want to add this employee's record to the system? (Y/N): ");
        scanf(" %c", &choice);
        clear();

        if (choice == 'Y' || choice == 'y') {
            addNewEmp(&temp, empHead);
            printf("New employee added to records.\n");
        }
    }

    processed_count++;
    printf("Appointment closed successfully.\n");
}

//reschedules an appointment given its ID from the stdin to next day
void reschedule_manual(typeQueue *Q, typeQueue *Next_day_queue) {
    char ID[9];
    typeCell *pred = NULL, *rescheduled = NULL;
    consultation temp;
    int option;

    if (!emptyQueue(*Q)) {
enter_ID:
        printf("Enter ID of the employee to reschedule his appointment to next day: ");
        scanf("%s", ID);
        clear();

        access_consultation(*Q, ID, &pred, &rescheduled);

        if (rescheduled != NULL) {  // Found the appointment
            if (pred == NULL) {  // The head is rescheduled
                dequeue(Q, &temp);  // Dequeue the head
            } else {  // Not the head
                // Save appointment info
                strcpy(temp.Employee_ID, rescheduled->conslt.Employee_ID);
                strcpy(temp.Employee_Name, rescheduled->conslt.Employee_Name);
                strcpy(temp.Consultation_Time, rescheduled->conslt.Consultation_Time);
                strcpy(temp.Consultation_Reason, rescheduled->conslt.Consultation_Reason);

                // Remove from current queue
                pred->addr = rescheduled->addr;
                if (rescheduled == Q->t) {  // If it's the tail
                    Q->t = pred;
                }
                free(rescheduled);
            }

            enqueue(Next_day_queue, temp);  // Push it to next day queue
            printf("Appointment rescheduled successfully.\n");
        } else {  // No such ID
            printf("Appointment not found: choose an option\n");
            printf("    1- Retype the ID (in case of typing mistake)\n");
            printf("    2- Add an appointment to Next day\n");
            printf("    3- Cancel operation\n");

            scanf("%d", &option);
            clear();

            switch (option) {
                case 1:
                    goto enter_ID;   //ask the user to enter an ID again
                    break;
                case 2:
                    add_appointment(Next_day_queue, NULL); //add an appointment to the next day
                    break;
                default:
                    printf("Operation canceled.\n");
                    break;
            }
        }
    } else {  //queue is empty
        printf("Queue is empty: no appointments to reschedule.\n");
    }
}

/*------------------------------- EMPLOYEE RECORD OPERATIONS -------------------------------*/
struct emp* createEmp() {
    struct emp *p = (struct emp*)malloc(sizeof(struct emp));
    if (p == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    p->adr = NULL;
    memset(p, 0, sizeof(struct emp));
    return p;
}

struct emp* findEmp(struct emp *h, char id[]) {
    struct emp *p = h;
    while (p != NULL) {
        if (strcmp(p->id, id) == 0) {
            return p;
        }
        p = p->adr;
    }
    return NULL;
}

void updateHistory(struct emp *h, char id[], char reason[]) {
    struct emp *p = findEmp(h, id);
    if (p == NULL) {
        printf("Employee doesn't exist\n");
    } else {
        int i = 0;
        while (i < 5 && p->history[i][0] != '\0') {
            i++;
        }
        if (i < 5) {
            strncpy(p->history[i], reason, sizeof(p->history[i]) - 1);
            p->history[i][sizeof(p->history[i]) - 1] = '\0';
        } else {
            for (i = 1; i < 5; i++) {
                strncpy(p->history[i - 1], p->history[i], sizeof(p->history[i - 1]) - 1);
                p->history[i - 1][sizeof(p->history[i - 1]) - 1] = '\0';
            }
            strncpy(p->history[4], reason, sizeof(p->history[4]) - 1);
            p->history[4][sizeof(p->history[4]) - 1] = '\0';
        }
    }
}

void changeConsultNum(int n, char id[], struct emp *h) {
    struct emp *p;
    p = findEmp(h, id);
    if (p != NULL) {
        p->consult_num = n;
    } else {
        printf("Employee doesn't exist\n");
    }
}

void changeLastConsult(char lastConsult[], char id[], struct emp *h) {
    struct emp *p;
    p = findEmp(h, id);
    if (p != NULL) {
        strncpy(p->last_consult, lastConsult, sizeof(p->last_consult) - 1);
        p->last_consult[sizeof(p->last_consult) - 1] = '\0';
    } else {
        printf(" Employee doesn't exist \n");
    }
}

void changeReturnWork(char returnWork[], char id[], struct emp *h) {
    struct emp *p;
    p = findEmp(h, id);
    if (p != NULL) {
        strncpy(p->return_work, returnWork, sizeof(p->return_work) - 1);
        p->return_work[sizeof(p->return_work) - 1] = '\0';
    } else {
        printf(" Employee doesn't exist \n");
    }
}

void updateEmp(struct emp *h, char id[]) {
    struct emp *p = findEmp(h, id);
    if (p == NULL) {
        printf(" Employee with id %s doesn't work here.\n", id);
    } else {
        int choice;
        char newDate[11];
        char newReason[25];
        char newId[9];
        char newName[35];
        int newConsult;
        printf(" What do you want to update?\n");
        printf(" 1. Id\n");
        printf(" 2. Name\n");
        printf(" 3. Consult Number\n");
        printf(" 4. Last Consult Date\n");
        printf(" 5. Return To Work Date\n");
        printf(" 6. Medical History\n");
        printf(" 7. None (input error)\n");
        printf(" Enter your choice: ");
        scanf("%d", &choice);
        getchar();
        switch (choice) {
            case 1:
                printf(" Enter new id: ");
                scanf("%8s", newId);
                strcpy(p->id, newId);
                break;
            case 2:
                printf(" Enter new Name: ");
                fgets(newName, sizeof(newName), stdin);
                newName[strcspn(newName, "\n")] = '\0';
                strcpy(p->name, newName);
                break;
            case 3:
                while (1) {
                    printf(" Enter new consult number: ");
                    if (scanf("%d", &newConsult) == 1) {
                        break;
                    } else {
                        printf(" You need to enter a number please.\n");
                        while (getchar() != '\n');
                    }
                }
                changeConsultNum(newConsult, id, h);
                break;
            case 4:
                printf(" Enter new last consult date: ");
                scanf("%10s", newDate);
                changeLastConsult(newDate, id, h);
                break;
            case 5:
                printf(" Enter new return to work date: ");
                scanf("%10s", newDate);
                changeReturnWork(newDate, id, h);
                break;
            case 6:
    {
        int reason_choice;
        printf(" Choose new medical reason:\n");
        printf(" 1. Work-accident\n 2. Occupational-Disease\n 3. Return-to-Work\n 4. Pre-employment\n 5. Periodic\n Choose (1-5): ");
        scanf("%d", &reason_choice);

        switch (reason_choice) {
            case 1: strcpy(newReason, "Work-accident"); break;
            case 2: strcpy(newReason, "Occupational-Disease"); break;
            case 3: strcpy(newReason, "Return-to-Work"); break;
            case 4: strcpy(newReason, "Pre-employment"); break;
            case 5: strcpy(newReason, "Periodic"); break;
            default:
                printf(" Invalid choice. Update canceled.\n");
                return;
        }
        updateHistory(h, id, newReason);
    }
    break;
            default:
                printf(" Update canceled due to input error\n");
                break;
        }
    }
}

void deleteEmp(struct emp **h, char deleted_id[]) {
    struct emp *p = *h;
    struct emp *q;

    if ((p != NULL) && strcmp(p->id, deleted_id) == 0) {
        *h = p->adr;
        free(p);
        printf(" Employee %s got deleted successfully.\n", deleted_id);
    } else {
        q = findEmp(*h, deleted_id);
        if (q != NULL) {
            p = *h;
            while (p->adr != q) {
                p = p->adr;
            }
            p->adr = q->adr;
            free(q);
            printf(" Employee %s deleted successfully.\n", deleted_id);
        } else {
            printf(" Employee with id %s doesn't work here.\n", deleted_id);
        }
    }
}

struct emp* addEmp(struct emp *h) {
    struct emp *p = (struct emp*)malloc(sizeof(struct emp));
    char choice;
    char tempID[9];

    memset(p, 0, sizeof(struct emp));

    // Get ID and check if it's unique
    while (1) {
        printf(" Enter Id: ");
        scanf("%8s", tempID);

        if (h != NULL && !unique_ID(h, tempID)) {
            printf(" ID already exists. Please enter a unique ID.\n");
        } else {
            strcpy(p->id, tempID);
            break;
        }
    }

    printf(" Enter Name: ");
    getchar();
    fgets(p->name, sizeof(p->name), stdin);
    p->name[strcspn(p->name, "\n")] = '\0';

    int num;
    while (1) {
        printf(" Enter consult number: ");
        if (scanf("%d", &num) == 1) {
            p->consult_num = num;
            break;
        } else {
            printf(" You need to enter a number please.\n");
            while (getchar() != '\n');
        }
    }

    printf(" Enter last consultation date: ");
    scanf("%10s", p->last_consult);

    printf(" does this employee have a return to work date? (y/n): ");
    scanf(" %c", &choice);
    if (choice == 'y' || choice == 'Y') {
        printf(" Enter return to work date: ");
        scanf("%10s", p->return_work);
    }

    getchar();
    for (int i = 0; i < 5; i++) {
        int reason_choice;
        printf(" History %d (enter 0 to stop):\n", i + 1);
        printf(" 1. Work-accident\n 2. Occupational-Disease\n 3. Return-to-Work\n 4. Pre-employment\n 5. Periodic\n Choose (1-5): ");
        if (scanf("%d", &reason_choice) != 1 || reason_choice == 0) {
            for (int j = i; j < 5; j++) p->history[j][0] = '\0';
            break;
        }
        switch (reason_choice) {
            case 1: strcpy(p->history[i], "Work-accident"); break;
            case 2: strcpy(p->history[i], "Occupational-Disease"); break;
            case 3: strcpy(p->history[i], "Return-to-Work"); break;
            case 4: strcpy(p->history[i], "Pre-employment"); break;
            case 5: strcpy(p->history[i], "Periodic"); break;
            default: p->history[i][0] = '\0'; i--; continue;
        }
        getchar();
    }

    printf("<------------------------------------------------------------>\n");

    if (h == NULL) {
        h = p;
    } else {
        struct emp *ptr = h;
        while (ptr->adr != NULL) ptr = ptr->adr;
        ptr->adr = p;
    }
    p->adr = NULL;

    return h;
}

void readLine(struct emp *p, char *line) {
    int i = 0;
    int k = 0;
    char num[3];

    sscanf(line + i, "%[^;]", p->id);
    i = i + strlen(p->id) + 1;

    sscanf(line + i, "%[^;]", num);
    p->consult_num = atoi(num);
    i = i + strlen(num) + 1;

    sscanf(line + i, "%[^;]", p->last_consult);
    i = i + strlen(p->last_consult) + 1;

    sscanf(line + i, "%[^;]", p->return_work);
    i = i + strlen(p->return_work) + 1;

    sscanf(line + i, "%[^;]", p->name);
    i = i + strlen(p->name) + 1;

    while (k < 5 && line[i] != '\0' && line[i] != '\n') {
        sscanf(line + i, "%[^,\n]", p->history[k]);
        i = i + strlen(p->history[k]);
        if (line[i] == ',') {
            i = i + 1;
        }
        k = k + 1;
    }

    while (k < 5) {
        p->history[k][0] = '\0';
        k = k + 1;
    }
}

struct emp* loadEmp(FILE *f) {
    struct emp *h = NULL, *p, *q;
    char line[255];

    while (fgets(line, sizeof(line), f) != NULL) {
        p = createEmp();
        readLine(p, line);
        p->adr = NULL;

        if (h == NULL) {
            h = p;
        } else {
            q = h;
            while (q->adr != NULL) {
                q = q->adr;
            }
            q->adr = p;
        }
    }
    return h;
}

void printEmp(struct emp *p) {
    printf(" Id: %s \n Name: %s \n Consults_num: %d\n Last consult date: %s \n Return to work date: %s\n", p->id, p->name, p->consult_num, p->last_consult, p->return_work);
    printf(" Medical History:\n");
    for (int h = 0; h < 5; h++) {
        if (p->history[h][0] != '\0') printf(" - reason %d: %s\n", h + 1, p->history[h]);
    }
    printf("<------------------------------------------------------------>\n");
}

void printInGrp(struct emp *h) {
    struct emp *p = h;
    int cpt = 0;
    char choice;
    printf("<------------------------------------------------------------>\n");
    while (p != NULL) {
        printEmp(p);
        cpt++;
        p = p->adr;
        if (cpt % 2 == 0 && p != NULL) {
            printf(" Show more? (y/n?): ");
            scanf(" %c", &choice);
            printf("<------------------------------------------------------------>\n");
            if (choice == 'n' || choice == 'N') break;
        }
    }
}

void saveEmp(struct emp *h, FILE *f) {
    struct emp *p = h;
    char line[300];
    int i, j;

    while (p != NULL) {
        i = 0;
        i = i + sprintf(line + i, "%s;", p->id);
        i = i + sprintf(line + i, "%d;", p->consult_num);
        i = i + sprintf(line + i, "%s;", p->last_consult);
        i = i + sprintf(line + i, "%s;", p->return_work);
        i = i + sprintf(line + i, "%s;", p->name);

        j = 0;
        while (j < 5 && p->history[j][0] != '\0') {
            i = i + sprintf(line + i, "%s,", p->history[j]);
            j = j + 1;
        }

        line[i - 1] = '\n';
        line[i] = '\0';

        fputs(line, f);
        p = p->adr;
    }
}

void addNewEmp(consultation *q, struct emp **h) {
    struct emp *p = (struct emp*)malloc(sizeof(struct emp));
    memset(p, 0, sizeof(struct emp));

    // Copy employee ID
    strncpy(p->id, q->Employee_ID, sizeof(p->id) - 1);
    p->id[sizeof(p->id) - 1] = '\0';

    // Copy employee name
    strncpy(p->name, q->Employee_Name, sizeof(p->name) - 1);
    p->name[sizeof(p->name) - 1] = '\0';

    // Set consultation number
    p->consult_num = 1;

    // Copy consultation reason to history
    strncpy(p->history[0], q->Consultation_Reason, sizeof(p->history[0]) - 1);
    p->history[0][sizeof(p->history[0]) - 1] = '\0';

    // Set last consultation date to current date
    time_t now = time(NULL);
    struct tm *tm_now = localtime(&now);
    strftime(p->last_consult, sizeof(p->last_consult), "%d/%m/%Y", tm_now);

    // Initialize return_work as empty
    p->return_work[0] = '\0';

    // Initialize adr to NULL
    p->adr = NULL;

    // Add to the list
    if (*h == NULL) {
        *h = p;
    } else {
        struct emp *ptr = *h;
        while (ptr->adr != NULL) ptr = ptr->adr;
        ptr->adr = p;
    }
}

void subAutoUpdate(struct emp *h, char id[], char reason[], char date[]) {
    updateHistory(h, id, reason);
    changeConsultNum(findEmp(h, id)->consult_num + 1, id, h);
    changeLastConsult(date, id, h);
}

void updateSingleEmp(struct emp **h, typeQueue *q, char id[], char date[]) {
    typeCell *c = q->h;

    while (c != NULL) {
        if (strcmp(c->conslt.Employee_ID, id) == 0) {
            struct emp *p = findEmp(*h, id);

            if (p == NULL) {
                addNewEmp(&(c->conslt), h);
            } else {
                subAutoUpdate(*h, id, c->conslt.Consultation_Reason, date);
            }

            return;
        }
        c = c->addr;
    }
}


int main() {
    // Files for saving/loading data
    FILE *appointmentFile, *nextDayFile, *employeeFile;

    // Initialize queues for appointments
    typeQueue todayQueue = createQueue();
    typeQueue nextDayQueue = createQueue();

    // Initialize employee records linked list
    emp *employeeRecords = NULL;

    // Temporary variables
    consultation tempConsultation;
    char userInput[10];
    int choice;
    int day, month, year;
    char *currentDate = get_date(&day, &month, &year);
    int currentTimeInt = time_int(get_time());

    // Try to load existing data
    printf("Loading existing data...\n");

    // Load employee records
    employeeFile = fopen("EmpRecords.txt", "r");
    if (employeeFile != NULL) {
        employeeRecords = loadEmp(employeeFile);
        fclose(employeeFile);
        printf("Employee records loaded successfully.\n");
    } else {
        printf("No existing employee records found. Starting with empty records.\n");
    }

    // Load today's appointments
    appointmentFile = fopen("cons.txt", "r");
    if (appointmentFile != NULL) {
        read_file_to_queue(appointmentFile, &todayQueue);
        fclose(appointmentFile);
        printf("Today's appointments loaded successfully.\n");
    } else {
        printf("No existing appointments found for today. Starting with empty queue.\n");
    }

    // Load next day's appointments
    nextDayFile = fopen("nextcons.txt", "r");
    if (nextDayFile != NULL) {
        read_file_to_queue(nextDayFile, &nextDayQueue);
        fclose(nextDayFile);
        printf("Next day's appointments loaded successfully.\n");
    } else {
        printf("No existing appointments found for next day. Starting with empty queue.\n");
    }

    printf("\n================= CLINIC MANAGEMENT SYSTEM =================\n");
    printf("Current date: %s\n", currentDate);
    printf("Current time: %s\n", get_time());
    printf("Appointments processed today: %d/%d\n", processed_count, maximum);

    // Main menu loop
    do {
        printf("\n==================== MAIN MENU ====================\n");
        printf("1. Display today's appointment queue\n");
        printf("2. Display next day's appointment queue\n");
        printf("3. Add new appointment\n");
        printf("4. Cancel appointment\n");
        printf("5. Process next appointment\n");
        printf("6. Reschedule appointment to next day\n");
        printf("7. Check automatic periodic and return-to-work appointments\n");
        printf("8. Employee record management\n");
        printf("9. Save and exit\n");
        printf("Enter your choice (1-9): ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            clear();
            continue;
        }
        clear();

        switch (choice) {
            case 1:
                printf("\n===== TODAY'S APPOINTMENT QUEUE =====\n");
                if (emptyQueue(todayQueue)) {
                    printf("No appointments scheduled for today.\n");
                } else {
                    display_queue(todayQueue);
                }
                break;

            case 2:
                printf("\n===== NEXT DAY'S APPOINTMENT QUEUE =====\n");
                if (emptyQueue(nextDayQueue)) {
                    printf("No appointments scheduled for next day.\n");
                } else {
                    display_queue(nextDayQueue);
                }
                break;

            case 3:
                printf("\n===== ADD NEW APPOINTMENT =====\n");
                add_appointment(&todayQueue, &nextDayQueue);
                break;

            case 4:
                printf("\n===== CANCEL APPOINTMENT =====\n");
                printf("Enter employee ID to cancel appointment: ");
                scanf("%s", userInput);
                clear();

                cancel_appointment(&todayQueue, userInput);
                printf("Appointment canceled (if it existed).\n");
                break;

            case 5:
                printf("\n===== PROCESS NEXT APPOINTMENT =====\n");
                if (emptyQueue(todayQueue)) {
                    printf("No appointments to process.\n");
                } else {
                    close_appointment(&todayQueue, &employeeRecords);
                }
                break;

            case 6:
                printf("\n===== RESCHEDULE APPOINTMENT =====\n");
                reschedule_manual(&todayQueue, &nextDayQueue);
                break;

            case 7:
                printf("\n===== CHECK AUTOMATIC APPOINTMENTS =====\n");
                printf("Checking for periodic visits and return-to-work appointments...\n");
                schedule_periodic_return(employeeRecords, &nextDayQueue, currentDate, currentTimeInt);
                printf("Check complete. View next day's queue to see any new appointments.\n");
                break;

            case 8:
                do {
                    printf("\n===== EMPLOYEE RECORD MANAGEMENT =====\n");
                    printf("1. View all employee records\n");
                    printf("2. Find employee by ID\n");
                    printf("3. Add new employee\n");
                    printf("4. Update employee information\n");
                    printf("5. Delete employee\n");
                    printf("6. Return to main menu\n");
                    printf("Enter your choice (1-6): ");

                    if (scanf("%d", &choice) != 1) {
                        printf("Invalid input. Please enter a number.\n");
                        clear();
                        continue;
                    }
                    clear();

                    switch (choice) {
                        case 1:
                            printf("\n===== ALL EMPLOYEE RECORDS =====\n");
                            if (employeeRecords == NULL) {
                                printf("No employee records found.\n");
                            } else {
                                printInGrp(employeeRecords);
                            }
                            break;

                        case 2:
                            printf("\n===== FIND EMPLOYEE =====\n");
                            printf("Enter employee ID: ");
                            scanf("%s", userInput);
                            clear();

                            emp *foundEmployee = findEmp(employeeRecords, userInput);
                            if (foundEmployee != NULL) {
                                printf("\nEmployee found:\n");
                                printEmp(foundEmployee);
                            } else {
                                printf("Employee with ID %s not found.\n", userInput);
                            }
                            break;

                        case 3:
                            printf("\n===== ADD NEW EMPLOYEE =====\n");
                            employeeRecords = addEmp(employeeRecords);
                            printf("Employee added successfully.\n");
                            break;

                        case 4:
                            printf("\n===== UPDATE EMPLOYEE INFORMATION =====\n");
                            printf("Enter employee ID to update: ");
                            scanf("%s", userInput);
                            clear();

                            updateEmp(employeeRecords, userInput);
                            break;

                        case 5:
                            printf("\n===== DELETE EMPLOYEE =====\n");
                            printf("Enter employee ID to delete: ");
                            scanf("%s", userInput);
                            clear();

                            deleteEmp(&employeeRecords, userInput);
                            break;

                        case 6:
                            printf("Returning to main menu...\n");
                            break;

                        default:
                            printf("Invalid choice. Please try again.\n");
                    }
                } while (choice != 6);
                choice = 0; // Reset choice to continue main menu
                break;

            case 9:
                printf("\n===== SAVING DATA AND EXITING =====\n");
                // Save today's appointments
                appointmentFile = fopen("cons.txt", "w");
                if (appointmentFile != NULL) {
                    write_queue_to_file(appointmentFile, todayQueue);
                    fclose(appointmentFile);
                    printf("Today's appointments saved.\n");
                } else {
                    printf("Error: Could not save today's appointments.\n");
                }

                // Save next day's appointments
                nextDayFile = fopen("nextcons.txt", "w");
                if (nextDayFile != NULL) {
                    write_queue_to_file(nextDayFile, nextDayQueue);
                    fclose(nextDayFile);
                    printf("Next day's appointments saved.\n");
                } else {
                    printf("Error: Could not save next day's appointments.\n");
                }

                // Save employee records
                employeeFile = fopen("EmpRecords.txt", "w");
                if (employeeFile != NULL) {
                    saveEmp(employeeRecords, employeeFile);
                    fclose(employeeFile);
                    printf("Employee records saved.\n");
                } else {
                    printf("Error: Could not save employee records.\n");
                }

                printf("Exiting program...\n");
                break;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 9);

    // Free memory before exiting
    free_Q(&todayQueue);
    free_Q(&nextDayQueue);

    // Free employee records
    emp *current = employeeRecords;
    emp *next;
    while (current != NULL) {
        next = current->adr;
        free(current);
        current = next;
    }

    return 0;
}
