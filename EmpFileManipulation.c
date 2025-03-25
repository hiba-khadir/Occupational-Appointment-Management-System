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

void changeConsultNum(int n, char id[], struct emp *h) {
    struct emp *p;
    p = findEmp(h, id);
    if (p != NULL) {
        p->consult_num = n;
    } else {
        printf(" Employee doesn't exist \n");
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

struct emp* createEmp() {
    struct emp *p = (struct emp*)malloc(sizeof(struct emp));
    p->adr = NULL;
    memset(p, 0, sizeof(struct emp));
    return p;
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

void automaticUpdate(struct emp **h, typeQueue *q, char date[]) {
    struct emp *p;
    typeCell *c = q->h;

    while (c != NULL) {
        p = findEmp(*h, c->conslt.Employee_ID);

        if (p == NULL) {
            addNewEmp(&(c->conslt), h);
        } else {
            subAutoUpdate(*h, c->conslt.Employee_ID, c->conslt.Consultation_Reason, date);
        }

        c = c->addr;
    }
}



int main() {
    FILE *f = fopen("C:\\Users\\daass\\OneDrive\\Documents\\tp2\\EmpRecords.txt", "r");
    struct emp *h = loadEmp(f);
    fclose(f);
    h = addEmp(h);
    printInGrp(h);
    f = fopen("C:\\Users\\daass\\OneDrive\\Documents\\tp2\\EmpRecords.txt", "w");
    saveEmp(h, f);
    fclose(f);
    return 0;
}
