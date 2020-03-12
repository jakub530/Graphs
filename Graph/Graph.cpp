// Graph.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

bool log_it = true;


class Edge
{
public:
    int dest;
    int cost;
};

class Node
{
public:
    std::vector<Edge> connections;
    int group = -1;
    int index;
    void add_connection(int node_number)
    {
        Edge tmp;
        tmp.dest = node_number;
        tmp.cost = 0;
        connections.push_back(tmp);
    }
};

class Group
{
public:
    std::vector<int> nodes;
    int group_index;
};


class Graph
{
public:
    int node_number = 0;
    std::vector<Node> Nodes;
    std::vector<Group> Groups;

    Graph()
    {

    }

    void Populate_Graph(int size, std::vector<std::vector<int>> input_vector, bool directional = false, int from = 0)
    {
        node_number = size;
        Nodes.resize(size);
        int index = 0;
        for (int i=0;i<Nodes.size();i++)
        {
            Nodes[i].index = index;
            index++;
        }

        for (auto const& row : input_vector)
        {
            if (!directional)
            {
                Nodes[row[0]].add_connection(row[1]);
                Nodes[row[1]].add_connection(row[0]);
            }
            else
            {
                Nodes[row[from]].add_connection(row[1-from]);
            }
            if (log_it)
            {
                std::cout << row[0] << " " << row[1] << "\n";
            }
        }
    }

    void Create_Groups()
    {
        if (log_it)
        {
            std::cout << "Creating Groups" << "\n";
        }
        for (int node_index = 0; node_index < Nodes.size();node_index++)
        {
            if (Nodes[node_index].group == -1)
            {
                Nodes[node_index].group = Groups.size();
                Group tmp;
                tmp.nodes = { Nodes[node_index].index};
                Groups.push_back(tmp);
            }

            for (auto connection : Nodes[node_index].connections)
            {
                if (Nodes[connection.dest].group == -1)
                {
                    Nodes[connection.dest].group = Nodes[node_index].group;
                    Groups[Nodes[node_index].group].nodes.push_back(connection.dest);
                }
                else if (Nodes[connection.dest].group != Nodes[node_index].group)
                {
                    int min_group_index = std::min(Nodes[node_index].group, Nodes[connection.dest].group);
                    int max_group_index = std::max(Nodes[node_index].group, Nodes[connection.dest].group);

                    for (auto node_index : Groups[max_group_index].nodes)
                    {
                        Groups[min_group_index].nodes.push_back(node_index);
                        Nodes[node_index].group = min_group_index;
                    }

                    Groups[max_group_index].nodes.clear();
                    
                }
            }
            if (log_it)
            {
                std::cout << "Node at index:" << Nodes[node_index].index << "is in group " << Nodes[node_index].group << "\n";
            }
        }
        std::vector<Group> Cleared_Groups;
        int new_index = 0;
        for (auto group : Groups)
        {
            
            if (group.nodes.size() != 0)
            {
                
                for (auto node_index : group.nodes)
                {
                    Nodes[node_index].group = new_index;
                }
                group.group_index = new_index;
                Cleared_Groups.push_back(group);
                new_index++;
            }
        }
        Groups = Cleared_Groups;
        if (log_it)
        {
            for (auto group : Groups)
            {
                std::cout << "Group at index " << group.group_index << "\n";
                for (auto n_index : group.nodes)
                {
                    std::cout << n_index << " ";
                }
                std::cout << "\n";
            }
        }

    }

    


};

int main()
{
    std::vector<std::vector<int>> vect{ { 0, 1 },
                                        { 2, 3 } };
    Graph New_Graph;
    New_Graph.Populate_Graph(4,vect);
    New_Graph.Create_Groups();
    std::vector<int> lol;
    
}
