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

struct emp *findEmp(struct emp *h, char id[]) {
    struct emp *p = h;
    while (p != NULL) {
        if (p->id == id) {
            return p;
        }
        p = p->adr;
    }
    if (p == NULL) {
        return NULL;
    }
}

void updateHistory(struct emp *h, char id[], char reason[]) {
    struct emp *p = findEmp(h, id);
    if (p == NULL) {
        printf(" Employee doesn't exist \n");
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

struct emp *changeConsultNum(int n, char id[], struct emp *h) {
    struct emp *p;
    p = findEmp(h, id);
    if (p != NULL) {
        p->consult_num = n;
    } else {                           //case of nonexistent emp
        printf(" Employee doesn't exist \n");
    }
}

struct emp *changeLastConsult(char lastConsult[], char id[], struct emp *h) {
    struct emp *p;
    p = findEmp(h, id);
    if (p != NULL) {
        strncpy(p->last_consult, lastConsult, sizeof(p->last_consult) - 1);  //doesnt copy extra characters
        p->last_consult[sizeof(p->last_consult) - 1] = '\0';                 //make sure it's well terminated so it doesn't print garbage
    } else {
        printf(" Employee doesn't exist \n");
    }
}

struct emp *changeReturnWork(char returnWork[], char id[], struct emp *h) {
    struct emp *p;
    p = findEmp(h, id);
    if (p != NULL) {
        strncpy(p->return_work, returnWork, sizeof(p->return_work) - 1);
        p->return_work[sizeof(p->return_work) - 1] = '\0';
    } else {
        printf(" Employee doesn't exist \n");
    }
}

void deleteEmp(struct emp **h, char deleted_id[]) {
    struct emp *p = *h;
    struct emp *q;

    if ((p != NULL) && strcmp(p->id, deleted_id) == 0) {  // Employee at head
        *h = p->adr;
        free(p);
        printf(" Employee %s got deleted successfully.\n", deleted_id);
    } else {
        q = findEmp(*h, deleted_id);  // Find the employee
        if (q != NULL) {
            p = *h;
            while (p->adr != q) {  // Find the one before it
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
    p->adr = NULL;
    memset(p, 0, sizeof(struct emp)); //so our fields aren't filled with garbage from memory

    printf(" Enter Id: ");
    scanf("%8s", p->id);

    printf(" Enter Name: ");
    getchar();
    fgets(p->name, sizeof(p->name), stdin);
    p->name[strcspn(p->name, "\n")] = '\0';

    printf(" Enter consult number: ");
    scanf("%d", &p->consult_num);

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

struct emp* createEmp() {
    struct emp *p = (struct emp*)malloc(sizeof(struct emp));
    p->adr = NULL;
    memset(p, 0, sizeof(struct emp));
    return p;
}

void readLine(struct emp *p, char *line) {
    int i = 0;
    int k = 0;
    char num[3]; //Holds 2 digits number

    sscanf(line + i, "%[^;]", p->id);
    i = i + strlen(p->id) + 1;

    sscanf(line + i, "%[^;]", p->name);
    i = i + strlen(p->name) + 1;

    sscanf(line + i, "%[^;]", num);
    p->consult_num = atoi(num);
    i = i + strlen(num) + 1;

    sscanf(line + i, "%[^;]", p->last_consult);
    i = i + strlen(p->last_consult) + 1;

    sscanf(line + i, "%[^;]", p->return_work);
    i = i + strlen(p->return_work) + 1;

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

int main() {
    FILE *f = fopen("C:\\Users\\daass\\OneDrive\\Documents\\tp2\\EmpRecords.txt", "r");
    struct emp *h = loadEmp(f);
    fclose(f);
    //h = addEmp(h);
    char id[] = "12345678";
    deleteEmp(&h, id);
    printInGrp(h);
    return 0;
}
