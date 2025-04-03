#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Health_system_function.h"


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


    //load existing data
    printf("Loading existing data...\n");

    // Load employee records
    employeeFile = fopen("EmpRecords.txt","r");

    if (employeeFile != NULL) {

        employeeRecords = loadEmp(employeeFile);
        printf("Employee records loaded successfully.\n");
        fclose(employeeFile);

    }
    else {
        printf("No existing employee records found. Starting with empty records.\n");
    }

    // Load today's appointments
    appointmentFile = fopen("Consultations.txt", "r");

    if (appointmentFile != NULL) {

        read_file_to_queue(appointmentFile, &todayQueue);
        fclose(appointmentFile);
        printf("Today's appointments loaded successfully.\n");

    }
    else {
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
                  close_appointment(&todayQueue, employeeRecords);
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
                appointmentFile = fopen("Consultations.txt", "w");
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
