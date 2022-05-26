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

vector<string> split_string(string input_string, char seperator)
{
    stringstream string_stream(input_string);
    vector<string> result;
    string current;
    while(getline(string_stream, current, seperator))
        result.push_back(current);
    return result;
}

void manage_topology_and_modify_command(vector<string> commands, string command_type)
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

void manage_show_command()
{
    cout << "\nu|v |\t" ;
    for(int i = 0; i < number_of_nodes; i++)
        cout << i+1 << "\t" ;
    cout << endl;

    for(int i = 0; i <= number_of_nodes; i++)
        cout << "--------" ;
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

void manage_lsrp_command(vector<string> commands)
{

}

void manage_dvrp_command(vector<string> nodes) 
{
    vector<vector<int>> next_hop(number_of_nodes + 1, vector<int>(number_of_nodes+ 1, NO_EDGE));

    for (int i = 0; i < number_of_nodes + 1; i++)
        next_hop[i][i] = i;

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
                        next_hop[i][j] = next_hop[i][k] != i ? next_hop[i][k] : k;
                    }

    if(nodes.size() == 0)
        for (int i = 0; i < number_of_nodes + 1; i++)
            nodes.push_back(to_string(i));

    for (auto node : nodes)
    {
        cout << "\nRouting table for node " << stoi(node) << ":" << endl;
        cout << "Dest\tNext Hop\tDist\tShortest path" << endl;
        for (int j = 1; j < number_of_nodes + 1; j++)
        {
            cout << j << "\t";
            if (next_hop[stoi(node)][j] != stoi(node))
                cout << next_hop[stoi(node)][j] << "\t\t";
            else
                cout << j << "\t\t";
            cout << distance[stoi(node)][j] << "\t" << endl;
            // TODO Shortest path
        }
    }
}

// topology 1-2-19 1-3-9 2-4-3

void manage_remove_command(vector<string> commands)
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
            topology[first_node][second_node] = -1;
            topology[second_node][first_node] = -1;
        }
    }
}

bool manage_command(vector<string> commands)
{
    string first_command = commands[0];
    commands.erase(commands.begin());
    if(first_command == "topology")
        manage_topology_and_modify_command(commands, first_command);
    else if(first_command == "show")
        manage_show_command();
    else if(first_command == "lsrp")
        manage_lsrp_command(commands);
    else if(first_command == "dvrp")
        manage_dvrp_command(commands);
    else if(first_command == "modify")
        manage_topology_and_modify_command(commands, first_command);
    else if(first_command == "remove")
        manage_remove_command(commands);
    else if(first_command == "quit")
        return true;
    return false;
}

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
        quit = manage_command(commands);
    }
    return 0;
}