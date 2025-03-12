#include <stdio.h>
#include <stdlib.h>


typedef struct cell typeCell;   /*type of an element in the list*/   

struct cell {          
    consultation conslt;            
    typeCell *addr;                 /*address of next*/
};



/*dynamic implementation of the queue*/
typedef struct typeQueue typeQueue ;  /*define queue as a type */

struct typeQueue
{
    typeCell *h ;                   /*pointer to the head of the queue*/
    typeCell *t ;                   /*pointer to the head of the queue*/
}; 





/*Consultation type definitin - elements inside the queue -*/

/* define a type of consultation reasons will be set to {0,1,2,3,4} according to thier priority 
                      where the highest priority is 4 lowest is 0  */

typedef enum {
    periodic_examination,
    pre_employment,
    return_to_work,
    occupational_disease,
    work_accident
}  reason;



typedef struct consultation consultation ;
 struct consultation
 {
    char Employee_ID[8];
    char Employee_Name[50];            
    char Consultation_Time[5];         /*in the format HH:MM*/
    reason Consultation_Reason ;     /* one of the reasons in reason type*/
 };
 


typeCell* crt_linked_l() {  
    typeCell *head = NULL, *current = NULL, *last = NULL;
    int i ,n;
    
    printf("Enter the number of elements: ");
    scanf("%d", &n);

    for (i = 0; i < n; i++) { 
        /* Create a single typeCell */
        current = (typeCell*) malloc(sizeof(typeCell));
        if (current == NULL) {
            printf("Memory allocation failed.\n");
            return head;
        }

        printf("Emloyee %d",i);
        printf("Enter name :", i);
        scanf("%s", &current->conslt.Employee_Name);
        printf("\n");

        printf("Enter  ID ", i);
        scanf("%s", &current->conslt.Employee_ID);
        printf("\n");
        
        printf("Enter  consultation Time ", i);
        scanf("%s", &current->conslt.Consultation_Time);
        printf("\n");

        printf("Enter  consultation Time ", i);
        scanf("%d", &current->conslt.Consultation_Reason);
        printf("\n");



        current->addr = NULL;  /* Initialize addr to NULL */

        /* Link the new typeCell */
        if (head == NULL) {  
            head = current;  /* First typeCell */
        } else {  
            last->addr = current;  /* Link to last typeCell */
        }
        last = current;  /* Update last */
    }

    return head;
}

void display_consultation(typeCell *k){
    char *name = k->conslt.Employee_Name ; 
    
    printf("Employee ID : %s \n", k->conslt.Employee_ID);

    printf("Employee Name : ");

    while(*name != '\0'){            /*display the name letter by letter*/
        printf("%c",*name);
        name++ ;
    }
    printf("\n");

    printf("Consultation Time : %s \n", k->conslt.Consultation_Time);
    printf("Concultation Reason : %s" , k->conslt.Consultation_Reason);

}

typeQueue createQueue(){
    typeQueue Q ;
    Q.h =  NULL;
    Q.t =  NULL;            /*initialize the queue as empty */

    return  Q ;
}

int main(){
    typeCell priority_Q ;
    priority_Q = crt_linked_l();

    
}