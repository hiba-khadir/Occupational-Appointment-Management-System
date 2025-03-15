-------------------------------------------Occupational Health Medical Visit Management------------------------------------------------

An occupational health service at an industrial company needs to automate the management 
of medical consultations for its employees (10 consultations per day). The service uses two 
text files EmpRecords.txt and Consultations.txt to store information about employees and 
day's scheduled consultations.

The file EmpRecords.txt contains, for each employee : 


        • Employee ID: Unique identifier (up to 8 characters) 
        
        • Full Name
        
        
        • Total Number of Consultations 
        
        • Last consultation Date: Format DD/MM/YYYY 
        
        • Return to work Date (optional): Format DD/MM/YYYY  
        
        • History: A list of previous consultation reasons (5 at most) 


The file Consultations.txt contains details for each consultation:  

        • Employee ID 
        
        • Employee Name  
        
        • Consultation Time 
        
        • Consultation Reason: possible reasons are: 
        
                Work accident: for employees who have suffered an accident at work. 
           
                Occupational disease: for employees illnesses linked to workplace exposures. 
                
                Return-to-work visit: for employees returning to work after an accident or illness. 
                
                Pre-employment visit: for initial health assessment required before hiring. 
                
                Periodic examination: for regular check-ups to monitor employees' well-being. 

        
The system implements functionalities to create a prioritized appointment queue for 
the day based on the consultations file, with priority assigned according to the consultation 
reason, manage appointments, update employees’ records and schedule next-day 
appointments.


the repository contains :
         
          queue_ops.c , queue_ops.h : source and header file for priority queue related functions.
          
          lists_ops.c , lists_ops.h : source and header file for employees' records related functions.
          
          Consultations.txt , EmpRecord.txt : text file from which the data well be read and written to .

