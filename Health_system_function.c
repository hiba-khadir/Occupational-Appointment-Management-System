#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Health_system_function.h"
//o

//type definition is in the header file 
//initialization 
int processed_count = 0 ;
int maximum = 3 ;


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
    /*copy each field from c consultation to the cell */
    strcpy((*k)->conslt.Consultation_Reason, c.Consultation_Reason);    
    strcpy((*k)->conslt.Employee_Name, c.Employee_Name);
    strcpy((*k)->conslt.Consultation_Time , c.Consultation_Time);
    strcpy((*k)->conslt.Employee_ID, c.Employee_ID ) ;
    (*k)->priority = reason_priority(c.Consultation_Reason);
    
}
//assignes the fields of src to dest
void Ass_consultation_type(consultation *dest , consultation src){
    /*copy each field from c consultation to the cell */
    strcpy(dest->Consultation_Reason, src.Consultation_Reason);    
    strcpy(dest->Employee_Name, src.Employee_Name);
    strcpy(dest->Consultation_Time , src.Consultation_Time);
    strcpy(dest->Employee_ID, src.Employee_ID ) ;
    
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
    printf("Priority : %d\n",k.priority);
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


//-------------------------------helper functions --------------------------
/*returns the priority of consultation reasons from 1 to 3 . returns -1 for invalid reasons*/

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
//clear the screen 
void clear_screen() {
    system("cls");
}
/*displays the queue*/
void display_queue(typeQueue Q){

    typeCell *p = Q.h ;
    int cpt = 1 ;

    if (emptyQueue(Q))
    {
        printf("No consultations scheduled : queue is empty \n");
    }
    else
    {
        while (p != NULL) {
            for (int i = 0; i < 2 && p != NULL; i++) {
                printf("\n");
                printf(" -------------------------------- Appointment %d: -------------------------------- \n", cpt);
                display_consultation(*p);
                p = Next(p);
                cpt++;
            }
            if (p != NULL) {
                printf("\nPress any key to show more: \n");
                clear();
                getchar(); // Wait for user input
                clear_screen();
            }
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
            fields = sscanf(line,"%8[^;];%49[^;];%5[^;];%20[^\n]",temp.Employee_ID,temp.Employee_Name,temp.Consultation_Time,temp.Consultation_Reason );
            if (fields == 4 )  /*ensure reading 4 fields*/
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


//prints the content of Q into file 
void write_queue_to_file(FILE *file , typeQueue Q){

    char line[90];
    typeCell *p = Q.h ;
    int cpt = 0 ;
    
    if (!emptyQueue(Q))
    {
        while (p != NULL && cpt < maximum) //write only within the limit of the day 
        {

            sprintf(line, "%s;%s;%s;%s\n",
            p->conslt.Employee_ID,
            p->conslt.Employee_Name,
            p->conslt.Consultation_Time,
            p->conslt.Consultation_Reason );

            printf("%s\n",line);
            fputs(line,file);           //print the line to file 

            p = Next(p);
            cpt++;
        }
        
    }
    else
    {
        printf("queue is empty : nothing to write .");
    }
        
}
    
 //clear input buffer
void clear() 
{
    while ( getchar() != '\n' );
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

//returns in p and q the address of the cell containing employee_id = ID  and the previous cell respectivly 
void access_consultation(typeQueue Q ,char* ID, typeCell* *q, typeCell* *p){

    *q=NULL;                         /*pointer to the previous cell*/
    *p= Q.h;                       /* pointer to the current cell*/
    int found = 0 ;                 /*control boolean*/

    while (*p != NULL && found == 0 ){        /*traverse the list*/

        if (strcmp((*p)->conslt.Employee_ID,ID) == 0 )        /*test if the appointement is found*/
        {
            found = 1;                          /*update the boolean*/
        }
        else{
            *q = *p ;                      /*move to next cell*/
            *p = (*p)->addr ;
        }
        
    }
}

//deletes a cell of the queue given a pointer 
void delete_cell(typeQueue *Q,typeCell *prev , typeCell *deleted_c){

    if (deleted_c != NULL)             /*the value was found and the list is not empty  */
    {  
         if( prev == NULL){       /*the Cell to delete is the head*/
            Q->h = deleted_c->addr ;  
        } 
        
        else                  
        {
            Ass_addr(&prev,Next(deleted_c));   /*link previous to next */
        }
        
        free(deleted_c);
     
    }
}


int size_of_queue(typeQueue Q){ //counts the number of appointments in the queue for the day 

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
void assign_time(typeQueue Q , consultation c ,char **min_time , char **max_time , int current_time){  

    //variables
    typeCell   *succ , *p;
    int pred_time , succ_time ,time_slot;
    int priority = reason_priority(c.Consultation_Reason);
    int found_time = 0 ;


    if (emptyQueue)
    {
        strcpy(c.Consultation_Time,"8h30"); //first time slot
        strcpy(*min_time,"8h30");
        max_time = NULL ;

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
                    strcpy(*min_time,time_string(pred_time + time_slot)); 
                    strcpy(*max_time,time_string(succ_time - time_slot));
                }
                

                p = Next(p);
            }
            
        }

        if (p == NULL || Next(p) == NULL)  //p is the tail or c has the lowest priority
        {
            pred_time = time_int(Q.t->conslt.Consultation_Time) ;
            strcpy(*min_time,time_string(pred_time + 20));

            *max_time = NULL ;
        }           
    }
}


//--------------------------------------------------------------------------------------------------------

//---------------------------------------QUEUE FUNCTIONALITIES----------------------------------------------

/* deletes an appointment of the queue given the employee iD  */
void cancel_appointment(typeQueue *Q , char* ID){
    typeCell *b, *a ;

    access_consultation(*Q,ID,&a,&b);
    delete_cell(Q,a,b);
    printf("Appointment Cancelled . \n");
  
} 


//add an appointment from the user's input 
void add_appointment(typeQueue *Q , typeQueue *Next_day_Q ){
    
    consultation temp ;  /*store data temporarly */
    int reason ,valid_choice = 0 , valid_time = 0 , option  ,  time ;


    if (size_of_queue(*Next_day_Q) >= maximum )
    {
        printf("Unable to add an appointment today: the rescheduling limit has been reached.\n");
        printf("Do you want to continue ? (appointment with the lowest priority will be discarded)\n");
        printf("    1- add and discared lowest priority \n");
        printf("    2- Cancel addition \n");
        clear();
        scanf("%d", &option);

        switch (option)
        {
        case 1:
            goto adding ;
            break;
        
        default:
            printf("Addition aborted .\n");
            break;
        }
        clear();
    }
    else
    {
adding:
        printf("------------------------------New Appointement-----------------------------\n\n");
        printf("Employee's ID : ");
        scanf("%s",temp.Employee_ID) ;   
        printf("\n");
        clear();

        printf("Employee's Name : "); 
        fgets(temp.Employee_Name, sizeof(temp.Employee_Name), stdin);    // handles the spaces in the name

        int len = strlen(temp.Employee_Name);      //lentgh of the name 

        if (len > 0 && temp.Employee_Name[len - 1] == '\n') {   //remove new lines

            temp.Employee_Name[len - 1] = '\0';
        }
        clear();

        do
        {
            printf("Consultation Time (ex : 9h20) : ");
            printf("\n");
            scanf("%s",temp.Consultation_Time) ;  
            time = time_int(temp.Consultation_Time); 
            
            if (time > 0 ) //valid time 
            {
                valid_time = 1 ; 
            }
            else
            {
                printf("Invalid time \n");
            }
            
             
            clear();

        } while (!valid_time);
        

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
    

    if (!full_queue_day(*Q))
    {
        enqueue(Q,temp);          // add based on priority 
    }
    
    else{
        reschedule(Q,Next_day_Q,temp);
    }   
     
    }
    
}



//reschedules by moving least priority appointment to next day  
void reschedule(typeQueue *Q,typeQueue *Next_day_Q,consultation c){  

    consultation rescheduled  ;
    typeCell *b = NULL, *a = Q->h;

    if (emptyQueue(*Q))
    {
        if (processed_count >= maximum) // max appointments were closed earlier
        {
            enqueue(Next_day_Q,c);
        }
        else
        {
            printf("no need to reschedule : \n");
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




void schedule_periodic_return(emp *head , typeQueue *Next_day_Q ,char* current_date ,int current_time){

    //variables
    char date_buffer[11] , *min_time , *max_time;
    int d,m,y ,current_d , current_m , current_y;
    emp *p = head;
    consultation temp ;

    //initialization
    min_time = malloc(6*sizeof(char));
    max_time = malloc(6*sizeof(char));

    //traverse the list to find periodics and return to work
    while (p!=NULL)
    {
        
        if (strcmp(p->return_work,current_date) == 0) //compare the return to work date with the current date
        {
            //initialize the appointment info
            strcpy(temp.Consultation_Reason,"Return-to-Work");
            strcpy(temp.Employee_ID,p->id);
            strcpy(temp.Employee_Name,p->name);
            assign_time(*Next_day_Q,temp,&min_time,&max_time,current_time);

            //schedule it for the next day 
            enqueue(Next_day_Q,temp);

        }
        
        strcpy(date_buffer,p->last_consult);
        sscanf(date_buffer,"%d/%d/%d",&d,&m,&y); //parse the date string and get int values
        sscanf(current_date,"%d/%d/%d",&current_d,&current_m,&current_y);



        if (d == current_d && m == current_m && y == current_y - 1)
        {
            strcpy(temp.Consultation_Reason,"Periodic");
            strcpy(temp.Employee_ID,p->id);
            strcpy(temp.Employee_Name,p->name);
            assign_time(*Next_day_Q,temp,&min_time,&max_time,current_time);

            //schedule it for the next day 
            enqueue(Next_day_Q,temp);
        }
        
        p = p->adr ;
    }
    free(min_time);
    free(max_time);

}


//closes an appointment appointment and update the corresponding employee record
void close_appointment(typeQueue *Q,emp *head){

    char choice ,ID[9];
    int option ;
    consultation temp ;
    typeCell *a ,*b ;

    printf("Choose an option : \n");
    printf("    1- close current appointment (highest priority appointment)\n");
    printf("    2- close an appointment by employee ID \n");
    clear();
    scanf("%d",&option);

    switch (option)
    {
    case 1:
        dequeue(Q,&temp);  //remove highest priority appointment 
        break;
    
    default:
        printf("Enter employee ID : \n");
        scanf("%8s",ID);
        access_consultation(*Q,ID,&a,&b); //find the consultation to close 
        Ass_consultation_type(&temp,a->conslt); //store it in temp
        delete_cell(Q,a,b);  //delete the appointment 
        
        break;
    }


    updateHistory(head,temp.Employee_ID,temp.Consultation_Reason);  //update the history of last 5 visits in EmpRecord
   
    if (strcmp(temp.Consultation_Reason,"Pre-employement") == 0 ) //if it is a pre-employment visit 
    {
        printf("Do you want to add this employee's record to the system ? ( enter \"Y\" for yes )\n ");
        scanf("%c",&choice);

        if (choice == 'y' || choice == 'Y')
        {
            addNewEmp(&temp,&head); //add the new employee to the record 
        }
        printf("Appointment closed . \n");
    
    }
    
    processed_count++ ;

}


//reschedules an appointment given its ID from the stdin to next day 
void reschedule_manual(typeQueue *Q , typeQueue *Next_day_queue){

    char ID[9];
    typeCell *pred , *rescheduled;  //pred points to the previous of reschedule
    consultation temp ;
    int option ;

    if (!emptyQueue(*Q))    
    {

enter_ID:
        printf("Enter ID of the employee to reschedule his appointment to next day : ");
        scanf("%8s",ID);    
        access_consultation(*Q,ID,&pred,&rescheduled);

        //linking  
        if (!pred)  //the head is rescheduled
        {
            printf("rescheduling the head : %s   , rescheduled : %s \n",Q->h->conslt.Employee_ID ,rescheduled->conslt.Employee_ID);
            dequeue(Q,&temp);   //dequeue the head 
            enqueue(Next_day_queue,temp);  //push it to next day queue 
        
        }
        else if (!rescheduled)   //no such ID 
        {
            printf("appointment not found : choose an option\n");
            printf("    1- Retype the ID ( in case of typing mistake ) \n");
            printf("    2- Add an appointment to Next day  \n");

            scanf("%d",&option);

            switch (option)
            {
            case 1:
                goto enter_ID ;   //ask the user to enter an ID again  
                break;

            case 2:
                add_appointment(Q,Next_day_queue); //add an appointment to the next day 
                break;

            default:
                printf("Invalid choice : rechedulement aborted .\n");
                break;
            }
        }
        else  //pred and rescheduled are not null 
        {
            printf("rescheduling %s \n",rescheduled->conslt.Employee_ID);
            enqueue(Next_day_queue,rescheduled->conslt);
            Ass_addr(&pred,Next(rescheduled));   //link the previous of rescheduled to its next 
            free(rescheduled);
        }
    }
    else //queue is empty
    {
        printf("Queue is empty : no appointments to reschedule .\n");
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
void enqueue(typeQueue *Q , consultation new_conslt){   
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

//dequeue the highest priority element in the queue and store the information in dequeued_consultation
void dequeue(typeQueue *Q , consultation *dequeued_conslt ){
    typeCell *temp ;

    if (!emptyQueue(*Q))
    {
        temp =  (*Q).h ;                      /*temporary save for the head*
         /*copy each field from the head  consultation to the dequeued consultation*/ 
        strcpy(dequeued_conslt->Consultation_Reason,consultation_info(Q->h).Consultation_Reason); 
        strcpy(dequeued_conslt->Employee_ID,consultation_info(Q->h).Employee_ID); 
        strcpy(dequeued_conslt->Employee_Name,consultation_info(Q->h).Employee_Name); 
        strcpy(dequeued_conslt->Consultation_Time,consultation_info(Q->h).Consultation_Time); 
        
        Q->h = Next(Q->h);                        /*move head to next */
        free(temp);                               /*free the dequeued element*/
    }
    else {
        printf("ERROR : QUEUE IS EMPTY. CANNOT DEQUEUE \n");
    }

}

//------------------------------------------------------------------------------------------------------------------



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
                getchar();
                printf(" Enter new medical reason: ");
                fgets(newReason, sizeof(newReason), stdin);
                newReason[strcspn(newReason, "\n")] = '\0';
                updateHistory(h, id, newReason);
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
    memset(p, 0, sizeof(struct emp));

    printf(" Enter Id: ");
    scanf("%8s", p->id);

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
        printf(" History %d (press enter to stop): ", i + 1);
        fgets(p->history[i], sizeof(p->history[i]), stdin);
        p->history[i][strcspn(p->history[i], "\n")] = '\0';
        if (p->history[i][0] == '\0') {
            for (int j = i; j < 5; j++) p->history[j][0] = '\0';
            break;
        }
    }
    printf("<------------------------------------------------------------>\n");

    if (h == NULL) {
        h = p;
    } else {
        struct emp *ptr = h;
        while (ptr->adr != NULL) ptr = ptr->adr;
        ptr->adr = p;
    }
    return h;
}

void readLine(struct emp *p, char *line) {

    int i = 0;
    int k = 0;
    char num[3];

    sscanf(line + i, "%8[^;]", p->id);
    i = i + strlen(p->id) + 1;

    sscanf(line + i, "%2[^;]", num);
    p->consult_num = atoi(num);
    i = i + strlen(num) + 1;

    sscanf(line + i, "%21[^;]", p->last_consult);
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
    struct emp *h = NULL , *p, *q;
    char line[255];

    while (fgets(line, sizeof(line), f) != NULL) {
        printf("line : %s\n",line);
        p = createEmp();
        readLine(p, line);
        printf("fields read : %s ,%s ,%s\n",p->id,p->name,p->last_consult);
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

    strncpy(p->id, q->Employee_ID, sizeof(p->id) - 1);
    p->id[sizeof(p->id) - 1] = '\0';

    strncpy(p->name, q->Employee_Name, sizeof(p->name) - 1);
    p->name[sizeof(p->name) - 1] = '\0';

    p->consult_num = 1;

    strncpy(p->history[0], q->Consultation_Reason, sizeof(p->history[0]) - 1);
    p->history[0][sizeof(p->history[0]) - 1] = '\0';

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
