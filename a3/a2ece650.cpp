
#include <iostream>
#include <sstream>
#include <vector>
#include <regex>

using namespace std;

vector<int> adj_list[] = {};

bool BFS(int v_num, int start, int end, vector<int> adj_list[],int pred[], int distance[])
{
    vector<int> queue;
 
    bool visited[v_num];
 
    for (int i = 0; i < v_num; i++) 
    {
        visited[i] = false;  // none of the vertices have been visited
        distance[i] = INT8_MIN;  // distance between all vertices is set initally to -infinity
        pred[i] = -1;   
    }
 
    visited[start] = true;  // start is the first to be visited
    distance[end] = 0;          // distance from start to start is 0
    queue.push_back(start);   //pushing starting point to back of the list (first element)
    int i = 0; //iterator for adjacency list
 
    // standard BFS algorithm
    while (!queue.empty()) 
    {
        int q = queue.front();

        i = 0;
        while(i < adj_list[q].size()) 
        {
            if (visited[adj_list[q][i]] == false) //if the vertex has not been visited
            {
                pred[adj_list[q][i]] = q;  //set the previous vertex to the one exploring
                distance[adj_list[q][i]] = distance[q] + 1; //increase the distance of the 
                visited[adj_list[q][i]] = true;  // now the vertex has been visited
                queue.push_back(adj_list[q][i]);
                if (adj_list[q][i] == end)  // stop exploring when we find end
                {
                    return true;
                }       
            }
            i++;
        }
        queue.erase(queue.begin()); // remove the first element after it is explored 
    }
    return false;
}  

vector<int> returnPath(int v_num, int start,
                           int end, vector<int> adj_list[])
{
    int pred[v_num];
    int distance[v_num];
    vector<int> path;
    int crawl = end;

    path.clear();
 
    if (BFS(v_num, start, end, adj_list, pred, distance) == false) 
    {
        cerr << "Error: Given start and end vertices are not connected."<< endl;
        return path;
    }

    path.push_back(crawl);
    while (pred[crawl] != -1) 
    {
        path.push_back(pred[crawl]);
        crawl = pred[crawl];
    }
    return path;
} 

void add_edge(int start, int end, vector<int> adj_list[]) 
{
    // add edge vertices to each others adjacency list
    adj_list[start].push_back(end);
    adj_list[end].push_back(start);
}


int main() {
    // Test code. Replaced with your code

    int v_num;  // # of vertices
    int v_start;  //start id of vertex
    int v_end;  // ending id of vertex
    char cmd;            // first command
    char prev_cmd = ' ';
    string line;
    string edge_str;
    vector<unsigned int> vertices;
    vector<int> path;
     

    while(getline(cin, line)){
        if (cin.eof()){
            break;
        }
        istringstream input(line);
        input >> cmd;
        // cout<< cmd<<endl;
        //cout<< line<< endl;
        if(cmd == 'V' || cmd == 'E')
        {
            cout << line << flush <<endl; //placed here to output the input provided by A1 (NEED TO CHECK IF IT WORKS)
        
        }
        if (cmd == 'V')
        {
            //int v_error = 0;
            input >> v_num;
            if (v_num < 0)
            {
                cerr << "Error: Invalid number of Vertices." << endl;
                //v_error = 1;  //commented since not getting used and warning in clang-tidy
            }
            else
            {
                prev_cmd = 'V';
                for (int i = 0; i < v_num; i++) 
                { 
                    adj_list[i].clear();   //clears the adj_list every time V is called.  
                }
            }
        }
        else if (cmd == 'E' and prev_cmd == 'V')
        {
            if(prev_cmd == 'E')
            {
                cerr << "Error: Repeated command!" << endl;
            }
            else
            {
                input >> edge_str;
                int v1, v2;
                int e_error = 0;

                regex f_e("-?[0-9]+"); //regex to match a # in the edge string
                regex_iterator<string::iterator> rit (edge_str.begin(), edge_str.end(),f_e); //starting iterator
                regex_iterator<string::iterator> rend; //ending iterator

                while (rit != rend) //iterate thru the string of #'s
                {
                    e_error = 0;
                    
                    try
                    {
                        v1 = stoi(rit->str());
                        rit++;
                        v2 = stoi(rit->str());
                        ++rit;
                        vertices.push_back(v1);
                        vertices.push_back(v2);
                    }
                    catch(invalid_argument)
                    {
                        cerr<<"Error: Check input vertices"<<endl;
                    }
                    if ((v1 >= v_num) || (v2 >= v_num))
                    {
                        cerr << "Error: Vertex index for an edge is higher than the number of vertices." << endl;
                        e_error = 1;
                        break;
                    }
                    else if (v1 == v2)
                    {   
                        cerr << "Error: Invalid Edge entered!" << endl;
                        e_error = 1;
                        break;
                    }
                    else if (v1 < 0 || v2 < 0)
                    {
                        cerr << "Error: Edge with a negative Vertex entered!" << endl;
                        e_error = 1;
                    }
                    else
                    {
                        add_edge(v1, v2,adj_list);
                    }
                }

                if(e_error != 1)
                {
                    prev_cmd = 'E';   
                }
            }
        }
              
        else if (cmd == 's' and prev_cmd == 'E')
        {
            input >> v_start;
           // cout << "starting vertice is : " << v_start << endl;
            input >> v_end;
            //cout << "starting vertice is : " << v_end << endl;
            int s_error = 0;           
            if ((v_start > v_num) || (v_end > v_num))
            {
                cerr << "Error: Vertex index for an edge is higher than the number of vertices." << endl;
                s_error = 1;
                //break;
            }
            else if (v_start < 0 || v_end < 0)
            {
                cerr << "Error: Shortest distance to a negative Vertex doesn't exist!" << endl;
                s_error = 1;
            }
            if((s_error != 1) && (v_start != v_end))
            {
                path = returnPath(v_num , v_start, v_end, adj_list);
                if (!path.empty())
                {
                    for (int i = path.size() - 1; i >= 0; i--)
                    {
                        cout << path[i];
                        if (i != 0)
                        {
                            cout<<"-";
                        }
                    }
                    cout<<"\n";
                }
            }
            else if(v_start == v_end)
            {
                cout << v_start << endl;
            }
        }
        // else
        // {
        //     cerr << "Error: Check input order." << endl;
        // }
    }
    return 0;
}
