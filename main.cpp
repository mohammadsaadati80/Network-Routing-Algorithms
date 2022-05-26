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
const int NO_EDGE = -1;
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
            cout << "\t" << topology[i][j];
        cout << endl;
    }
}

void manage_lsrp_command(vector<string> commands)
{

}

void manage_dvrp_command(vector<string> commands)
{

}

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