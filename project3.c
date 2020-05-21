//**************************************************************************** 
//File:project3.c* 
//Author: Jose Lamuno* 
//Procedures:* main -deadlock detection algorithm which uses 4 input files with information regarding processes and resources.
//***************************************************************************

#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<sys/wait.h> 
#include<sys/time.h>

//**************************************************************************** 
//int main* 
//Author: Jose Lamuno* 
//Date 3/8/2020*
//Description: Runs a deadlock detection algorithm for P processes and R resources. Uses 4 input files with information regarding
//the processes, resources used, and resources available.
//***************************************************************************

int main() 
{ 
    //Initialize problem size variables
    int P;
    int R;
    
    //Ask the user for problem size variables and update them    
    printf("Enter the number of processes(rows): \n");
    scanf("%d", &P);
    printf("Enter the number of resources(columns): \n");
    scanf("%d", &R);

    //Feedback to the user
    printf("You chose %i processes and %i resources. \n", P, R);

    //Initialize matrices and vectors based on problem size
    int request[P][R];
    int allocation[P][R];
    int available[R];
    int resource[R];

    //Initialize variables needed for algorithm
    int marked[P];
    int prequests[P];
    int totalAvailable = 0;
    int totalMarks = 0;
    int W[R];   

    //Loop to make every process marked and with 0 resource requests(best case).
    for(int i = 0; i<P; i++)
    {
        marked[i] = 0;
        prequests[i] = 0;
    }
    
    //All code from here on is to read from each file into the matrices/vectors
    
    //Initialize filepointer
    FILE *fp;

    //Open request matrix file
    fp = fopen("request", "r");
    
    //Check if file opened succesfully and proceed with reading data into request matrix
    if (fp == NULL)
    {
        printf("request file failed to open\n");
    }
    else
    {
        printf("\nRequest matrix: \n");
        for(int i = 0; i<P; i++)
        {
            for(int j = 0; j<R; j++)
            {
                fscanf(fp, "%i", &request[i][j]);
                if (request[i][j] > 0)                            //This if statement updates the total amount of resources
                    prequests[i] = prequests[i] + request[i][j];  //requested by each process individually
                printf("%i ", request[i][j]);
                if(j == (R-1))
                    printf("\n");
            }
        }
        fclose(fp);
    }
    
    //Open allocation matrix file
    fp = fopen("allocation", "r");
    
    //Check if file opened succesfully and proceed with reading data into allocation matrix
    if (fp == NULL)
    {
        printf("Allocation file failed to open\n");
    }
    else
    {
        printf("\nAllocation matrix: \n");
        for(int i = 0; i<P; i++)
        {
            for(int j = 0; j<R; j++)
            {
                fscanf(fp, "%i", &allocation[i][j]);
                if (allocation[i][j] > 0 && marked[i] == 0)       //If statement that makes a process
                {                                                 //unmarked if they have 1 or more
                    marked[i] = 1;                                //resource requests and updates the
                    totalMarks = totalMarks + 1;                  //total amount of unmarked processes
                }   
                printf("%i ", allocation[i][j]);
                if(j == (R-1))
                    printf("\n");
            }
        }
        fclose(fp);
    }

    //Open available vector file
    fp = fopen("available", "r");
    
    //Check if file opened succesfully and proceed with reading data into available vector
    if (fp == NULL)
    {
        printf("Available file failed to open\n");
    }
    else
    {
        printf("\nAvailable matrix: \n");
        for(int i = 0; i<R; i++)
        {
            fscanf(fp, "%i", &available[i]);
            W[i] = available[i];
            printf("%i ", available[i]);
            if (available[i] > 0)                                 //If statement that checks for available
                totalAvailable = totalAvailable + available[i];   //resources at R and updates total resources available
            if(i == (R-1))
                printf("\n");
        }
        fclose(fp);
    }

    //Open resource vector file
    fp = fopen("resource", "r");
    
    //Check if file opened succesfully and proceed with reading data into resource vector
    if (fp == NULL)
    {
        printf("Resource file failed to open\n");
    }
    else
    {
        printf("\nResource matrix: \n");
        for(int i = 0; i<R; i++)
        {
            fscanf(fp, "%i", &resource[i]);
            printf("%i ", resource[i]);
            if(i == (R-1))
                printf("\n");
        }
        fclose(fp);
    }

    printf("\nTotal available resources: %i\n", totalAvailable);

    //Code from here relates to operations to detect deadlock
    
    //Main loop that traverses through the mark vector and attempts to check for deadlock between unmarked processes
    for (int i = 0; i < P; i++)
    {
        if (marked[i] == 0)                                                                  //Checks for marked processes
            printf("\nProcess %i has 0 resources allocated and is now marked.\n", i+1);
        else if (prequests[i] <= totalAvailable && prequests[i] != 0)                        //If processes are unmarked
        {                                                                                    //and Pr < W
               printf("\nAttempting to give process %i %i resource(s)...\n", i+1, prequests[i]);
               for (int j = 0; j < R; j++)
               {
                   if (W[j] >= request[i][j])                                                //Checks to see if resources
                   {                                                                         //can be given to process P
                        W[j] = W[j] - request[i][j];
                        totalAvailable = totalAvailable - request[i][j];
                        prequests[i] = prequests[i] - request[i][j];
                    }               
                }
                if (prequests[i] > 0)                                                        //If not possible the process
                    printf("The specific resource requested is not available :(\n");         //remains unmarked
        }
        if (prequests[i] == 0)                                                               //If possible the process is
        {                                                                                    //marked
            printf("\nSuccess! Process %i has 0 requests and is now marked.\n", i+1);
            marked[i] = 0;
            totalMarks = totalMarks - 1;
        }       
    }
    
    if (totalMarks == 0)                                              //If all processes are marked, no deadlock
        printf("\nCongrats, no deadlock detected\n");
    else
    {
        printf("\nOops you got a deadlock :(\n");                     //If 1+ processes are unmarked then deadlock
        for (int i = 0; i < P; i++)
        {
            if (marked[i] == 1)
                printf("Process %i is deadlocked.\n", i+1);
        }
    }
} 
