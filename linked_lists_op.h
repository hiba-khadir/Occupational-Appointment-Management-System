// header file for linked lists operations
#ifndef linked_lists_op
#define linked_lists_op


/*type definition*/
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
node* crt_linked_l() ;

/*display a simple linked list given its head*/
void display_linked_list(node* q) ;


/*gives in q the adress of the position before pos in q the address of pos */
void access_pos(node *head ,int pos, node* *q, node* *p);



/*insert a new node in a list at a give position*/
void insert(node **head , int pos , int val);

/*gives in q the adress of the position before value in q the address of the value */
void access_val(node *head ,int v, node* *q, node* *p);



/*delete the first node containing the given value v*/
void delete_val(node* *head, int v);

/*deletes the node at a given position*/
void delete_pos(node* *head, int index);



/*free allcated space*/
void free_list(node* head);

/* Creates a doubly linked list  */
void crt_double_linked_list(cell **head , cell **tail) ;

void display_double_list(cell *head);

/*merges two increasingly sorted lists into one sorted list */
node * merged_list(node*h1 , node *h2);

#endif
