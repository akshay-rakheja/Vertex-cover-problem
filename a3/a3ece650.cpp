#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include<vector>

using namespace std;


/// Entry point of process C
int ProcessC(void) {
    while (!cin.eof()) 
    {
        string line;
        getline(cin, line);
        if (line.size () > 0)
        {
            cout << line << endl;
        }
    }
    return 0;
}

int main (int argc, char **argv) 
{

    vector<pid_t> kids;
    pid_t child_pid;   

    // create pipes
    int RtoA1[2];
    pipe(RtoA1);

    int A1toA2[2];
    pipe(A1toA2);
    
   //Entering Rgen fork
    child_pid = fork();
    if (child_pid == 0) 
    {
        // redirect stdout to the pipe
        dup2(RtoA1[1], STDOUT_FILENO);
        close(RtoA1[0]);
        close(RtoA1[1]);
        execv("rgen", argv);
        perror ("Error: Unable to execute rgen");
        return 1;
    } 
    else if (child_pid < 0) 
    {
        cerr << "Error: Unable to fork rgen." << endl;
        return 1;  
    }
    kids.push_back(child_pid);
 
    // Entering A1 fork
    child_pid = fork();
    if (child_pid == 0) 
    {
        // Redirecting input to STDIN_FILENO
        dup2(RtoA1[0], STDIN_FILENO);
        close(RtoA1[1]);
        close(RtoA1[0]);

        // Redirecting Output to STDOUT_FILENO
        dup2(A1toA2[1], STDOUT_FILENO);
        close(A1toA2[0]);
        close(A1toA2[1]);
        execv("a1ece650.py", argv); //executing python file
        perror ("Error: Unable to execute a1ece650.py"); //error handling
        return 1;
    } 
    else if (child_pid < 0) 
    {
        cerr << "Error: Unable to fork a1." << endl; //error handling
        return 1;  
    }
    kids.push_back(child_pid); //pushing kid to vector of kids

     
    // Entering a2 fork  
    child_pid = fork();
    if (child_pid == 0) 
    {
        // Redirecting input to STDIN_FILENO
        dup2(A1toA2[0], STDIN_FILENO);
        close(A1toA2[1]);
        close(A1toA2[0]);
        execv("a2ece650", argv);
        perror ("Error: Unable to execute a2ece650.");
        return 1;
    } 
    else if (child_pid < 0) 
    {
        cerr << "Error: Unable to fork a2" << endl;
        return 1;  
    }
    kids.push_back(child_pid); 

    // Entering a3 fork
    child_pid = fork();
    if (child_pid == 0) 
    {
        // Redirecting Output to STDOUT_FILENO
        dup2(A1toA2[1], STDOUT_FILENO);
        close(A1toA2[0]);
        close(A1toA2[1]);
        
        return ProcessC();
    } 
    else if (child_pid < 0) {
        cerr << "Error: Unable to fork a3" << endl;
        return 1;  
    }
    kids.push_back(child_pid);

    int status;
    wait(&status);

    // send kill signal to children processes
    for (pid_t k : kids) 
    {
        int Childstatus;
        kill (k, SIGTERM);
        waitpid(k, &Childstatus, 0);

    }

    return 0;
}
