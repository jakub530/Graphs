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
    int source;
    int dest;
    int cost;
    bool bi;

    Edge()
    {

    }

    Edge(int _source, int _dest, int _cost,bool _bi)
    {
        source = _source;
        dest   = _dest;
        cost   = _cost;
        bi     = _bi;
    }

    Edge reverse_edge()
    {
        Edge reversed_edge = *this;
        reversed_edge.source = dest;
        reversed_edge.dest = source;
        return reversed_edge;
    }

    void print_edge()
    {
        std::cout << "Source: " << source << "\n";
        std::cout << "Destination: " << dest << "\n";
        std::cout << "Cost: " << cost << "\n";
        std::cout << "Biderectional: " << bi << "\n";
        std::cout << "\n";
    }


};


bool cost_comp(const Edge& e1, const Edge& e2)
{
    return e1.cost < e2.cost;
}

class Node
{
public:
    std::vector<Edge> connections;
    int group = -1;
    int index;
    void add_connection(Edge connection)
    {
        connections.push_back(connection);
    }

    void print_node()
    {
        std::cout << "Node with index: " << index << "\n";
        std::cout << "With group " << group << "\n";
        std::cout << "Connections:" << "\n";
        int index = 0;
        for (auto connection : connections)
        {
            std::cout << "Connection " << index << "\n";
            connection.print_edge();
        }
        std::cout << "\n";
    }
};

class Group
{
public:
    std::vector<int> nodes;
    int group_index;

    Group(int node_index, int _group_index)
    {
        nodes.push_back(node_index);
        group_index = _group_index;
    }

    Group()
    {

    }
};


class Graph
{
public:
    std::vector<Node> Nodes;
    std::vector<Group> Groups;
    std::vector<Edge> Edges;
    int sum_cost;

    Graph()
    {

    }

    void create_nodes(int size, bool assign_group)
    {
        Nodes.resize(size);
        int index = 0;
        for (size_t i = 0; i < Nodes.size(); i++)
        {
            if (assign_group)
            {
                if (i == 0)
                {
                    Groups.resize(size);
                }
                Group new_group(i, i);
                Groups[i]=(new_group);
                Nodes[i].group = index;
                
            }
            Nodes[i].index = index;
            index++;
        }
    }

    void sort_edges()
    {
        std::sort(Edges.begin(), Edges.end(), cost_comp);
    }

    void connect_nodes(Edge connection)
    {
        Edges.push_back(connection);
        if (connection.bi)
        {
            Edge r_connection = connection.reverse_edge();
            Nodes[connection.source].add_connection(connection);
            Nodes[r_connection.source].add_connection(r_connection);
        }
    }

    void populate_graph(int size, std::vector<std::vector<int>> input_vector, bool directional = true, int from = 0)
    {
        create_nodes(size,false);
        int cost = 0;
        for (auto const& row : input_vector)
        {
            if (row.size() > 2)
            {
                cost = row[2];
            }
            Edge tmp_edge(row[0], row[1], cost, directional);
            connect_nodes(tmp_edge);
        }
    }

    Group group_connected(int start_index, std::vector<bool>&grouped, int group_index)
    {
        Group new_group(start_index, group_index);
        grouped[start_index] = true;
        std::vector<int> nodes_to_visit;
        nodes_to_visit.push_back(start_index);
        while (nodes_to_visit.size() > 0)
        {
            int source_node;
            source_node = nodes_to_visit.back();
            nodes_to_visit.pop_back();
            for (auto connection : Nodes[source_node].connections)
            {
                if (grouped[connection.dest] == false)
                {
                    grouped[connection.dest] = true;
                    Nodes[connection.dest].group = group_index;
                    nodes_to_visit.push_back(connection.dest);
                    new_group.nodes.push_back(connection.dest);
                }
            }
        }
        return new_group;
    }

    void create_groups()
    {
        int group_index = 0;
        std::vector<bool> grouped(Nodes.size(), false);
        for (size_t node_index = 0; node_index < Nodes.size(); node_index++)
        {
            if (grouped[node_index] == false)
            {
                Group new_group = group_connected(node_index, grouped, group_index);
                Groups.push_back(new_group);
                group_index++;
            }
        }
    }

    void print_groups()
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

    void print_nodes()
    {
        for (auto node : Nodes)
        {
            node.print_node();
        }
    }

    void print_edges()
    {
        std::cout << "------------------" << "\n";
        for (auto edge : Edges)
        {

            edge.print_edge();
        }
        std::cout << "=================" << "\n";
    }

    void assimilate_group(int  min_group_index, int max_group_index)
    {
        for (int i = 0; i < Groups[max_group_index].nodes.size(); i++)
        {
            Groups[min_group_index].nodes.push_back(Groups[max_group_index].nodes[i]);
            Nodes[Groups[max_group_index].nodes[i]].group = Groups[min_group_index].group_index;
        }
    }

    int sum_edge_cost()
    {
        int sum = 0;
        for (auto edge : Edges)
        {
            sum = sum + edge.cost;
        }
        return sum;
    }

    Graph find_min_graph()
    {
        Graph min_graph;
        this->sort_edges();
        min_graph.create_nodes(Nodes.size(),true);
        for (auto edge : Edges)
        {
            if (min_graph.Nodes[edge.source].group != min_graph.Nodes[edge.dest].group)
            {
                int min_group_index = std::min(min_graph.Nodes[edge.source].group, min_graph.Nodes[edge.dest].group);
                int max_group_index = std::max(min_graph.Nodes[edge.source].group, min_graph.Nodes[edge.dest].group);
                min_graph.connect_nodes(edge);
                min_graph.assimilate_group(min_group_index, max_group_index);
                if (min_graph.Groups[0].nodes.size() == min_graph.Nodes.size())
                {
                    break;
                }
            }
        }
        min_graph.sum_cost = min_graph.sum_edge_cost();
        //std::cout << "Edge cost sum is " << sum << "\n"; 
        return min_graph;
    }
};

int main()
{
    std::vector<std::vector<int>> vect{ { 0, 1 ,3 },
                                        { 1, 2 , 2}, 
                                        { 2, 3 , 6}, 
                                        { 1, 3 , 4}, 
                                        { 2, 5 , 2}, 
                                        { 2, 0 , 1} };
    Graph New_Graph;
    New_Graph.populate_graph(6,vect);
    New_Graph.print_edges();
    Graph min_graph = New_Graph.find_min_graph();
    min_graph.print_edges();
    std::cout << "Edge cost sum is " << min_graph.sum_cost << "\n";
    //New_Graph.sort_edges();
    //New_Graph.print_edges();

    //New_Graph.create_groups();
    //New_Graph.print_groups();
    //New_Graph.print_nodes();
    std::vector<int> lol;
    
}
