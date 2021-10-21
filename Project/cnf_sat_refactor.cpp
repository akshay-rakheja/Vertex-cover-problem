// git clone https://github.com/agurfinkel/minisat.git
// clang-tidy ece650-a4.cpp -p=./build/compile_commands.json
// clang-tidy -p ./build clang_chk.cpp -checks=*

#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <chrono>
#include <regex>
#include <list>
#include <limits.h>
#include <set>

#include <minisat/core/SolverTypes.h>
#include <minisat/core/Solver.h>

class Graph // reference: https://www.geeksforgeeks.org/check-given-graph-tree/
{
public:
    int V;    // No. of vertices
    std::list<int> *adj; // adjacency lists

    Graph(int V)
    {
        this->V = V;
        adj = new std::list<int>[V];
    }
    
    void addEdge(int v, int w)
    {
        adj[v].push_back(w); 
        adj[w].push_back(v); 
    }
    
    // A recursive function that uses visited[] and parent to
    // detect cycle in subgraph reachable from vertex v.
    bool isCyclicUtil(int v, bool visited[], int parent)
    {
        // Mark the current node as visited
        visited[v] = true;
    
        // Recur for all the vertices adjacent to this vertex
        std::list<int>::iterator i;
        for (i = adj[v].begin(); i != adj[v].end(); ++i)
        {
            // If an adjacent is not visited, then recur for 
            // that adjacent
            if (!visited[*i])
            {
            if (isCyclicUtil(*i, visited, v))
                return true;
            }
    
            // If an adjacent is visited and not parent of current
            // vertex, then there is a cycle.
            else if (*i != parent)
            return true;
        }
        return false;
    }
    
    // Returns true if the graph is a tree, else false.
    bool isTree()
    {
        // Mark all the vertices as not visited and not part of 
        // recursion stack
        bool *visited = new bool[V];
        for (int i = 0; i < V; i++)
            visited[i] = false;
    
        // The call to isCyclicUtil serves multiple purposes.
        // It returns true if graph reachable from vertex 0 
        // is cyclcic. It also marks all vertices reachable 
        // from 0.
        if (isCyclicUtil(0, visited, -1))
                return false;
    
        // If we find a vertex which is not reachable from 0 
        // (not marked by isCyclicUtil(), then we return false
        for (int u = 0; u < V; u++)
            if (!visited[u])
            return false;
    
        return true;
    }
};

class VertexCover {

private:
    int vertices;
    std::vector< std::pair<int,int> > edges;

    Minisat::Var toVar(int row, int column, int k) 
    {
        int columns = k;
        return row * columns + column;
    }

    void C_at_least_one_in_cover(Minisat::Solver& solver, int k) 
    {
        for (int i = 0; i < k; i++) 
        {
            Minisat::vec<Minisat::Lit> clause;
            for (int n = 0; n < vertices; n++) 
            {
                clause.push(Minisat::mkLit(toVar(n,i,k)));
            }
            solver.addClause(clause);
        }
    }

    void C_vertex_only_once(Minisat::Solver& solver, int k) 
    {
        for (int i = 0; i < vertices; i++) 
        {   
            for (int q = 0; q < k; q++) 
            {
                for (int p = 0; p < q; p++) 
                {
                    solver.addClause( ~Minisat::mkLit(toVar(i,p,k)), ~Minisat::mkLit(toVar(i,q,k)));  
                }   
            }        
        }
    }

    void C_one_per_cover_pos(Minisat::Solver& solver, int k) 
    {
        for (int i = 0; i < k; i++) 
        {   
            for (int q = 0; q < vertices; q++) 
            {
                for (int p = 0; p < q; p++) 
                {
                    solver.addClause( ~Minisat::mkLit(toVar(p,i,k)), ~Minisat::mkLit(toVar(q,i,k)));  
                }   
            }        
        }
    }

    void C_every_edge_covered(Minisat::Solver& solver, int k) 
    {
        for ( auto& e : edges) 
        {
            Minisat::vec<Minisat::Lit> literals;
            for (int i = 0; i < k; i++) 
            {
                literals.push(Minisat::mkLit(toVar(e.first, i, k)));
                literals.push(Minisat::mkLit(toVar(e.second, i, k)));
            }
            solver.addClause(literals);
        }
    }

    bool solve(Minisat::Solver& solver, int k) 
    {
        for (int r = 0; r < vertices; r++) 
        {
            for (int c = 0; c < k; c++) 
            {
                solver.newVar();
            }
        }

        C_at_least_one_in_cover(solver, k);
        C_vertex_only_once(solver, k);
        C_one_per_cover_pos(solver, k);
        C_every_edge_covered(solver, k);

        auto sat = solver.solve();
        
        return sat;
    }

    std::vector<int> path_output(Minisat::Solver& solver, int k) 
    {
        std::vector<int> path;
        for (int r = 0; r < vertices; r++) 
        {
            for (int c = 0; c < k; c++) 
            {
                if (solver.modelValue(toVar(r,c,k)) == Minisat::l_True) 
                {
                    path.push_back(r);
                }
            }      
        }
        std::sort(path.begin(), path.end());
        return path;
    }

public:
    // VertexCover( int v = 0): vertices(v) {}
    VertexCover( int v, std::vector< std::pair<int,int> > e): vertices(v), edges(e) {}

    int get_vertices() const 
    {
        return vertices;
    }

    std::string print_vector(std::vector<int> result_paths) 
    {
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

    std::set<int> s1;

    int min(int x, int y) { return (x < y) ? x : y; }

    struct node
    {
        int data;
        int vc;
        std::vector <int> vc_vector;
        struct node *left, *right;
        node(int data) : data(data),vc(-1),left(NULL),right(NULL){}
    };

    int vertexCover(struct node *root)
    {

        if (root == NULL)
            return 0;
        if (root->left == NULL && root->right == NULL)
            return 0;

        if (root->vc != -1) //dp
            return root->vc;

        int size_root_inc = 1 + vertexCover(root->left) + vertexCover(root->right);

        int size_root_exc = 0;
        if (root->left)
            size_root_exc += 1 + vertexCover(root->left->left) + vertexCover(root->left->right);
        if (root->right)
            size_root_exc += 1 + vertexCover(root->right->left) + vertexCover(root->right->right);

        root->vc = min(size_root_inc, size_root_exc);
        
        return root->vc;
    }

    struct node *newNode(int data)
    {
        struct node *temp = new node(data);
        return temp;
    };

    void printInorder(struct node* node)
    {
        if (node == NULL)
            return;
    
        printInorder(node->left);
            
        int right_vc, left_vc;

        if (node->right != NULL && node->right->vc != -1)
            right_vc = node->right->vc;
        else
            right_vc = 0;
        if (node->left != NULL && node->left->vc != -1)
            left_vc = node->left->vc;
        else 
            left_vc = 0;

        // std::cout << node->data << " " <<  node->vc << " " << left_vc << " " << right_vc << " " << std::endl;
        if (node->vc > left_vc+right_vc)
        {
            // std::cout << "Part of VC: " << node->data << std::endl;
            s1.insert(node->data);
        }
        
        printInorder(node->right);
    }

    bool binary_tree = false;
    bool found = false;

    void printInorder_for_searching(struct node* node, int search, int add)
    {
        if (node == NULL)
            return;
    
        /* first recur on left child */
        printInorder_for_searching(node->left, search, add);
    
        /* then print the data of node */
        if (node->data == search)
        {
            found = true;
            // std::cout << node->data << std::endl;
            if (node->left == NULL)
                node->left = newNode(add);
            else if (node->right == NULL && node->left->data!=add) // to avoid adding twice
                node->right = newNode(add);
            else
                binary_tree = false;
        }
        else if (node->data == add)
        {
            found = true;
            // std::cout << node->data << std::endl;
            if (node->left == NULL)
                node->left = newNode(search);
            else if (node->right == NULL && node->left->data!=search) // to avoid adding twice
                node->right = newNode(search);
            else
                binary_tree = false;
        }

        /* now recur on right child */
        printInorder_for_searching(node->right, search, add);
    }

    std::string find_minimum() 
    {
        if (edges.empty()) 
        {
            std::string result;
            result = "";
            return result;
        }
        Graph *g = new Graph(vertices);
        
        for (auto &e: edges)
        {
            // std::cout << e.first << " " << e.second << std::endl;
            g->addEdge(e.first, e.second);
        }
        // for (int i=0; i < g->V; i++)
        // {
        //     std::list <int> :: iterator it;
        //     std::cout << i << ":\t";
        //     for(it = g->adj[i].begin(); it != g->adj[i].end(); ++it)
        //         std::cout << '\t' << *it;
        //     std::cout << '\n';
        // }
        if (g->isTree())
        {
            // std::cout << "Graph is Tree\n";
            binary_tree = true;
            struct node *root = newNode(edges[0].first);
            root->left = newNode(edges[0].second);
            for (auto i=1; i < edges.size(); i++)
            {
                auto e = edges[i];
                // std::cout << e.first << " " << e.second << std::endl;
                printInorder_for_searching(root, e.first, e.second);
                if (found)
                {
                    found = false;
                }
                else
                {
                    binary_tree=false;
                }
            }

            if (binary_tree)
            {
                int vc_dp = vertexCover(root);
                printInorder(root);
                std::set<int>::iterator itr;
                std::vector<int> result_dp;
                // std::cout << "\nThe set s1 is : \n";
                for (itr = s1.begin(); itr != s1.end(); itr++)
                {
                    result_dp.push_back(*itr);
                }
                print_vector(result_dp);// << std::endl;
            }
            // else std::cout << "Not a binary tree" << std::endl;

            //E {<0,1>,<0,2>,<1,3>,<3,4>,<1,5>,<2,6>,<6,7>,<6,8>,<2,9>}
            //E {<0,1>,<2,1>,<2,3>,<4,3>,<4,5>,<5,6>,<7,6>,<7,8>,<8,9>,<9,10>,<10,11>}
            //E {<0,1>,<0,2>,<0,3>} V4
            //E {<0,1>,<0,2>,<1,3>} V4
            //E {<0,1>,<1,2>,<2,3>} V4
            //E {<0,1>,<1,2>,<1,3>,<1,4>,<0,5>} V6

            // struct node *root = newNode(20);
            // root->left = newNode(18);
            // root->left->left = newNode(40);
            // root->left->right = newNode(12);
            // root->left->right->left = newNode(10);
            // root->left->right->right = newNode(14);
            // root->right = newNode(24);
            // root->right->right = newNode(25);
        }
        // else std::cout << "Graph is not Tree\n";
        delete g;

        int estimate_max_vc = (int)approx_vc_1();
        int low = 0;
        int high = estimate_max_vc;
        int mid;
            
        int results[vertices];  
        std::vector<int> result_paths[vertices];
        std::fill_n(results, vertices, -1);

        while (low <= high) 
        {
            mid = (high+low)/2;
            Minisat::Solver solver;

            results[mid] = solve(solver, mid);

            if (results[mid]) {
                result_paths[mid] = path_output(solver, mid);
            }
            
            // If SAT and result[k-1] are UNSAT, the minimum is found
            if (results[mid] == 1 && results[mid-1] == 0 && mid != 0) {
                return print_vector(result_paths[mid]);
            }

            // If UNSAT and result[k+1] are SAT, the minimum is found
            if (results[mid] == 0 && results[mid+1] == 1 && mid != vertices) {
                return print_vector(result_paths[mid+1]);
            }
            
            if (results[mid]) {
                high = mid - 1;
            } 
            else {
                low = mid + 1;
            }
        }

        std::cerr << "Error: UNSAT" << std::endl;
    }

    void find_minimum_old() 
    {
        if (edges.empty()) 
        {
            std::cout << std::endl;
            return;
        }
            
        int results[vertices];  
        std::vector<int> result_paths[vertices];
        std::fill_n(results, vertices, -1);

        for (int i=0; i <=vertices; i++) 
        {
            Minisat::Solver solver;
            results[i] = solve(solver, i);
            if (results[i]) {
                result_paths[i] = path_output(solver, i);
                for (auto v : result_paths[i]) 
                {
                    std::cout << v << " ";
                } 
                std::cout << std::endl;
                return;
            } 
        }

        std::cerr << "Error: UNSAT" << std::endl;
    }

    std::vector< std::vector<int> > create_graph_adj_list(int v, std::vector< std::pair<int,int> > edges) 
    {
        std::vector< std::vector<int> > graph(v);
        for ( auto& e : edges) {
                graph[e.first].push_back(e.second);
                graph[e.second].push_back(e.first);
            }
        return graph;
    }


    std::vector< std::vector<int> > create_graph_adj_matrix(int v, std::vector< std::pair<int,int> > edges) 
    {
        
        // initialize v x v matrix
        std::vector< std::vector<int> > graph(v);
        for ( int i = 0 ; i < v ; i++ )
            graph[i].resize(v);

        // add edges between vertices
        for ( auto& e : edges) {
                graph[e.first][e.second] = 1;
                graph[e.second][e.first] = 1;
            }
        return graph;
    }


    std::size_t approx_vc_1() 
    {
        std::vector<int> v_cover;
        std::vector< std::vector<int> > graph = create_graph_adj_matrix(vertices, edges);

        while(true) {
            // pick highest degree vertex
            int vertex = -1;
            int max_degree = 0;
            for (unsigned int r=0; r < graph.size(); r++) {
                int v_deg = 0;
                for (auto& n : graph[r])
                    v_deg += n;
                
                if (v_deg > max_degree) {
                    vertex = r;
                    max_degree = v_deg;
                }
            }
            
            // break if no edges remain (ever vertex has degree of 0)
            if (vertex < 0) {
                break;
            }

            // remove edges incident to vertex
            for (unsigned int i=0; i < graph.size(); i++) {
                graph[i][vertex] = 0; // set column to 0
                graph[vertex][i] = 0; // set row to 0
            }

            // add vertex to cover
            v_cover.push_back(vertex);
            
            // debug messages
            // std::cout << "Adding vertex: " << vertex << std::endl;
            // std::cout << "Current Graph:" << std::endl;
            // for (auto& r : graph) {
            //     for (auto& c : r) {
            //         std::cout << c << " ";
            //     }
            //     std::cout << std::endl;
            // }
        }
        // std::cout << "Size from approx VC: " << v_cover.size();
        return v_cover.size();
    }

};


// E {<6,2>,<6,5>,<6,3>,<6,4>,<2,0>,<0,1>}

// E {<6,2>,<6,3>,<2,0>,<0,1>,<2,7>}