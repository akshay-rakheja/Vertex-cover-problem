#include <unistd.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <utility>

using namespace std;

vector<string> street_names;
vector<int> st_segments;
//vector<point> coors;
vector<vector<pair< int, int> > > coordinates;
int street_error = 0;
//int exit_at = 25;


unsigned int num_streets(unsigned int k) //returns random # of streets for input [2,k]
{
    //open /dev/urandom to read
    ifstream urandom("/dev/urandom");
    // check that it did not fail
    if (urandom.fail())
    {
        cerr << "Error: cannot open /dev/urandom"<< endl;
        return 1;
    }

    // read a random unsigned int
    unsigned int num = 15;
    urandom.read((char*)&num, sizeof(unsigned int));
    num = (num % (k-1)) + 2;
    urandom.close();

    return num;
}

unsigned int num_segments(unsigned int k) //return random # of line segments for a street [1,k]
{
    //open /dev/urandom to read
    ifstream urandom("/dev/urandom");
    // check that it did not fail
    if (urandom.fail())
    {
        cerr << "Error: cannot open /dev/urandom" << endl;
        return 1;
    }

    // read a random unsigned int
    unsigned int num = 10;
    urandom.read((char*)&num, sizeof(unsigned int));
    num = (num % k) + 1;
    urandom.close();

    return num;
}

unsigned int wait(unsigned int k) //returns random seconds to wait before next input between [5,k]
{
    //open /dev/urandom to read
    ifstream urandom("/dev/urandom");
    // check that it did not fail
    if (urandom.fail())
    {
        cerr << "Error: cannot open /dev/urandom\n";
        return 1;
    }

    // read a random unsigned int
    unsigned int num = 20;
    urandom.read((char*)&num, sizeof(unsigned int));
    num = (num % (k-4)) + 5;
    urandom.close();

    return num;
}



vector<string> create_street(int s)
{
    string letter = "a";

    for(int i = 0; i < s; i++)
    {
        street_names.push_back(letter);
        letter = letter + "b";
    }
    return street_names;
}

vector<int> st_segment_length (int s, int n_val)
{
    for (int i = 0; i < s; i++)
    {   
        int n = num_segments(n_val);
        st_segments.push_back(n);
    }
    return st_segments;
}

int create_coor(unsigned int k) //returns random coordinate between [-k,k]
{
    //open /dev/urandom to read
    ifstream urandom("/dev/urandom");
    // check that it did not fail
    if (urandom.fail())
    {
        cerr << "Error: cannot open /dev/urandom\n";
        return 1;
    }

    // read a random unsigned int
    int num = 42;
    //urandom.read((char *)&num, sizeof(int));
    urandom.read((char*)&num, sizeof(int));

    //cout<<"k ="<<k<<endl;
    num = (num % ((2*k)+1));
    num = num - k;
    urandom.close();
    //cout<< num<<endl;

    return num;
}

int min(int x, int y)
{
    if (x < y)
    {
        return x;
    }
    
    return y;

}

int max(int x, int y)
{
    if (x > y)
    {
        return x;
    }
    
    return y;

}


bool intersect(pair<int,int> p1, pair<int,int> p2, pair<int,int> p3, pair<int,int> p4)
{   
    int x1 = p1.first;
    int x2 = p2.first;
    int x3 = p3.first;
    int x4 = p4.first;
    int y1 = p1.second;
    int y2 = p2.second;
    int y3 = p3.second;
    int y4 = p4.second;

    // min and max of the 2 segment's x and y coordinates
    int xmin_seg1 = min(x1, x2);
    int xmax_seg1 = max(x1, x2);
    int xmin_seg2 = min(x3, x4);
    int xmax_seg2 = max(x3, x4);
    int ymin_seg1 = min(y1, y2);
    int ymax_seg1 = max(y1, y2);
    int ymax_seg2 = max(y3, y4);
    int ymin_seg2 = min(y3, y4);
    
    vector<int> interval_x, interval_y;

    interval_x.push_back(max(xmin_seg1, xmin_seg2));
    interval_x.push_back(min(xmax_seg1, xmax_seg2));

    interval_y.push_back(max(ymin_seg1, ymin_seg2));
    interval_y.push_back(min(ymax_seg1, ymax_seg2));

    if (x1 == x2 == x3 == x4) //check for overlapping lines
    { 
        vector<pair<int,int> > points;
        //points = [(x1, y1), (x2, y2), (x3, y3), (x4, y4)]
        points.push_back(make_pair(x1,y1));
        points.push_back(make_pair(x2,y2));
        points.push_back(make_pair(x3,y3));
        points.push_back(make_pair(x4,y4));

        vector<pair<int,int> > temp;
        //temp = []
        for(int i = 0; i < points.size();i++)
        {
            if(interval_y[0] <= points[i].second <= interval_y[1])
            {
                temp.push_back(points[i]);
            }
        }
        if(!temp.empty())
        {
            return true;    
        }
        else
        {
            return false;
        }
        
    }
    else if ((x1 != x2) && (x3 != x4))
    {
        float m1 = (float)(y2 - y1) / (float)(x2 - x1);
        float m2 = (float)(y4 - y3) / (float) (x4 - x3);
        
        float b1 = y1 - m1 * x1;
        float b2 = y3 - m2 * x3;

        if ((m1 == m2) && (b1 == b2))
        {   
            vector<pair<int, int> > points;
            points.push_back(make_pair(x1,y1));
            points.push_back(make_pair(x2,y2));
            points.push_back(make_pair(x3,y3));
            points.push_back(make_pair(x4,y4));    

            vector<pair<int,int> > temp;
            
            for(int i = 0; i < points.size();i++)
            {
                if((interval_x[0] <= points[i].first <= interval_x[1]) && (interval_y[0] <= points[i].second <= interval_y[1]))
                {
                    temp.push_back(points[i]);
                }
            }
            if(!temp.empty())
            {
                return true;    
            }
            else
            {
                return false;
            }
        
        }
    }

    float x_num = (float)((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4));
    float y_num = (float)(x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4);
    float x_den = (float)((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));
    float y_den = (float)((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));

    //cout<<"x_den is "<<x_den<<endl;
    try
    {
        float x_coor = (float)(x_num / x_den);
        float y_coor = (float)(y_num / y_den);
        //cout<<"reached here-> safe till here"<<endl;
        
        if (x_coor < interval_x[0] || x_coor > interval_x[1] || y_coor < interval_y[0] || y_coor > interval_y[1])
        {
            return false;
        }

    }
    catch (runtime_error& e) 
    {  
        return false;
    }
    return true;
}

bool overlap(pair<int,int> p1, pair<int,int> p2, pair<int,int> p3, pair<int,int> p4)
{
    int x1 = p1.first;
    int x2 = p2.first;
    int x3 = p3.first;
    int x4 = p4.first;
    int y1 = p1.second;
    int y2 = p2.second;
    int y3 = p3.second;
    int y4 = p4.second;
    
    float slope1,slope2,b1,b2;
    if((x2!=x1) && (x3!=x4))
    {
        slope1 = float (y2 - y1) / float (x2 - x1);
    	slope2 = float (y4 - y3) / float (x4 - x3);
    	b1 = float (y1) - float (slope1 * x1);
    	b2 = float (y3) - float (slope2 * x3);
    	//cout<<slope1<<endl;
    	if ((slope1 == slope2) && (b1 == b2))
        {
	        if(min(x3,x4) >= 0)
	        {
    			if(max(x1,x2) >= min(x3,x4))
	    		{
		    	    //cout<<"here 1"<<endl;
			        return true;
			    }
			}
			else
			{
    			if(max(x3,x4) >= min(x1,x2))
    			{
    			    //cout<<"or here 1"<<endl;		    
    			    return true;
    			}
			}
        }
    }
    else
    {
        if(x2==x3)
        {
            if(min(y3,y4) >= 0)
    	   {
    			if(max(y1,y2) >= min(y3,y4))
        		{
    	    	   // cout<<"here 2"<<endl;
    		        return true;
    		    }
    		}
    		else
    		{
    			if(max(y3,y4) >= min(y1,y2))
    			{
    			    //cout<<"or here 2"<<endl;		    
    			    return true;
    			}
    		}
        }
    }    

	return false;
}

bool onSegment(pair<int,int> p, pair<int,int>  q, pair<int,int> r) 
{ 
    if (q.first != p.first) //for non vertical lines with slop != inf
    {
        double m = ((double)(q.second-p.second))/((float)(q.first-p.first));
        double c = (double)(q.second - (m * q.first));
        
        if (r.second == (m * r.first + c))
        {
            if(max(p.first,q.first) >=r.first && min(p.first,q.first) <= r.first)
            {
                    return true;
            }
            else
            {
                return false;
            }
            
        }
        else
        {
            return false;
        }
    }
    else //for vertical lines with slop = inf
    {
        //cout<<"entered here"<<endl;
        if(r.second <= max(q.second,p.second) && r.second >= min(q.second,p.second) && r.first == p.first)
        {
            return true;
        }    
        else
        {
            return false;
        }
    }
}

bool check_onSegment(int s, int c_val) //checking if the newly created point shouldnt lie on the segments created by the previous 2 points
{

    for (int i  = 0; i < coordinates.size(); i++) 
    {
        for (int k = 0; k < (coordinates[i].size()-2); k++)
        {   
            if(onSegment(coordinates[i][k],coordinates[i][k+1],coordinates[i][k+2]))
            {
                return true;
            } 
            
        }

    }
    return false;

}

bool check_selfintersection(int s, int c_val) //checking if the street doesn't intersect itself
{
    for(int i = 0; i < coordinates.size(); i++)
    {
        for (int k = 0; k < (coordinates[i].size()-3); k++)
        { 
           for(int j =0; (j < (coordinates[i].size()-3)); j++) //check again
            {
                if((coordinates[i][k+1] != coordinates[i][j+2]) && (coordinates[i][k] != coordinates[i][j+2]))
                {
                    if(intersect(coordinates[i][k],coordinates[i][k+1],coordinates[i][j+2],coordinates[i][j+3]))
                    {
                        // cout<<"intersection between ("<<coordinates[i][k].first<<","<<coordinates[i][k].second<<"),("<<coordinates[i][k+1].first<<","<<coordinates[i][k+1].second<<") and ("<<coordinates[i][j+2].first<<","<<coordinates[i][j+2].second<<"), ("<<coordinates[i][j+3].first<<","<<coordinates[i][j+3].second<<")"<<endl;
                        return true;
                    }

                }
                    
           }
        }   
    }
   
    return false;
}

bool check_overlap(int s, int c_val) //checking if any line segment overlaps with line segments from other streets
{

    for(int i = 0; i< coordinates.size()-1; i++)
    {
        for(int k = 0 ; k < coordinates[i].size()-1; k++)
        {
            for(int j = 0; j <coordinates[i+1].size()-1; j++ )
            {
                if(overlap(coordinates[i][k],coordinates[i][k+1],coordinates[i+1][j],coordinates[i+1][j+1]))
                {
                    return true;
                }
            }
        
        }
    }
    return false;
}

void vector_of_streets(int s, int c_val)
{
    coordinates.clear();
    vector <pair<int,int> >  temp; //temp vector to add coordinates for each street which is to be added to vector of streets 
    
    for(int i = 0; i < s; i++)  //for each street
    {   
        temp.clear(); //clearing temp vector for each new street
        for(int k = 0; k <= st_segments[i]; k++)  //for line segment in a street
        {
           // temp.clear(); //clearing temp vector for each new street
            temp.push_back(make_pair(create_coor(c_val),create_coor(c_val))); //add coordinates of a street to a temp vector
           // cout<<"pushing "<< temp.first<<" and "<<temp.second<<endl;
            
            //cout<<"coordinates are " << coordinates[i][k].first <<" and "<<coordinates[i][k].second << endl;
        
        }
        coordinates.push_back(temp);  //adding temp vector to vector of vectors
    }

    if((check_overlap(s,c_val) == true || check_onSegment(s,c_val) == true || check_selfintersection(s,c_val) == true ) && (street_error<=500))
    {
        street_error += 1;
       // cout<<"st error is "<<street_error<<endl;
        vector_of_streets(s,c_val);
    }
    
}


int main(int argc, char **argv)
{
    int s_val = 10; //default value of # of streets in between [2,k], k = 10 here and k >= 2
    int n_val = 5; //default value for # of line segments in each street in between [1,k], k = 5 here and k >= 1
    int l_val = 5; //default value of # of seconds to wait before next random input, w seconds, where w is in [5, k] here k = 5 and k >= 5.
    int c_val = 20 ; //process should generate (x, y) coordinates such that every x and y value is in the range [-k, k]
    
    string svalue, nvalue, lvalue, cvalue;

    
    int c; // for arguments

    while ((c = getopt(argc, argv, "s:n:l:c:")) != -1)
    {

        switch (c) 
        {
            case 's':

                svalue = optarg;
                s_val = atoi(svalue.c_str());
                if (s_val < 2)
                {
                    cerr << "Error: Number of streets (argument after s) is less than 2" << endl;
                    exit(1);
                }
                break;
            
            case 'n':
                
                nvalue = optarg;
                n_val = atoi(nvalue.c_str());
                if (n_val < 1)
                {
                    cerr << "Error: Number of street segments (argument after n) is less than 1" << endl;
                    exit(1);
                }
                break;
            
            case 'l':
                
                lvalue = optarg;
                l_val = atoi(lvalue.c_str());
                if (l_val < 5)
                {
                    cerr << "Error: Wait time for next input (argument after l) is less than 5" << endl;
                    exit(1);
                }
                break;

            case 'c':
                
                cvalue = optarg;
                c_val = atoi(cvalue.c_str());
                if (c_val < 1)
                {
                    cerr << "Error: Input k for coordinates (x,y) in range [-k,k] (argument after c) is less than 1" << endl;
                    exit(1);
                }
                break;

            case '?':
                if (optopt== 's') 
                {
                    cerr << "Error: option 's' requires an argument." << endl;
                    exit(1);
                    break;
                }
                else if (optopt == 'n') 
                {
                    cerr << "Error: option 'n' requires an argument." << endl;
                    exit(1);
                    break;
                }
                else if (optopt == 'l') 
                {
                    cerr << "Error: option 'l' requires an argument." << endl;
                    exit(1);
                    break;
                }
                else if (optopt == 'c') 
                {
                    cerr << "Error: option 'c' requires an argument." << endl;
                    exit(1);
                    break;
                }
                else
                    cerr << "Error: unknown option: " << optopt << endl;
                    exit(1);
                    break;  
                
        }

    }

    while(true)
    {
        //remove existing streets if at all using "r" command passed on to a1
        if(street_names.size() != 0)
        {
            for( int i = 0; i< street_names.size(); i++)
            {   
                cout<<"r "<<'"'<< street_names[i]<<'"'<<flush<< endl; 
            }
    
        }
        //removing streets and the number of its segments from the vector 
        street_names.clear();
        st_segments.clear();
        coordinates.clear();

        // # of streets to generate with valid argument k>=2, [2,k]
        int s = num_streets(s_val);

        // # of seconds to wait before new input to be used after "g"
        int w = wait(l_val);
        
        //creating streets for value in between [2,s]-> a,ab,abb,abbb,etc.
        create_street(s);

        // # of linesegments in each street with valid argument  k >= 1, [1,n] here
        st_segment_length(s, n_val);

        ///creates thes 1ST vector of vector of streets with coordinate pairs; passing # of streets and point boundary
        vector_of_streets(s, c_val); //also checks for overlapping, onsegment and intersection conditions
        
        if(street_error < 500)
        {
            //cout<<"st error before printing is "<<street_error<<endl;
            for (int i = 0; i<s;i++)
            {

                cout<<"a "<<'"'<<street_names[i]<<'"';
                for(int k =0; k<= st_segments[i];k++)
                {
                    cout<<" ("<<coordinates[i][k].first<<","<<coordinates[i][k].second<<")";
                }
                cout<<"\n";
                cout<< flush;


            }
             cout<<"g"<<endl;
             cout<<flush;
             street_error =0;
        }
        else
        {
            street_names.clear();
            cerr<<"Error: failed to generate valid input for 35 simultaneous attempts!"<<endl;
            exit(1);

        }
       
        street_error = 0;
        //wait after the above valid input is printed
        sleep(w);

    }

}
