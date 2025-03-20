#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct emp {
    char id[9];
    int consult_num;
    char last_consult[11];
    char return_work[11];
    char name[35];
    char history[5][25];
    struct emp *adr;
};

struct emp* add_employee(struct emp *head) {
    struct emp *p = (struct emp*)malloc(sizeof(struct emp));
    char choice;
    p->adr = NULL;
    memset(p, 0, sizeof(struct emp)); //so our fields aren't filled with garbage from memory

    printf("Enter Id: ");
    scanf("%8s", p->id);

    printf("Enter Name: ");
    getchar();
    fgets(p->name, sizeof(p->name), stdin);
    p->name[strcspn(p->name, "\n")] = '\0';
    //fgets because it reads spaces
    //strcspn so get pos of /n and we replace enter with \0 like the end of name, so we move to the next field.

    printf("Enter consult number: ");
    scanf("%d", &p->consult_num);

    printf("Enter last consultation date: ");
    scanf("%10s", p->last_consult);

    printf("does this employee have a return to work date? (y/n): ");
    scanf(" %c", &choice);
    if (choice == 'y' || choice == 'Y'){
    printf("Enter return to work date: ");
    scanf("%10s", p->return_work);
    }

    getchar(); //because scanf leaves \n and it messes the reading process

for (int i = 0; i < 5; i++) {
    printf("History %d (press enter to stop): ", i + 1);
    fgets(p->history[i], sizeof(p->history[i]), stdin);
    p->history[i][strcspn(p->history[i], "\n")] = '\0';

    if (p->history[i][0] == '\0') {
        for (int j = i; j < 5; j++) p->history[j][0] = '\0';
        break;
    }
}
printf("--------------------------------------------------\n");

    if (head == NULL) {
        head = p;
    } else {
        struct emp *ptr = head;
        while (ptr->adr != NULL) ptr = ptr->adr;
        ptr->adr = p;
    }
    return head;
}

struct emp* create_emp() {
    struct emp *p = (struct emp*)malloc(sizeof(struct emp));
    p->adr = NULL;
    memset(p, 0, sizeof(struct emp));
    return p;
}

void read_line(struct emp *p, char *line) {
    int i = 0;
    int k = 0;
    char num[3]; //Holds 2 digits number

    sscanf(line + i, "%[^;]", p->id); //stops at ;
    i = i + strlen(p->id) + 1; //skips to next field

    sscanf(line + i, "%[^;]", p->name);
    i = i + strlen(p->name) + 1;

    sscanf(line + i, "%[^;]", num);
    p->consult_num = atoi(num);
    i = i + strlen(num) + 1;

    sscanf(line + i, "%[^;]", p->last_consult);   //just like previous
    i = i + strlen(p->last_consult) + 1;

    sscanf(line + i, "%[^;]", p->return_work);
    i = i + strlen(p->return_work) + 1;

    while (k < 5 && line[i] != '\0' && line[i] != '\n') {
        sscanf(line + i, "%[^,\n]", p->history[k]); // copy what's in string until ,
        i = i + strlen(p->history[k]); // skip to the next reason in line
        if (line[i] == ',') {
            i = i + 1;
        }
        k = k + 1; //skip to next reason in history field
    }

    while (k < 5) {
        p->history[k][0] = '\0'; //making rest of strings in history empty
        k = k + 1;
    }
}

struct emp* load_employees(FILE *f) {
    struct emp *head = NULL, *p, *q;
    char line[255];

    while (fgets(line, sizeof(line), f) != NULL) {
        p = create_emp();
        read_line(p, line);
        p->adr = NULL;

        if (head == NULL) {
            head = p;
        } else {
            q = head;
            while (q->adr != NULL) {
                q = q->adr;
            }
            q->adr = p;
        }
    }
    return head;
}

void print_employee(struct emp *p) {
    printf(" Id: %s \n Name: %s \n Consults_num: %d\n Last consult date: %s \n Return to work date: %s\n", p->id, p->name, p->consult_num, p->last_consult, p->return_work);
    printf(" Medical History:\n");
    for (int h = 0; h < 5; h++) {
        if (p->history[h][0] != '\0') printf(" - reason %d: %s\n", h+1, p->history[h]);
    }
    printf("--------------------------------------------------\n");
}

void print_in_groups(struct emp *head) {
    struct emp *p = head;
    int cpt = 0;
    char choice;
    printf("--------------------------------------------------\n");
    while (p != NULL) {
        print_employee(p);   //print two employees at a time
        cpt++;
        p = p->adr;

        if (cpt % 2 == 0 && p != NULL) {
            printf("Show more? (y/n?): ");    //ask for user choice
            scanf(" %c", &choice);
            printf("--------------------------------------------------\n");
            if (choice == 'n' || choice == 'N') break;
        }
    }
}

int main() {
    FILE *f = fopen("C:\\Users\\daass\\OneDrive\\Documents\\tp2\\EmpRecords.txt", "r"); //here the path to the file
    struct emp *h = load_employees(f);
    fclose(f);
    h = add_employee(h);
    print_in_groups(h);
    return 0;
}
