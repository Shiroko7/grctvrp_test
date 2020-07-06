#include<stdio.h> 
#include <iostream>
#include <string>
#include <vector>
#include <numeric> //std::iota
#include <algorithm> 
#include <random>  
#include <cmath>
#include <limits>



using namespace std;

int int_rand(int a, int b) {
    int retorno = 0;

    if (a < b) {
        retorno = (int) ((b - a) * drand48());
        retorno = retorno + a;
    } else {
        retorno = (int) ((a - b) * drand48());
        retorno = retorno + b;
    }

    return retorno;
}

float float_rand(float a, float b) {
    float retorno = 0;

    if (a < b) {
        retorno = (float) ((b - a) * drand48());
        retorno = retorno + a;
    } else {
        retorno = (float) ((a - b) * drand48());
        retorno = retorno + b;
    }

    return retorno;
}


void print_solution(vector<int> sol)
{
    for(size_t i = 0; i < sol.size();i++)
    {
        cout << sol[i] <<" ";
    }
    cout << "\n";
}

struct coordinates
{
    float x,y;
    bool taken;
};

float distance(coordinates v, coordinates u)
{
    return sqrt(pow(v.x-u.x, 2) + pow(v.y-u.y, 2));
}

float short_FO(int i, int j, float** d_matrix, float current_risk,float R)
{
    float alpha = current_risk/R;
    return ((1-alpha)*d_matrix[i][j] + alpha*d_matrix[j][0]);
}

float single_FO(vector<int> solution, float** d_matrix, coordinates* &map)
{
    float dist = 0.0;
    if (!solution.empty())
    {
        dist = distance(map[0],map[solution[0]]);
        size_t i;
        for(i = 0; i < solution.size() - 1; i++) 
        {   
            dist += d_matrix[solution[i]][solution[i+1]];
        }
        dist += distance(map[solution.back()],map[0]);
    }
    return dist;
}

float FO(vector<vector<int>> solution, float** d_matrix, coordinates* &map)
{
    float dist = 0.0;
    for(size_t i = 0; i < solution.size(); i++)
    {
        dist += single_FO(solution[i], d_matrix, map);
    }
    return dist;
}

bool update_demand(coordinates* &map, int N)
{
    for(int i = 0; i < N; i++)
    {
        if (map[i].taken == false)
            return true;
    }
    return false;
}

float route_demand(int* demands_array, vector<int> sol, size_t n)
{
    float  r_demand = 0.0;
    size_t i;
    if (!sol.empty())
    {
        for(i = 0; i < n; i++) 
        {
            r_demand += demands_array[sol[i]];
        }
    }
    return r_demand;
}

float route_risk(int* demands_array, float** d_matrix,vector<int> sol)
{    float r_risk = 0.0;

    if (!sol.empty())
    {
        //cout << "0 - " << sol[0] << " - ";

        if (sol.size() > 1)
        {
            for(size_t j = 0; j < sol.size()-1; j++) 
            {
                //cout  << sol[j+1] << " - ";
                r_risk += d_matrix[sol[j]][sol[j+1]]*route_demand(demands_array, sol,j+1);
            }     
        }
        
        //cout << "la wea p " << sol.size()  << " sjaja ";
        r_risk += d_matrix[sol[sol.size() -1]][0]*route_demand(demands_array, sol, sol.size());
    }
    //cout << "Risk: " << r_risk <<"\n---------------- \n";
    /*
    if (sol.size() > 1)
    {
        //cout << "cuuu" << sol.size() <<" "<< i <<'\n';
        r_risk += d_matrix[0][sol.back()]*route_demand(demands_array, sol,sol.size());
        //cout << "co" <<  '\n';
    }
    else if (sol.size()==1)
    {
        r_risk += d_matrix[0][sol[0]]*demands_array[sol[0]];
    }
    */
    return r_risk;
}

bool valid_choice(vector<int> sol, bool taken, float d_dist, float min_dist, int* demands_array, float R, float** d_matrix)
{
    if (taken)
        return false;
    if (d_dist > min_dist)
        return false;
    if (route_risk(demands_array, d_matrix,sol) > R)
        return false;
    return true;
}

int choose_node(vector<int> sol, int* demands_array,float** d_matrix,coordinates* &map, int N, float R)
{
    float min_dist = numeric_limits<float>::infinity(), candidate_fo;
    int min_pos;
    bool flag = false;
    float current_risk;
    int last;
    for(int i = 0; i < N; i++)
    {
        current_risk = route_risk(demands_array,d_matrix,sol);
        if (!sol.empty())
            last = sol.back();
        else
            last = 0;
        candidate_fo = short_FO(last, i, d_matrix, current_risk, R); //single_FO(sol, d_matrix, map); //change this
        sol.push_back(i);
        if(valid_choice(sol,map[i].taken, candidate_fo, min_dist, demands_array,R,d_matrix))
        {
            min_dist = candidate_fo;
            min_pos = i;
            flag = true;
        }
        sol.pop_back();
    }


    if (flag)
    {
        return min_pos;
    }
    else
        return -1;
}

vector<vector<int>> greedy_solver(int* demands_array, coordinates* &map,float** d_matrix, int N, float R)
{
    vector<vector<int>> solution;
    bool demand = true;
    // while there's demand, choose which vehicle movement minimizes the current vehicle FO
    int n_vehicles = 0;
    while(demand)
    {
        
        vector<int> sol;
        int node_candidate = -1;
        while(true)
        {
            node_candidate = choose_node(sol,demands_array,d_matrix,map,N,R);
            if (node_candidate == -1)
                break;
            sol.push_back(node_candidate);
            map[node_candidate].taken = true;
        }
        solution.push_back(sol);
        demand = update_demand(map,N);
        n_vehicles++;
        if (n_vehicles > 300)
        {
            vector<vector<int>> solution;
            return solution;
        }
    }
    return solution;
}

vector<vector<int>> random_solver(int* demands_array, coordinates* &map,float** d_matrix, int N, float R, int seed)
{
    //random sequence of routes
    vector<int> order(N);
    iota(begin(order),end(order), 0);
    shuffle (order.begin(), order.end(), default_random_engine(seed));
    
    vector<vector<int>> solution;
    bool demand = true;
    // make the routes in order and split according to risk constraint
    int n_vehicles = 0;
    int node_candidate;
    int i = 0;
    while(demand)
    {
        vector<int> sol;
        while(i < N)
        {
            node_candidate = order[i];
            sol.push_back(node_candidate);
            if (route_risk(demands_array, d_matrix,sol) < R)
            {
                i++;
                map[node_candidate].taken = true;
            }
            else
            {
                sol.pop_back();
                break;
            }
        }
        solution.push_back(sol);
        demand = update_demand(map,N);
        n_vehicles++;
        if (n_vehicles > N)
        {
            vector<vector<int>> solution;
            return solution;
        }
    }
    return solution;
}

vector<vector<int>> intra_swap(vector<vector<int>> sol)
{
    vector<vector<int>> sol_n = sol;
    //intra route
    int route_1 = int_rand(0,sol.size()-1);

    //pick two random node
    int node_1 = int_rand(0, sol[route_1].size()-1);
    int node_2 = int_rand(0, sol[route_1].size()-1);
    int swap;
    
    //swap them
    swap = sol_n[route_1][node_1];
    sol_n[route_1][node_1] = sol_n[route_1][node_2];
    sol_n[route_1][node_2] = swap;
    return sol_n;
}

vector<vector<int>> intra_reverse(vector<vector<int>> sol)
{
    vector<vector<int>> sol_n = sol;
    //intra route
    int route_1 = int_rand(0,sol.size()-1);
    //cout <<"rfail? ";
    //reverse route
    if (sol_n[route_1].size() > 1)
        reverse(sol_n[route_1].begin(),sol_n[route_1].end());
    //cout <<"not fail"<<"\n";
    return sol_n;
}

vector<vector<int>> inter_swap(vector<vector<int>> sol)
{
    vector<vector<int>> sol_n = sol;
    //node swap
    
    //chose two random routes in solution
    int route_1 = int_rand(0,sol.size()-1);
    int route_2 = int_rand(0,sol.size()-1);

    //if they're equal chose another one
    if ( route_1 == route_2 )
    {
        if ( route_1 == (int) sol.size()-1 )
            route_2 = 0;
        else
            route_2 = route_1 + 1;
    }


    //cout << sol.size() << "\n";
    //cout << route_1 <<" " << route_2 << "what\n";
    //pick random node from each route
    int node_1 = int_rand(0, sol[route_1].size()-1);
    int node_2 = int_rand(0, sol[route_2].size()-1);
    int swap;
    
    //swap them
    //cout << "what\n";
    swap = sol_n[route_1][node_1];
    //cout << "what\n";
    sol_n[route_1][node_1] = sol_n[route_2][node_2];
    //cout << "what\n";
    sol_n[route_2][node_2] = swap;

    return sol_n;
}

vector<vector<int>> inter_realoc(vector<vector<int>> sol)
{
    vector<vector<int>> sol_n = sol;

    //chose two consecutive random routes in solution
    int route_1 = int_rand(0,sol_n.size()-1);
    int route_2 = int_rand(0,sol_n.size()-1);

    //if they're equal chose another one
    if ( route_1 == route_2 )
    {
        if ( route_1 == (int) sol_n.size()-1 )
            route_2 = 0;
        else
            route_2 = route_1 + 1;
    }

    //pick random node from one route
    int node_1 = int_rand(0, sol_n[route_1].size()-1);
    int node_2 = int_rand(0, sol_n[route_2].size()-1);


    // change it from route_1 to route_2 

    sol_n[route_2].insert(sol_n[route_2].begin() + node_2, sol_n[route_1][node_1]);
    //cout << "no fail\n";
    if ( sol_n[route_1].size() > 1 ) 
        sol_n[route_1].erase(sol_n[route_1].begin()+node_1);
    else
        sol_n.erase(sol_n.begin()+route_1);
    
    return sol_n;
}

vector<vector<int>> inter_merge(vector<vector<int>> sol)
{
    vector<vector<int>> sol_n = sol;

    //chose two consecutive random routes in solution
    int route_1 = int_rand(0,sol.size()-1);
    int route_2 = int_rand(0,sol.size()-1);

    //if they're equal chose another one
    if ( route_1 == route_2 )
    {
        if ( route_1 == (int) sol.size()-1 )
            route_2 = 0;
        else
            route_2 = route_1 + 1;
    }
 
    vector<int> route_12;
    //cout <<"fail? ";
    route_12.reserve( sol_n[route_1].size() + sol_n[route_2].size() ); // preallocate memory
    route_12.insert( route_12.end(), sol_n[route_1].begin(), sol_n[route_1].end() );
    route_12.insert( route_12.end(), sol_n[route_2].begin(), sol_n[route_2].end() );

    // delete each route
    sol_n.erase(sol_n.begin()+route_1);
    sol_n.erase(sol_n.begin()+route_2);

    sol_n.push_back(route_12);
    //cout <<"not fail"<<"\n";
    return sol_n;
}

vector<vector<int>> random_neighbour(vector<vector<int>> sol)
{
    vector<vector<int>> sol_n = sol;
    
    int choice = int_rand(0,5);
    if ( choice == 0 )
    {
        //cout << "smh1\n";
        sol_n = intra_swap(sol);
    }

    else if ( choice == 1 )
    {
        //cout << "smh2\n";
        sol_n = inter_swap(sol);
    }

    else if ( choice  == 2 )
    {
        //cout << "smh3\n";
        sol_n = inter_realoc(sol);
    }

    else if ( choice  == 3 )
    {
        //cout << "smh4\n";
        sol_n = intra_reverse(sol);
    }

    else if ( choice  == 4 )
    {
        //cout << "smh5\n";
        sol_n = inter_merge(sol);
    }

    return sol_n;
}

vector<vector<int>> simulated_annealing(float max_T, float min_T, int iterations, int max_non, float alpha, vector<vector<int>> sol_0, int* demands_array, coordinates* &map,float** d_matrix, int N, float R)
{
    int non_improves = 0;
    float current_T = max_T;
    vector<vector<int>> sol_b = sol_0;
    vector<vector<int>> sol_c = sol_0;
    vector<vector<int>> sol_n;

    float FO_b = FO(sol_b, d_matrix, map);
    float FO_c, FO_n;

    while( current_T > min_T && non_improves < max_non )
    {
        int it = 0;
        bool improve_flag = false;
        while( it < iterations )
        {//SA
            bool keep = true;
            sol_n = random_neighbour(sol_c);
            
            FO_c = FO(sol_c, d_matrix, map);
            FO_n = FO(sol_n, d_matrix, map);
            
            
            for(size_t j = 0; j < sol_n.size();j++)
            {
                if (route_risk(demands_array,d_matrix,sol_n[j]) > R)
                {
                    keep = false;
                    break;
                }
            }
            


            if (keep & (FO_n < FO_c ))
            {
                sol_c = sol_n;
                FO_c = FO_n;
            }
                
            else if (keep & (float_rand(0.0, 1.0) < exp((FO_c-FO_n)/current_T)))
            {
                sol_c = sol_n;
                FO_c = FO_n;
            }
            if (keep & (FO_c < FO_b ))
            {
                sol_b = sol_c;
                FO_b = FO_c;
                improve_flag = true;
            }
            it++;
        }
        current_T = current_T * alpha;
        if ( !improve_flag )
            non_improves++;        
    }
    
    return sol_b;
}
//int main(int argc, char** argv) {

//    Capture_Params(argc,argv);

int main(int argc, char* argv[])
{
    // seed, max_t, min_t, iterations, max_non, alpha
    // setting parameters 
    int seed = (int)atoi(argv[1]);
    float max_T = (float)atof(argv[2]);
    float min_T = (float)atof(argv[3]);
    int iterations = (int)atoi(argv[4]);
    int max_non = (int)atoi(argv[5]);
    float alpha = (float)atof(argv[6]);

    
    //cout << "Seed: " << seed << "\n";
    //cout << "max_T: " << max_T << "\n";
    //cout << "min_T: " << min_T << "\n";
    //cout << "iterations " << iterations << "\n";
    //cout << "max_non: " << max_non << "\n";
    //cout << "alpha: " << alpha << "\n";


    srand48(seed);
    
    // Numer of nodes including the depot
    int N;
    // Risk threshold
    float R;  
    // demand array
    int* demands_array;
    // nodes coordinates;
    coordinates* map;

    // read inputs
    cin >> N;
    cin >> R;

    // fill demand array
    demands_array = new int[N];
    for(int i = 0; i < N; i++)
    {
        cin >> demands_array[i];
    }

    // make distance map
    map = new coordinates[N];
    for(int i = 0; i < N; i++)
    {
        cin >> map[i].x;
        cin >> map[i].y;
    }

    map[0].taken = true;
    // distance matrix 
    float **d_matrix = new float*[N];
    for(int i = 0; i < N; ++i) {
        d_matrix[i] = new float[N];
    }
    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
            d_matrix[i][j] = distance(map[i],map[j]);
    //solution vector array
    vector<vector<int>> sol;
    // initial solution
    //sol = greedy_solver(demands_array,map,d_matrix,N,R);
    
    sol = random_solver(demands_array,map,d_matrix,N,R,seed);
    if (!sol.empty())
    {      
        sol = simulated_annealing(max_T, min_T, iterations, max_non, alpha, sol, demands_array, map,  d_matrix,  N, R);
        
        // print solution
        
        //for(size_t i = 0; i < sol.size() ; i++)
        //{
        //    cout << "vehicle " << i <<": ";
        //    for(vector<int>::iterator it = sol[i].begin(); it != sol[i].end(); ++it)
        //    {
        //        cout << *it << " ";
        //    }
        //    cout << "risk: " << route_risk(demands_array,d_matrix,sol[i]) << "\n";
        //}
        //cout << "FO: ";
        
        
        // check that all nodes were visited
        //for (int i = 0; i < N; i++)
        //    cout << map[i].taken << "\n";
        
        
        cout << FO(sol,d_matrix, map) <<"\n";
    }
    else
    {
        cout << -1 << "\n";
    }
    
    // free memory
    delete[] demands_array;
    delete[] map;

    for(int i = 0; i < N;i++) 
    {
        delete [] d_matrix[i];
    }
    delete [] d_matrix;
    
    return 0;
}