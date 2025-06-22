#Occupational Appointment Management System

A C-based console application for managing employee medical consultations within an occupational health context. It prioritizes appointments, tracks employee health history, and handles scheduling constraints efficiently using a dynamic queue system.

 Developed collaboratively by **Hibatallah Khadir** and **Daas Maram**.


##  Project Structure

appointment-management-system/
┣  main.c # Program entry point and user interface

┣  functions.c # functions source code

┣  functions.h # header file

┗  README.md # Project documentation


## Key Features

- Priority-based scheduling based on consultation reason
- Daily capacity handling with automatic rescheduling
- Time slot assignment depending on priority
- Employee medical record tracking with history
- Linked list implementation of a priority queue
- File persistence for queue and employee data
- System time and date integration

---

##  How to Compile & Run

```bash
gcc main.c functions.c -o health_manager
./health_manager

 Note: This project is built for Windows (windows.h). Adjustments may be needed for Linux.

Time & Date Handling
Time: HH:MM format (as string), or HHMM (as integer)
Date: DD/MM/YYYY

File I/O
Queue state is saved and reloaded from files
Employee records are persisted between sessions

 Contributors
Hibatallah Khadir
Daas Maram 

📜 License
This project is intended for educational and academic use. Contact contributors for collaboration or reuse permissions.
