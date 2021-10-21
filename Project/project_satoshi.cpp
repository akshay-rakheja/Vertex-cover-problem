// mkdir build && cd build && cmake ../ && make
// cat ../inputs/5-10-15_graphs.in | ./prjece650 -o
// cat ../inputs/ALL_graphs.in | ./prjece650 > log_optim.log 
// cat ../inputs/20v_to_50v_graphs.in | ./prjece650 -o -i
// cat ../inputs/5-10-15_graphs.in | ./prjece650 > log_final_chk.log

#include <minisat/core/SolverTypes.h>
#include <minisat/core/Solver.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <regex>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>

#include <climits>
#include <algorithm>
#include <chrono>

#include "cnf_sat_refactor.cpp"

#define handle_error(msg) \
        do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define handle_error_en(en, msg) \
        do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

using namespace std;

string NAME = "0";
int OUT_TO_FILE = 0;
int LOG_EN = 0;

struct job 
{
  int vertices;
  vector< pair <int,int> > edges;
};


pthread_mutex_t res_queue1_mut = PTHREAD_MUTEX_INITIALIZER;  //mutex between cnf_sat and IO_handler
pthread_mutex_t res_queue2_mut = PTHREAD_MUTEX_INITIALIZER;  //mutex between approx_vc_1 and IO_handler
pthread_mutex_t res_queue3_mut = PTHREAD_MUTEX_INITIALIZER;  //mutex between approx_vc_2 IO_handler
pthread_mutex_t task_queue1_mut = PTHREAD_MUTEX_INITIALIZER;  //mutex between cnf_sat and IO_handler
pthread_mutex_t task_queue2_mut = PTHREAD_MUTEX_INITIALIZER;  //mutex between approx_vc_1 and IO_handler
pthread_mutex_t task_queue3_mut = PTHREAD_MUTEX_INITIALIZER;  //mutex between approx_vc_2 IO_handler

list<job*> jq1, jq2, jq3;  // job queue 

list<string> q1, q2, q3; // result queue 

bool check_valid_inp(int v, vector< pair<int,int> > edges) 
{
    int e_size = edges.size();
    
    for(int i=0; i<e_size; i++)
    //for ( auto& e : edges) 
    {
        if (v <= edges[i].first || v <= edges[i].second || edges[i].first < 0 || edges[i].second < 0) 
        {
            cerr << "Error: Attempted to add edge to vertex that does not exist"<< endl;
            return false;
        }

        if (edges[i].first == edges[i].second) 
        {
            cerr << "Error: Cannot add edge from vertex to itself"<< endl;
            return false;
        }
    }
    return true;
}

std::string print_vector(std::vector<int> result_paths) {
    std::string result;
    bool first = true;
    for (auto& v : result_paths) {
        if (first) {
            first = false;
        }
        else {
            result += ",";
        }
        result += std::to_string(v);  
    }
    return result;
}

vector< vector<int> > create_adj_matrix(int v, vector< pair<int,int> > edges) 
{
    
    // initialize v x v matrix
    vector< vector<int> > graph(v);

    int i =0;
    while(i<v)
    {
        graph[i].resize(v);
        i++;
    }
    // for ( int i = 0 ; i < v ; i++ )
    //     graph[i].resize(v);

    // add edges between vertices
    int e_size= edges.size();
    for(int i = 0; i< e_size;i++)
//    for ( auto& e : edges) 
    {
        graph[edges[i].first][edges[i].second] = 1;
        graph[edges[i].second][edges[i].first] = 1;
    }
    return graph;
}


string approx_vc_1(int v, vector< pair<int,int> > edges) 
{
    
    if (edges.empty()) 
    {
        return "";
    }

    if (!check_valid_inp(v, edges)) 
    {
        return "";
    }


    vector<int> v_cover;
    vector< vector<int> > graph = create_adj_matrix(v, edges);

    //while(true) 
    for(;true;)
    {
        // pick highest degree vertex
        int vertex = -1;
        int max_degree = 0;
        unsigned int r = 0;
        while(r < graph.size())
        {
            int v_deg = 0;
            int g_size = graph[r].size();
            //            for (auto& n : graph[r])
            for(int i = 0; i < g_size; i++)

            {
                v_deg += graph[r][i];
            }             
            if (v_deg > max_degree) 
            {
                vertex = r;
                max_degree = v_deg;
            }

            r++;
        }
        
        // for (unsigned int r = 0; r < graph.size(); r++) 
        // {
        //     int v_deg = 0;
        //     for (auto& n : graph[r])
        //     {
        //         v_deg += n;
        //     }             
        //     if (v_deg > max_degree) 
        //     {
        //         vertex = r;
        //         max_degree = v_deg;
        //     }
        
        
        // break if no edges remain (ever vertex has degree of 0)
        if (vertex < 0) 
        {
            break;
        }

        // remove edges incident to vertex
        unsigned int i=0;
        while(i < graph.size())
        {
            graph[i][vertex] = 0; // set column to 0
            graph[vertex][i] = 0; // set row to 0
            
            i++;
        }

        // for (unsigned int i=0; i < graph.size(); i++) 
        // {
        //     graph[i][vertex] = 0; // set column to 0
        //     graph[vertex][i] = 0; // set row to 0
        // }

        // add vertex to cover
        v_cover.push_back(vertex);

    }
    sort(v_cover.begin(), v_cover.end());

    return print_vector(v_cover);
}


string approx_vc_2(int v, vector< pair<int,int> > edges) 
{
    if (!check_valid_inp(v, edges)) 
    {
        return "";
    }

    if (edges.empty()) 
    {
        return "";
    }

    vector<int> v_cover;
    vector< vector<int> > graph = create_adj_matrix(v, edges);

    //while(true) 
    for(;true;)
    {
        // find an edge E = <v1, v2>
        int v1;
        int v2;
        int found_flag = 0;
        int r = 0;
        while(r < v)
        {   
            int c = 0;
            while(c<v)
            {
                if (graph[r][c] == 1) 
                {
                    v1 = r;
                    v2 = c;
                    found_flag = 1;
                    break;
                } 
                c++;
            }
            // for (int c=0; c < v; c++) 
            // {
            //     if (graph[r][c] == 1) 
            //     {
            //         v1 = r;
            //         v2 = c;
            //         found_flag = true;
            //         break;
            //     } 
            // }
            if (found_flag)
            {
                break; // exit first for loop
            } 
            r++;
        }
        // for (int r=0; r < v; r++) 
        // {
        //     for (int c=0; c < v; c++) {
        //         if (graph[r][c] == 1) {
        //             v1 = r;
        //             v2 = c;
        //             found_flag = true;
        //             break;
        //         } 
        //     }
        //     if (found_flag)
        //     {
        //         break; // exit first for loop
        //     } 
        // }
    
        // exit while loop if no edges remain
        if (!found_flag) 
        {
            break;
        }

        // add vertices to vertex cover
        v_cover.push_back(v1);
        v_cover.push_back(v2);

        // remove edges incident to vertices
        unsigned int i=0;
        while(i < graph.size())
        {
            graph[i][v1] = 0; // set column to 0
            graph[i][v2] = 0; // set column to 0 
            graph[v1][i] = 0; // set row to 0
            graph[v2][i] = 0; // set row to 0       
            
            i++;
        }
        // for (unsigned int i=0; i < graph.size(); i++) 
        // {
        //     graph[v1][i] = 0; // set row to 0
        //     graph[v2][i] = 0; // set row to 0       
        //     graph[i][v1] = 0; // set column to 0
        //     graph[i][v2] = 0; // set column to 0     

        // }
    }

    sort(v_cover.begin(), v_cover.end());

    return print_vector(v_cover);
}

vector< pair<int,int> > parse(string s) 
{
    pair<int, int> edge;
    vector< pair<int,int> > result;
    
    // using regex
    try {
        regex re("-?[0-9]+"); //match consectuive numbers, matches lazily
        sregex_iterator next(s.begin(), s.end(), re);
        sregex_iterator end;
//        while (next != end) 
        for(;next!=end;)
        {
            smatch match1;
            smatch match2;
            
            match1 = *next;
            next++;
            // iterate to next match
            if (next != end) 
            {
                match2 = *next;
                edge.first = stoi(match1.str());
                edge.second = stoi(match2.str());
                result.push_back(edge);
                next++;
            }
        } 
    } 
    catch (regex_error& e) 
    {
        result.clear();
    }

    return result;
}


string cnf_sat_vc_solve(int v, vector< pair<int,int> > e)
{
    string result_;
    VertexCover v_cover = VertexCover(v, e);
    result_ = v_cover.find_minimum();

    return result_;
}


void* IO_func(void* args) 
{
    char cmd;
    int vertices = 0;
    string edges_input;
    vector< pair<int,int> > parsed_edges;

    
    //while(cin >> cmd)
    for(;cin>>cmd;)
    {    
        if(cmd == 'V'|| cmd =='v')
        {
            cin >> vertices;
                // std::cout << "V " << vertices << std::endl;    
            if (vertices < 0) 
            {
                cerr << "Error: Incorrect value for vertices entered" << endl;
                vertices = 0;
            }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

        }
        else if (cmd == 'E'|| cmd =='e')
        {
            cin >> edges_input;
                // std::cout << "E " << edges_input << std::endl;
            parsed_edges = parse(edges_input);

            struct job* incoming_job;

            // add to queue 3
            incoming_job = new job;
            incoming_job->vertices = vertices;
            incoming_job->edges = parsed_edges;
            pthread_mutex_lock (&task_queue3_mut);
            jq3.push_back(incoming_job);
            pthread_mutex_unlock (&task_queue3_mut);


            // add to queue 2
            incoming_job = new job;
            incoming_job->vertices = vertices;
            incoming_job->edges = parsed_edges;
            pthread_mutex_lock (&task_queue2_mut);
            jq2.push_back(incoming_job);
            pthread_mutex_unlock (&task_queue2_mut);

            // add to queue 1
            incoming_job = new job;
            incoming_job->vertices = vertices;
            incoming_job->edges = parsed_edges;
            pthread_mutex_lock (&task_queue1_mut);
            jq1.push_back(incoming_job);
            pthread_mutex_unlock (&task_queue1_mut);

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

        }
        else
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cerr << "Error: command not recognized" << endl;
        
        }
    }
    return NULL;
}


void* output_func(void* args) {
    
    int exit_flag = 0;

    //while(true)
    for(;true;) 
    {  
        pthread_mutex_lock (&res_queue1_mut);
        pthread_mutex_lock (&res_queue2_mut);
        pthread_mutex_lock (&res_queue3_mut);

        // write results to console in required order
        for(;!q1.empty() && !q2.empty() && !q3.empty();) 
        {
            string result;
            
            result = q1.front();
            q1.pop_front();
            cout << result << endl;

            result = q2.front();
            q2.pop_front();
            cout << result << endl;

            result = q3.front();
            q3.pop_front();
            cout << result << endl;
        }

        if ( (*((bool*)args+1) && q1.empty() && q2.empty() && q3.empty()) || *((bool*)args+2) ) // check if cnf_sat ignore flag was passed as argument
        { 
            exit_flag = 1;
        }

        pthread_mutex_unlock (&res_queue1_mut);
        pthread_mutex_unlock (&res_queue2_mut);
        pthread_mutex_unlock (&res_queue3_mut);

        if(exit_flag) 
        {
            break;
        }
    }

    return NULL;
}


void* calc_cnf_sat_vc_solve(void* args) 
{
    
   // clockid_t clock_id;
    int job_number = 1;
    int first = true;
    bool exit_flag = false;
    ofstream outfile;
    string file_name = NAME + "_CNF-SAT-VC.csv";
   
    switch (OUT_TO_FILE)
    {
    case 1:
        outfile.open(file_name, ios_base::app);
        break;
    
    default:
        break;
    }

    // if (OUT_TO_FILE) 
    // { 
    //     outfile.open(file_name, ios_base::app); 
    // }

    //while (true) 
    for(;true;)
    {
        struct job* retrieved_job_ = NULL;
        string result;

        // get job from queue, use mutex for thread safety
        pthread_mutex_lock (&task_queue1_mut);
        if (!jq1.empty()) 
        {
            retrieved_job_ = jq1.front(); 
            jq1.pop_front();

            // time start
           // pthread_getcpuclockid(pthread_self(), &clock_id);
            struct timespec ts_begin;
            clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts_begin);

            // compute result
            result = "CNF-SAT-VC: " + cnf_sat_vc_solve(retrieved_job_->vertices, retrieved_job_->edges);
            
            //time end
            struct timespec ts_stop;
            clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts_stop);
            
            long double elapsed_time_us = ((long double)ts_stop.tv_sec*1000000 + (long double)ts_stop.tv_nsec/1000.0) - ((long double)ts_begin.tv_sec*1000000 + (long double)ts_begin.tv_nsec/1000.0);
            
            // output result to terminal if LOG_EN option
            switch (LOG_EN)
            {
                case 1:
                    clog << "CNF-SAT-VC,Job_num," << job_number << "," << "time_us," << elapsed_time_us << endl;
                    break;
                
                default:
                    break;
            }
            // if (LOG_EN) 
            // {
            //     clog << "CNF-SAT-VC,Job_num," << job_number << "," << "time_us," << elapsed_time_us << endl;
            // }
            
            // write to file
            switch (OUT_TO_FILE)
            {
                case 1:
                    if (first) 
                    {
                        outfile << elapsed_time_us;
                        first = false;
                    }
                    else 
                    {
                        outfile << "," << elapsed_time_us;
                    }
                    break;
                
                default:
                    break;
            }

            // if (OUT_TO_FILE) 
            // {
            //     if (first) 
            //     {
            //         outfile << elapsed_time_us;
            //         first = false;
            //     }
            //     else 
            //     {
            //         outfile << "," << elapsed_time_us;
            //     }
            // }

            //write result to result queue, use mutex for thread safety
            pthread_mutex_lock (&res_queue1_mut);
            q1.push_back(result);
            pthread_mutex_unlock (&res_queue1_mut);
            
            job_number++;
            
        }
        // exit loop when IO_handler see EOF (flag is set in int main after thread joins)
        int cond =0;
        if (*(bool*)args && jq1.empty())
        {
            cond = 1;
        }
        switch (cond)
        {
            case 1:
                exit_flag = true;
                outfile << endl;
                break;
            
            default:
                break;
        }
        // if(*(bool*)args && job_queue1.empty()) 
        // {
        //     exit_flag = true;
        //     outfile << endl;
        // }
        pthread_mutex_unlock (&task_queue1_mut);

        // cleanup memory taken by job
        delete retrieved_job_;
        if (exit_flag)
        {
            break;
        }
    }

    return NULL;
}


void* calc_aprox_vc_1(void* args) 
{
    
    int job_number = 1;
    bool exit_flag = false;
    ofstream outfile;
    string file_name = NAME + "_APPROX-VC-1.csv";
    bool first = true;

    switch (OUT_TO_FILE)
    {
        case 1:
            outfile.open(file_name, ios_base::app); 
            break;

        default:
            break;
    }
    // if (OUT_TO_FILE) 
    // { 
    //     outfile.open(file_name, ios_base::app); 
    // }
    

    //while (true) 
    for(;true;)
    {
        string result;
        struct job* retrieved_job = NULL;

        // get job from queue, use mutex for thread safety
        pthread_mutex_lock (&task_queue2_mut);
        if (!jq2.empty()) 
        {
            retrieved_job = jq2.front(); 
            jq2.pop_front();

            // time start
           // pthread_getcpuclockid(pthread_self(), &clock_id);
            struct timespec ts_start;
            clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts_start);

            // compute result
            result = "APPROX-VC-1: " + approx_vc_1(retrieved_job->vertices, retrieved_job->edges);

            //time end
            struct timespec ts_end;
            clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts_end);
            long double elapsed_time_us = ((long double)ts_end.tv_sec*1000000 + (long double)ts_end.tv_nsec/1000.0) - ((long double)ts_start.tv_sec*1000000 + (long double)ts_start.tv_nsec/1000.0);

            // output result to terminal if LOG_EN option
            switch (LOG_EN)
            {
            case 1:
                clog << "APPROX-VC-1,Job_num," << job_number << "," << "time_us," << elapsed_time_us << endl;
                break;
            
            default:
                break;
            }
            // if (LOG_EN) 
            // {
            //     clog << "APPROX-VC-1,Job_num," << job_number << "," << "time_us," << elapsed_time_us << endl;
            // }
            
            // write to file
            switch (OUT_TO_FILE)
            {
                case 1:
                    outfile << elapsed_time_us;
                    first = false;
                    break;
                
                default:
                    outfile << "," << elapsed_time_us;
                    break;
            }
            // if (OUT_TO_FILE) 
            // {
            //     if (first)
            //     {
            //         outfile << elapsed_time_us;
            //         first = false;
            //     }
            //     else 
            //     {
            //         outfile << "," << elapsed_time_us;
            //     }
            // }

            //write result to result queue, use mutex for thread safety
            pthread_mutex_lock (&res_queue2_mut);
            q2.push_back(result);
            pthread_mutex_unlock (&res_queue2_mut);  

            job_number++;
        }
        // exit loop when IO_handler see EOF (flag is set in int main after thread joins)
        int cond =0;
        if (*(bool*)args && jq2.empty())
        {
            cond = 1;
        }
        switch (cond)
        {
            case 1:
                exit_flag = true;
                outfile << endl;
                break;
            
            default:
                break;
        }
        // if(*(bool*)args && job_queue2.empty()) 
        // {
        //     exit_flag = true;
        //     outfile << endl;
        // }
        pthread_mutex_unlock (&task_queue2_mut);

        // cleanup memory taken by job
        delete retrieved_job;
        if (exit_flag) 
        {
            break;
        }

    }
    return NULL;
}


void* calc_approx_vc_2(void* args) 
{
        
    //clockid_t clock_id;
    int job_number = 1;
    bool first = true;
    bool exit_flag = false;
    ofstream outfile;
    string file_name = NAME + "_APPROX-VC-2.csv";

    switch (OUT_TO_FILE)
    {
        case 1:
            outfile.open(file_name, ios_base::app);
            break;
        
        default:
            break;
    }
    // if (OUT_TO_FILE) 
    // { 
    //     outfile.open(file_name, ios_base::app); 
    // }
    
    for(;true;)
//    while (true) 
    {
        struct job* retrieved_job = NULL;
        string result;

        // get job from queue, use mutex for thread safety
        pthread_mutex_lock (&task_queue3_mut);
        if (!jq3.empty()) 
        {
            retrieved_job = jq3.front(); 
            jq3.pop_front();

            // time start
            struct timespec ts_start;
            clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts_start);

            // compute result
            result = "APPROX-VC-2: " + approx_vc_2(retrieved_job->vertices, retrieved_job->edges);

            // time end
            struct timespec ts_end;
            clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts_end);
            long double elapsed_time_us = ((long double)ts_end.tv_sec*1000000 + (long double)ts_end.tv_nsec/1000.0) - ((long double)ts_start.tv_sec*1000000 + (long double)ts_start.tv_nsec/1000.0);
             
            // output result to terminal if LOG_EN option
            switch (LOG_EN)
            {
            case 1:
                clog << "APPROX-VC-2,Job_num," << job_number << "," << "time_us," << elapsed_time_us << endl;
                break;
            
            default:
                break;
            }
            // if (LOG_EN) 
            // {
            //     clog << "APPROX-VC-2,Job_num," << job_number << "," << "time_us," << elapsed_time_us << endl;
            // }

            // write to file if option selected
            switch (OUT_TO_FILE)
            {
                case 1:
                    /* code */
                    if (first) 
                    {
                        outfile << elapsed_time_us;
                        first = false;
                    }
                    else 
                    {
                        outfile << "," << elapsed_time_us;
                    }
                    break;
                
                default:
                    break;
            }
            // if (OUT_TO_FILE) 
            // {
            //     if (first) 
            //     {
            //         outfile << elapsed_time_us;
            //         first = false;
            //     }
            //     else 
            //     {
            //         outfile << "," << elapsed_time_us;
            //     }
            // }

            //write result to result queue, use mutex for thread safety
            pthread_mutex_lock (&res_queue3_mut);
            q3.push_back(result);
            pthread_mutex_unlock (&res_queue3_mut);
            
            job_number++;
        }
        // exit loop when IO_handler see EOF (flag is set in int main after thread joins)
        int cond  =0;
        if (*(bool*)args && jq3.empty())
        {
           cond =1 ;
        }
        switch (cond)
        {
            case 1:
                exit_flag = true;
                outfile << endl;
                break;
            
            default:
                break;
        }
        // if(*(bool*)args && job_queue3.empty()) 
        // {
        //     exit_flag = true;
        //     outfile << endl;
        // }

        pthread_mutex_unlock (&task_queue3_mut);

        // cleanup memory taken by job
        delete retrieved_job;
        if (exit_flag) 
        {
            break;
        }
    }
    
    return NULL;
}


int main(int argc, char **argv) 
{
    opterr = 0;
    string tmp;
    int ignore_sat = 0;
    int k;
    pthread_t cnf_sat_th;
    pthread_t approx_vc1_th;
    pthread_t approx_vc2_th;
    pthread_t IO_th;
    pthread_t output_th;
    bool finished_flags_indicator[3] = {false};
    
    // expected options are '-o' [out to file], '-n value' [name to prepend filename], '-i' [ignore cnfsat]
    
    for(;(k = getopt (argc, argv, "on:il?")) != -1;)
    {
        if(k == 'o')
        {
            OUT_TO_FILE = true;
        }
        else if (k == 'n')
        {
            NAME = optarg;
        }
        else if (k == 'i')
        {
            ignore_sat = 1;
            finished_flags_indicator[2] = true;
        }
        else if (k == 'l')
        {
            LOG_EN = true;
        }
        else
        {
            cerr << "Error: unknown option: " << optopt << endl;   
        }
        
    }

    pthread_create (&IO_th, NULL, &IO_func, (void *) finished_flags_indicator);
    pthread_create (&output_th, NULL, &output_func, (void *) finished_flags_indicator);
    
    switch (ignore_sat)
    {
        case 0:
            pthread_create (&cnf_sat_th, NULL, &calc_cnf_sat_vc_solve, (void *) finished_flags_indicator);
            break;
        
        default:
            break;
    }
    // if (!ignore_sat) 
    // {
    //     pthread_create (&cnf_sat_thread, NULL, &calc_cnf_sat_vc_solve, (void *) finished_flags); 
    // }
    pthread_create (&approx_vc1_th, NULL, &calc_aprox_vc_1, (void *) finished_flags_indicator);
    pthread_create (&approx_vc2_th, NULL, &calc_approx_vc_2, (void *) finished_flags_indicator);

    pthread_join (IO_th, NULL);
    finished_flags_indicator[0] = true; // flag indicates that EOF was seen at input, signal all threads to finish work and return

    // if (!ignore_sat) 
    // {
    //     pthread_join (cnf_sat_thread, NULL); 
    // }
    switch (ignore_sat)
    {
        case 0:
            pthread_join (cnf_sat_th, NULL);
            break;
        
        default:
            break;
    }
    
    pthread_join (approx_vc1_th, NULL);
    pthread_join (approx_vc2_th, NULL);
    finished_flags_indicator[1] = true; // flag indicates all jobs processed, signal output writer to finish print results and return
    pthread_join (output_th, NULL);
    return 0;
}
