#include<stdio.h> 
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
using namespace std;



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
    return ((1-alpha)*d_matrix[i][j] + alpha*d_matrix[0][j]);
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
        for(i = 1; i < n; i++) 
        {
            r_demand += demands_array[sol[i]];
        }
    }
    return r_demand;
}

float route_risk(int* demands_array, float** d_matrix,vector<int> sol)
{
    float r_risk = 0.0;
    size_t i;
    if (!sol.empty())
    {
        r_risk +=  demands_array[sol[0]]*d_matrix[0][sol[0]];
        for(i = 1; i < sol.size(); ++i) 
        {
            r_risk += d_matrix[sol[i-1]][sol[i]]*route_demand(demands_array, sol,i);
        }
        
    }
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
        candidate_fo = short_FO(last, i, d_matrix, current_risk,R); //single_FO(sol, d_matrix, map); //change this
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

void print_solution(vector<int> sol)
{
    for(size_t i = 0; i < sol.size();i++)
    {
        cout << sol[i] <<" ";
    }
    cout << "\n";
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

int main()
{
    // Numer of nodes including the depot
    int N;
    // Risk threshold
    float R;  
    // demand array
    int* demands_array;
    // nodes coordinates;
    coordinates* map;

    //read inputs
    cin >> N;
    cin >> R;

    demands_array = new int[N];
    for(int i = 0; i < N; i++)
    {
        cin >> demands_array[i];
    }

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
    
    sol = greedy_solver(demands_array,map,d_matrix,N,R);
    
    /*
    for(size_t i = 0; i < sol.size() ; i++)
    {
        cout << "vehicle " << i <<": ";
        for(vector<int>::iterator it = sol[i].begin(); it != sol[i].end(); ++it)
        {
            cout << *it << " ";
        }
        cout << "risk: " << route_risk(demands_array,d_matrix,sol[i]) << "\n";
    }
    cout << "FO: ";
    */
    cout << FO(sol,d_matrix, map) <<"\n";

    
    //for(int i=0; i<N;i++)
    //    cout << i << " " << map[i].taken <<"\n"; 

    // liberar memoria
    delete[] demands_array;
    delete[] map;

    for(int i = 0; i < N;i++) 
    {
        delete [] d_matrix[i];
    }
    delete [] d_matrix;
    return 0;
}