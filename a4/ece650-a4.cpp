//#include <bits/stdc++.h>
#include <sstream>
#include <fstream>
#include <string>
#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"
#include <vector>
#include <iostream>
#include <regex>

using namespace std;
using namespace Minisat;


vector<int> cover;


void print_output(vector<int> cover)
{
    for (unsigned int i = 0; i<cover.size(); i++)
    {
        cout<<cover[i]<<" ";
    }
    cout<<endl; 
    cover.clear();
}

vector<vector<Lit> > create_literals(int v_num, int k, Minisat::Solver& solver)
{
    vector<vector<Lit> > x(v_num);
    int i =0;
    while(i<v_num)
    {
        int j =0;
        while(j<k)
        {
            Lit literal = mkLit(solver.newVar());
            x[i].push_back(literal);   
            j++;
        }
        i++;
    }

    return x;

}

void clause_atleast_one_vertex(int k, int v_num, Minisat::Solver& solver,vector<vector<Lit> > x)
{
    int i =0;
    while(i<k)
    {
        vec<Lit> temp;
        int j =0;
        while(j < v_num)
        {
            temp.push(x[j][i]);
            j++;
        }
        solver.addClause(temp);
        temp.clear();
        i++;

    }

}

void clause_no_vertex_twice(int k, int v_num, Minisat::Solver& solver,vector<vector<Lit> > x)
{
    int i =0;
    while(i<v_num)
    {
        int j =0;
        while(j<k-1)
        {
            int l =j+1;
            while(l<k)
            {
                solver.addClause(~x[i][j],~x[i][l]);
                l++;
            }
            j++;
        }
        i++;
    }
}

void clause_no_more_than_one(int k, int v_num, Minisat::Solver& solver,vector<vector<Lit> > x)
{
    int i =0;
    while(i<k)
    {
        int j =0;
        while(j < v_num-1)
        {
            int l = j+1;
            while(l<v_num)
            {
                solver.addClause(~x[j][i], ~x[l][i]);
                l++;
            }
            j++;
        }
        i++;
    }
}

void clause_edge_is_incident(int k, int v_num, Minisat::Solver& solver,vector<vector<Lit> > x, vector<int> vertices)
{
    unsigned int j = 0;
    while(j< vertices.size())
    {
        vec<Lit> temp1;    
        int m = vertices[j+1];  
        int l = vertices[j];
         
        int i =0;
        while(i<k)
        {
            temp1.push(x[l][i]);
            temp1.push(x[m][i]);
            i++;
        }
        solver.addClause(temp1);
        temp1.clear();
        j=j+2;

    }
}


void vertex_coversat(int v_num,vector<int> vertices)
{

    vector<int> cover;

    int k = 0;
    int low = 0;
    int high = v_num;

    
    while(low <= high)
    {
        Solver solver;
        vector<vector<Lit> > x(v_num);
        k = int(low + high)/ 2;
        

        x = create_literals(v_num, k, solver);
        //cout<< x.size()<<endl;

        clause_atleast_one_vertex(k,v_num,solver,x);

        clause_no_vertex_twice(k, v_num, solver, x);
        
        clause_edge_is_incident(k,v_num,solver,x,vertices);

        clause_no_more_than_one(k,v_num,solver,x);

    
        bool sat = solver.solve(); //check for sat or unsat
        if(sat) 
        {
            cover.clear();
            high = k - 1; 
            int i =0;
            while(i<v_num)
            {
                int j = 0;
                while(j< k)
                {
                    lbool tr_val = solver.modelValue(x[i][j]);
                    if(tr_val == l_True)
                    {   
                        cover.push_back(i);
                    }
                    j++;
                }
                i++;
            }
        }
        else
        {
            low = k+1;
        }

    }

    print_output(cover);

}

//Main Program
int main(int argc, char** argv)
{

    int v_num;  // # of vertices
    char cmd;            // first command
    string line;
    string edge_str;
    vector<int> vertices; 

    while(getline(cin, line)){
        if (cin.eof()){
            break;
        }
        istringstream input(line);
        input >> cmd;

        if (cmd == 'V')
        {
            //int v_error = 0;
            input >> v_num;
            if (v_num < 0)
            {
                cerr << "Error: Invalid number of Vertices." << endl;
            }
        }
        else if (cmd == 'E')
        {

            input >> edge_str;
            int v1, v2;
            //int e_error = 0;

            regex f_e("-?[0-9]+"); //regex to match a # in the edge string
            regex_iterator<string::iterator> rit (edge_str.begin(), edge_str.end(),f_e); //starting iterator
            regex_iterator<string::iterator> rend; //ending iterator

            while (rit != rend) //iterate thru the string of #'s
            {
                v1 = stoi(rit->str());
                rit++;
                v2 = stoi(rit->str());
                ++rit;
                vertices.push_back(v1);
                vertices.push_back(v2);
                
            }
            vertex_coversat(v_num,vertices);
            vertices.clear();    
        }
    }
    return 0;

}
