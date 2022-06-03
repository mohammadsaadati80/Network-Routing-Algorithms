#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <time.h>
#include <fstream>
#include <sstream>
#include <cmath>
#include <iomanip>

using namespace std;

const int MAX_TOPOLOGY_SIZE = 50;
const int NO_EDGE = INT32_MAX;
const char MAIN_SEPARATOR = ' ';
const char NUMBER_SEPARATOR = '-';

int number_of_nodes = 0;
vector<vector<int>> topology(MAX_TOPOLOGY_SIZE + 1 , vector<int>(MAX_TOPOLOGY_SIZE + 1, NO_EDGE)); 

vector<string> split_string(string input_string, char seperator);
bool handle_commands(vector<string> commands);
void handle_topology_and_modify_command(vector<string> commands, string command_type);
void handle_show_command();
void handle_lsrp_command(vector<string> nodes);
pair<vector<int>, vector<int>> dijkstra(int src);
void show_lsrp_iteration_table(vector<int> dist, int iteration);
void show_lrsp_table(vector<int> &parent, vector<int> &dist, int src);
void handle_dvrp_command(vector<string> nodes);
string dvrp_shortest_path(string shortest_path);
void handle_remove_command(vector<string> commands);

int main()
{  
    for(int i = 0; i < MAX_TOPOLOGY_SIZE + 1; i++)
        topology[i][i] = 0;
    bool quit = false;
    while(!quit)
    {
        cout << endl;
        string command_line;
        getline(cin, command_line);
        vector<string> commands = split_string(command_line, MAIN_SEPARATOR);
        quit = handle_commands(commands);
    }
    return 0;
}

vector<string> split_string(string input_string, char seperator)
{
    stringstream string_stream(input_string);
    vector<string> result;
    string current;
    while(getline(string_stream, current, seperator))
        result.push_back(current);
    return result;
}

bool handle_commands(vector<string> commands)
{
    string first_command = commands[0];
    commands.erase(commands.begin());
    if(first_command == "topology")
        handle_topology_and_modify_command(commands, first_command);
    else if(first_command == "show")
        handle_show_command();
    else if(first_command == "lsrp")
        handle_lsrp_command(commands);
    else if(first_command == "dvrp")
        handle_dvrp_command(commands);
    else if(first_command == "modify")
        handle_topology_and_modify_command(commands, first_command);
    else if(first_command == "remove")
        handle_remove_command(commands);
    else if(first_command == "quit")
        return true;
    else
        cout << "\nInvalid command." << endl;
    return false;
}

void handle_topology_and_modify_command(vector<string> commands, string command_type)
{
    for(auto current_command : commands)
    {
        vector<string> numbers = split_string(current_command, NUMBER_SEPARATOR);
        if(numbers[0] == numbers[1])
            cout << "Wrong "<< command_type << " input: " << current_command << endl;
        else
        {
            int first_node = stoi(numbers[0]);
            int second_node = stoi(numbers[1]);
            int cost = stoi(numbers[2]);
            topology[first_node][second_node] = cost;
            topology[second_node][first_node] = cost;
            number_of_nodes = max(number_of_nodes, max(first_node, second_node));
        }
    }
}

void handle_show_command()
{
    cout << "\nu|v |\t" ;
    for(int i = 0; i < number_of_nodes; i++)
        cout << i+1 << "\t" ;
    cout << endl;

    for(int i = 0; i <= number_of_nodes; i++)
        cout << "------" ;
    cout << endl;

    for(int i = 1; i < number_of_nodes + 1; i++)
    {
        cout << i << "  |" ;
        for(int j = 1; j < number_of_nodes + 1; j++)
            if(topology[i][j] == NO_EDGE)
                cout << "\t" << -1;
            else
                cout << "\t" << topology[i][j];
        cout << endl;
    }
}

void handle_lsrp_command(vector<string> nodes) 
{
    if(nodes.size() == 0)
        for (int i = 1; i < number_of_nodes + 1; i++)
            nodes.push_back(to_string(i));

    for (auto src : nodes)
    {
        auto p = dijkstra(stoi(src) - 1);
        vector<int> parent = p.first;
        vector<int> dist = p.second;
        show_lrsp_table(parent, dist, stoi(src) - 1);
    }  
}

pair<vector<int>, vector<int>> dijkstra(int src)
{
    vector<int> parent(number_of_nodes , src);
    vector<int> dist(number_of_nodes , NO_EDGE);
    vector<bool> flag(number_of_nodes , false);

    parent[src] = -1;
    dist[src] = 0;

    for (int c = 0; c < number_of_nodes - 1; c++)
    {
        int min_val = NO_EDGE, min_index;
        for (int i = 0; i < number_of_nodes ; i++)
            if (flag[i] == false and dist[i] < min_val)
            {
                min_val = dist[i];
                min_index = i;
            }
        flag[min_index] = true;
        for (int v = 0; v < number_of_nodes ; v++)
            if (!flag[v] and topology[min_index + 1][v + 1] != NO_EDGE and dist[min_index] + topology[min_index + 1][v + 1] < dist[v])
            {
                parent[v] = min_index;
                dist[v] = dist[min_index] + topology[min_index + 1][v + 1];  
            }
        show_lsrp_iteration_table(dist, c + 1);
    }
    return {parent, dist};
}

void show_lsrp_iteration_table(vector<int> dist, int iteration) //TODO
{
    cout << endl << "\t" << "Iter " << iteration << ":" << endl;
    cout << "Dest\t|";
    for (int i = 1; i < number_of_nodes + 1; i++)
        cout << "  " << i << "|";
        
    cout << endl << "Cost\t|";
    for (int i = 0; i < dist.size(); i++)
        if (dist[i] == NO_EDGE)
            cout << "  " << -1 << "|";
        else
            cout << "  " << dist[i] << "|";

    cout << endl;
    for(int i = 0; i <= number_of_nodes; i++)
        cout << "------" ;
    cout << endl;
}

void show_lrsp_table(vector<int> &parent, vector<int> &dist, int src) //TODO
{
  int V = dist.size();
  cout << "\nRouting table for node " << src + 1 << ":\n";
  cout <<  "Path: [s]->[d]" << "\t  " << "  Min_Cost" << "\t  " << "Shortest Path" << endl;
  cout << "      ----------    ----------    --------------" << endl;
  for (int i = 0; i < V; i++)
  {
    if (i!=src)
    {
        vector<int> path;
        int memo = i;
        for (int min_index = i; parent[min_index] != -1; min_index = parent[min_index])
            memo = min_index;
        for (int min_index = i; min_index != -1; min_index = parent[min_index])
            path.push_back(min_index);
        reverse(path.begin(), path.end());
        cout << "      " << '[' << src + 1 << "]->[" << i+1 << ']' << "        " << dist[i] << "          ";
        for (int p = 0; p < path.size() - 1; p++)
            cout << path[p]+1 << "->";
        cout << path[path.size() - 1] + 1 << "\n";
    }
  }
}

void handle_dvrp_command(vector<string> nodes) 
{
    vector<vector<int>> next_hop(number_of_nodes + 1, vector<int>(number_of_nodes+ 1, NO_EDGE));
    vector<vector<string>> shortest_path(number_of_nodes + 1, vector<string>(number_of_nodes+ 1, ""));
    
    for (int i = 0; i < number_of_nodes + 1; i++)
        next_hop[i][i] = i;

    for (int i = 1; i < number_of_nodes + 1; i++)
        for (int j = 1; j < number_of_nodes + 1; j++)
            if(i == j)
                shortest_path[i][i] = to_string(i);
            else if (topology[i][j] != NO_EDGE)
                shortest_path[i][j] = to_string(i) + NUMBER_SEPARATOR + to_string(j) + NUMBER_SEPARATOR;

    for (int i = 1; i < number_of_nodes + 1; i++)
        for (int j = 1; j < number_of_nodes + 1; j++)
            if (topology[i][j] != NO_EDGE)
                next_hop[i][j] = i;

    vector<vector<int>> distance = topology;
    for (int k = 1; k < number_of_nodes + 1; k++)
        for (int i = 1; i < number_of_nodes + 1; i++)
            for (int j = 1; j < number_of_nodes + 1; j++)
                if (distance[i][k] != NO_EDGE and distance[k][j] != NO_EDGE and distance[i][k] + distance[k][j] < distance[i][j])
                    {
                        distance[i][j] = distance[i][k] + distance[k][j];
                        shortest_path[i][j] = shortest_path[i][k] + NUMBER_SEPARATOR + shortest_path[k][j] + NUMBER_SEPARATOR;
                        next_hop[i][j] = next_hop[i][k] != i ? next_hop[i][k] : k;
                    }

    if(nodes.size() == 0)
        for (int i = 1; i < number_of_nodes + 1; i++)
            nodes.push_back(to_string(i));

    for (auto node : nodes)
    {
        cout << "\nRouting table for node " << stoi(node) << ":" << endl;
        cout << "Dest\tNext Hop\tDist\tShortest path" << endl;
        cout << "--------------------------------------------------------" << endl;
        for (int j = 1; j < number_of_nodes + 1; j++)
        {
            cout << j << "\t";
            if (next_hop[stoi(node)][j] != stoi(node))
                cout << next_hop[stoi(node)][j] << "\t\t";
            else
                cout << j << "\t\t";
            cout << distance[stoi(node)][j] << "\t" ;
            cout << dvrp_shortest_path(shortest_path[stoi(node)][j]) << endl;
        }
    }
}

string dvrp_shortest_path(string shortest_path)
{
    vector<string> numbers = split_string(shortest_path, NUMBER_SEPARATOR);
    vector<string> path;
    for(int i = 0; i < numbers.size(); i++)
        if (numbers[i] != "")
            if(path.size() == 0)
                path.push_back(numbers[i]);
            else if(path[path.size() - 1] != numbers[i])
                path.push_back(numbers[i]);
    string result = "[";
    for (int i = 0; i < path.size(); i++)
        result += (path.size() - 1) == i ? path[i] : (path[i] + "->");
    result += "]";
    return result;
}

void handle_remove_command(vector<string> commands)
{
    for(auto current_command : commands)
    {
        vector<string> numbers = split_string(current_command, NUMBER_SEPARATOR);
        if(numbers[0] == numbers[1])
            cout << "Wrong remove input: " << current_command << endl;
        else
        {
            int first_node = stoi(numbers[0]);
            int second_node = stoi(numbers[1]);
            topology[first_node][second_node] = NO_EDGE;
            topology[second_node][first_node] = NO_EDGE;
        }
    }
}
