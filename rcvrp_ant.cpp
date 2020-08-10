#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <numeric> //std::iota
#include <algorithm>
#include <random>
#include <cmath>
#include <limits>

using namespace std;

int int_rand(int a, int b)
{
    int retorno = 0;

    if (a < b)
    {
        retorno = (int)((b - a) * drand48());
        retorno = retorno + a;
    }
    else
    {
        retorno = (int)((a - b) * drand48());
        retorno = retorno + b;
    }

    return retorno;
}

float float_rand(float a, float b)
{
    float retorno = 0;

    if (a < b)
    {
        retorno = (float)((b - a) * drand48());
        retorno = retorno + a;
    }
    else
    {
        retorno = (float)((a - b) * drand48());
        retorno = retorno + b;
    }

    return retorno;
}

struct coordinates
{
    float x, y;
    bool taken;
};

float distance(coordinates v, coordinates u)
{
    return sqrt(pow(v.x - u.x, 2) + pow(v.y - u.y, 2));
}

float short_FO(int i, int j, float **d_matrix, float current_risk, float R)
{
    float alpha = current_risk / R;
    return ((1 - alpha) * d_matrix[i][j] + alpha * d_matrix[j][0]);
}

float single_FO(vector<int> solution, float **d_matrix, coordinates *&map)
{
    float dist = 0.0;
    if (!solution.empty())
    {
        dist = distance(map[0], map[solution[0]]);
        size_t i;
        for (i = 0; i < solution.size() - 1; i++)
        {
            dist += d_matrix[solution[i]][solution[i + 1]];
        }
        dist += distance(map[solution.back()], map[0]);
    }
    return dist;
}

float FO(vector<vector<int>> solution, float **d_matrix, coordinates *&map)
{
    float dist = 0.0;
    for (size_t i = 0; i < solution.size(); i++)
    {
        dist += single_FO(solution[i], d_matrix, map);
    }
    return dist;
}


void print_solution(vector<vector<int>> sol, float **d_matrix, coordinates *map)
{
    cout << "Solution:\n";
    for (size_t n = 0; n < sol.size();n++)
    {
        for (size_t i = 0; i < sol[n].size(); i++)
        {
            cout << sol[n][i] << " ";
        }
        cout << "\n";
    }
    cout << "FO:" << FO(sol, d_matrix, map) << "\n";
}


bool update_demand(coordinates *&map, int N)
{
    for (int i = 0; i < N; i++)
    {
        if (map[i].taken == false)
            return true;
    }
    return false;
}

float route_demand(int *demands_array, vector<int> sol, size_t n)
{
    float r_demand = 0.0;
    size_t i;
    if (!sol.empty())
    {
        for (i = 0; i < n; i++)
        {
            r_demand += demands_array[sol[i]];
        }
    }
    return r_demand;
}

float route_risk(int *demands_array, float **d_matrix, vector<int> sol)
{
    float r_risk = 0.0;

    if (!sol.empty())
    {
        //cout << "0 - " << sol[0] << " - ";

        if (sol.size() > 1)
        {
            for (size_t j = 0; j < sol.size() - 1; j++)
            {
                //cout  << sol[j+1] << " - ";
                r_risk += d_matrix[sol[j]][sol[j + 1]] * route_demand(demands_array, sol, j + 1);
            }
        }

        //cout << "la wea p " << sol.size()  << " sjaja ";
        r_risk += d_matrix[sol.back()][0] * route_demand(demands_array, sol, sol.size());
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

float p_ant(float alpha, float beta, float **ph_matrix, float **d_matrix, vector<int> sol, coordinates *&map, int N, int i)
{
    if (!sol.empty())
    {
        float p = pow(ph_matrix[sol.back()][i], alpha) * pow(1 / d_matrix[sol.back()][i], beta);
        //cout <<"p: " << d_matrix[sol.back()][i] << sol.back() <<" "<< i << "\n";
        float b = 0.0;
        for (int n = 1; n < N; n++)
        {
            if (!map[n].taken)
                b += pow(ph_matrix[sol.back()][n], alpha) * pow(1 / d_matrix[sol.back()][n], beta);
        }
        if (b != 0.0)
            return p / b;
    }
    return 1.0;
}

int choose_node(float alpha, float beta, float rho, float **ph_matrix, vector<int> sol, int *demands_array, float **d_matrix, coordinates *&map, int N, float R)
{
    bool flag = false;
    float pro, current_risk;
    int next_pos = -1;
    vector<float> probs;
    vector<int> choices;

    for (int i = 1; i < N; i++)
    {
        if (!map[i].taken)
        {
            sol.push_back(i);
            current_risk= route_risk(demands_array, d_matrix, sol);
            sol.pop_back();
            if (current_risk < R)
            {
                choices.push_back(i);
                pro = p_ant(alpha, beta, ph_matrix, d_matrix, sol, map, N, i);
                //cout << "pro: " << pro<<"\n";
                probs.push_back(pro);
                flag = true;
            }
        }
    }

    float rand_choice = float_rand(0.0, 1.0);

    if (flag)
    {
        for (size_t j = 0; j < probs.size()-1; j++)
        {
            if (rand_choice < probs[j])
            {
                next_pos = choices[j];
                return next_pos;
            }
            else
            {
                probs[j + 1] += probs[j];
            }
        }
        next_pos = choices.back();
    }
    return next_pos;
}

vector<vector<int>> ant_solver(float alpha, float beta, float rho, float **ph_matrix, int *demands_array, coordinates *map, float **d_matrix, int N, float R)
{
    vector<vector<int>> solution;
    bool demand = true;
    // while there's demand, choose which vehicle movement minimizes the current vehicle FO
    while (demand)
    {

        vector<int> sol;
        int node_candidate = -1;
        while (true)
        {
            node_candidate = choose_node(alpha, beta, rho, ph_matrix, sol, demands_array, d_matrix, map, N, R);
            if (node_candidate == -1)
                break;
            //cout <<"candidate: " << node_candidate << "\n";
            sol.push_back(node_candidate);
            map[node_candidate].taken = true;
        }
        solution.push_back(sol);
        demand = update_demand(map, N);
    }
    return solution;
}

float dif_tau(vector<vector<vector<int>>> ants, float **d_matrix, int i, int j)
{
    float dif = 0.0;
    for (size_t k = 0; k < ants.size(); k++)
    {
        for (size_t n = 0; n < ants[k].size(); n++)
        {
            for (size_t l = 0; l < ants[k][n].size() - 1; l++)
                if (ants[k][n][l] == i && ants[k][n][l+1] == j)
                    dif += 1 / d_matrix[ants[k][n][l]][ants[k][n][l + 1]];
        }
    }
    return dif;
}
void update_pheromones(float** &ph_matrix, float **d_matrix, vector<vector<vector<int>>> ants, float rho, int N)
{
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            ph_matrix[i][j] = (1 - rho) * ph_matrix[i][j] + dif_tau(ants, d_matrix, i, j);
}

vector<vector<int>> ant_colony(int n_ants, float alpha, float beta, float rho, float tau, int max_it, int max_non, int *demands_array, coordinates *map, float **d_matrix, int N, float R)
{
    // pheromone matrix
    float **ph_matrix = new float *[N];
    for (int i = 0; i < N; ++i)
    {
        ph_matrix[i] = new float[N];
    }
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            ph_matrix[i][j] = tau;

    float min_dist = numeric_limits<float>::infinity(), cur_dist;
    // ants
    vector<vector<vector<int>>> ants;

    vector<vector<int>> best_solution;
    int it=0, ni=0;
    bool flag;
    // while criteria
    while (it <max_it && ni < max_non)
    {
        //form solution for each ant
        flag = true;
        for (int m = 0; m < n_ants; m++)
        {
            ants.push_back(ant_solver(alpha, beta, rho, ph_matrix, demands_array, map, d_matrix, N, R));
            for (int k=1;k<N;k++)
                map[k].taken = false;
            //print_solution(ants[m], d_matrix, map);
        }

        update_pheromones(ph_matrix, d_matrix, ants, rho, N);
        for (int m = n_ants-1; m > -1; m--)
        {
            //cout << "shoo: " << m  <<"\n";
            cur_dist = FO(ants[m], d_matrix, map);
            //cout << "shoot: " << m  <<"\n";
            if (cur_dist < min_dist)
            {
                min_dist = cur_dist;
                best_solution = ants[m];
                flag = false;
            }
            ants.pop_back();
        }
        if (flag)
            ni++;
        it++;
    }
    for (int i = 0; i < N; i++)
    {
        delete[] ph_matrix[i];
    }
    delete[] ph_matrix;

    return best_solution;
}

int main(int argc, char *argv[])
{
    // setting parameters
    int seed = (int)atoi(argv[1]);
    int n_ants = (int)atoi(argv[2]);
    float alpha = (float)atof(argv[3]);
    float beta = (float)atof(argv[4]);
    float rho = (float)atof(argv[5]);
    float tau = (float)atof(argv[6]);
    int max_it=(int)atoi(argv[7]);
    int max_non=(int)atoi(argv[8]);;
    //cout << "Seed: " << seed << "\n";

    srand48(seed);

    // Numer of nodes including the depot
    int N;
    // Risk threshold
    float R;
    // demand array
    int *demands_array;
    // nodes coordinates;
    coordinates *map;

    // read inputs
    cin >> N;
    cin >> R;

    // fill demand array
    demands_array = new int[N];
    for (int i = 0; i < N; i++)
    {
        cin >> demands_array[i];
    }

    // make distance map
    map = new coordinates[N];
    for (int i = 0; i < N; i++)
    {
        cin >> map[i].x;
        cin >> map[i].y;
    }

    map[0].taken = true;
    // distance matrix
    float **d_matrix = new float *[N];
    for (int i = 0; i < N; ++i)
    {
        d_matrix[i] = new float[N];
    }
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            d_matrix[i][j] = distance(map[i], map[j]);
    //solution vector array
    vector<vector<int>> sol;
    sol = ant_colony(n_ants, alpha, beta, rho, tau, max_it, max_non, demands_array, map, d_matrix, N, R);
    if (!sol.empty())
    {
        // print solution

        //for (size_t i = 0; i < sol.size(); i++)
        //{
        //    cout << "vehicle " << i <<": ";
        //    for (vector<int>::iterator it = sol[i].begin(); it != sol[i].end(); ++it)
        //    {
        //        cout << *it << " ";
        //    }
        //    cout << "risk: " << route_risk(demands_array, d_matrix, sol[i]) << "\n";
        //}
        //cout << "FO: ";
        /////check that all nodes were visited
        //for (int i = 0; i < N; i++)
        //   cout << map[i].taken << "\n";

        cout << FO(sol, d_matrix, map) << "\n";
    }
    else
    {
        cout << -1 << "\n";
    }

    // free memory
    delete[] demands_array;
    delete[] map;

    for (int i = 0; i < N; i++)
    {
        delete[] d_matrix[i];
    }
    delete[] d_matrix;

    return 0;
}