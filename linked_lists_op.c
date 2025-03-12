// contains the main operation to manipulate linked lists 
#include <stdio.h>
#include <stdlib.h>
#include "linked_lists_op.h"



typedef struct node node;  /* Define node as a type */

struct node {     
    int value;
    node *addr;
};

node* p;


typedef struct cell cell;  /* Define cell as a type */

struct cell {                /*cell of a double list*/
    int value;
    cell* prev;
    cell* next;
};




/* Creates a linked list and returns a pointer to the first node */
node* crt_linked_l() {  
    node *head = NULL, *current = NULL, *last = NULL;
    int i ,n;
    
    printf("Enter the number of elements: ");
    scanf("%d", &n);

    for (i = 0; i < n; i++) { 
        /* Create a single node */
        current = (node*) malloc(sizeof(node));
        if (current == NULL) {
            printf("Memory allocation failed.\n");
            return head;
        }

        printf("Enter element %d: ", i);
        scanf("%d", &current->value);
        current->addr = NULL;  /* Initialize addr to NULL */

        /* Link the new node */
        if (head == NULL) {  
            head = current;  /* First node */
        } else {  
            last->addr = current;  /* Link to last node */
        }
        last = current;  /* Update last */
    }

    return head;
}




/*display a simple linked list given its head*/
void display_linked_list(node* q) {
    node* a = q;
    int j = 0;

    while (a != NULL) {
        printf("Element %d: %d\n", j, a->value);
        a = a->addr;
        j++;
    }
}


void access_pos(node *head ,int pos, node* *q, node* *p){
    *q=NULL;                         /*pointer to the previous cell*/
    *p= head;                       /* pointer to the current cell*/

    int cpt = 1 ;
    while (*p != NULL && cpt<pos ){        /*traverse the list*/
        cpt++;                             /*increment position counter*/
        *q = *p ;
        *p = (*p)->addr ;                /*move to next */
    }
    
}

/*insert a new node in a list at a give position*/
void insert(node **head , int pos , int val){
    node *a , *b ,*n;

    n = malloc(sizeof(node));       /*create new cell*/
    n->value = val ;               /*assign the value */
    access_pos(*head,pos,&a,&b);

    if (a == NULL)                      /*list is empty or insert at first position */
    {
        n->addr = *head ;              /*link head to it*/
        *head = n ;     
    }
    else                             /*list is not empty*/
    {
        a->addr = n ;               /*link */
        n->addr = b ;
        
    }
    
}

/*gives in q the adress of the position before value in q the address of the value */
void access_val(node *head ,int v, node* *q, node* *p){
    *q=NULL;                         /*pointer to the previous cell*/
    *p= head;                       /* pointer to the current cell*/
    int found = 0 ;                 /*control boolean*/

    while (*p != NULL && found == 0 ){        /*traverse the list*/

        if ((*p)->value == v)                /*test id the value is found*/
        {
            found = 1;                          /*update the boolean*/
        }
        else{
            *q = *p ;                      /*move to next cell*/
            *p = (*p)->addr ;
        }
        
    }
    

}




/*delete the first node containing the given value v*/
void delete_val(node* *head, int v){
    node *b, *a ;
    access_val(*head,v,&a,&b);

    if (b != NULL)             /*then the value was found and the list is not empty  */
    {   if( a == NULL){       /*the node to delete is the head*/
            *head = b->addr ;  
        } 
        
        else                  
        {
            a->addr = b->addr ;  /*link previous to next */
        }
        
        free(b);
     
    }
  
}


/*deletes the node at a given position*/
void delete_pos(node* *head, int index){
    node *b, *a ;
    access_pos(*head,index,&a,&b);

    if (b != NULL)             /*then the value was found and the list is not empty  */
    {   if( a == NULL){       /*the node to delete is the head*/
            *head = b->addr ;  
        } 
        
        else                  
        {
            a->addr = b->addr ;  /*link previous to next */
        }
        
        free(b);
     
    }
  
}


/*free allcated space*/

void free_list(node* head) {
    node* temp;
    while (head != NULL) {
        temp = head;
        head = head->addr;
        free(temp);
    }
}


/*merges two increasingly sorted lists into one sorted list */
node * merged_list(node*h1 , node *h2){
    node *h = NULL , *p , *p1 = h1 , *p2 = h2;
    while (p1!=NULL && p2 != NULL)
    {
       if (p1->value <= p2->value)
       {
         if (h == NULL)
         {
            h = p1 ;       /*smallest element */
            p = h ;
            p1 = p1->addr ;
         }
         else
         {
            p->addr = p1;
            p = p1 ;
            p1 = p1->addr ;
         }
         
         
       }
       
       else{    /*value(p1) >value(p2)*/
        if (h == NULL)
        {
            h = p2 ;       /*smallest element */
            p = h ;
            p2 = p2->addr ;
        }
        else
        {
            p->addr = p2;
            p = p2 ;
            p2 = p2->addr ;
        }

       }               
 
       
       
    }

    if (p1 == NULL)
    {
        if (h == NULL)    /*first list was initialy empty*/
        {
           h = p2 ;
        }
        else
        {
            p->addr = p2 ;  
        }
      
    }

    return h ;
    
    
}


/*doubly linked lists */

void crt_double_linked_list(cell **head , cell **tail) {  /* Creates a doubly linked list  */
    int v , i ,n;
    cell *p , *new = NULL ;                /*p points to last cell created , n points to newcell */

    *head = NULL ;                         /*initialize the head and tail*/
    *tail = NULL ; 

    printf("Enter the number of elements : ");
    scanf("%d",&n);

    if (n > 0)   /*list is not empty */
    {
        *head = malloc(sizeof(cell));               /*create the head */
        (*head)->prev = NULL ;                    /*set the previous of head to null*/

        printf("Enter value 1 : ");
        scanf("%d",&v);
        (*head)->value  = v ;
        p = *head ;
    }
    

    
    for(i=2 ; i<n+1 ; i++)
    {
        new =  malloc(sizeof(cell));
        p->next = new ;
        new->prev = p ;
        printf("Enter value %d : ",i);
        scanf("%d",&v);
        new->value  = v ;
        p = new ; 


    }
    if (n==1)
    {
       *tail = *head ;
    }
    else if (n != 0)
    {
        *tail = new ;
        (*tail)->next = NULL ;
    }
    

}

void display_double_list(cell *head){
    int cpt=1 ;
    cell *p=head ;
    while (p != NULL)
    {
        printf("Element %d : %d \n",cpt,p->value);
        cpt++;          
        p = p->next ;  /*move to next cell*/
    }
    

}