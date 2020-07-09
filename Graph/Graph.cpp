#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

bool log_it = true;

struct ind_cost_pair
{
public:
    int dist;
    int ind;
    ind_cost_pair(int _dist, int _ind)
    {
        dist = _dist;
        ind = _ind;
    }
    ind_cost_pair()
    {
        dist = 0;
        ind = 0;

    }


};

class Edge
{
public:
    int source;
    int dest;
    int cost;
    bool bi;

    Edge()
    {
        source = -1;
        dest = -1;
        cost = -1;
        bi = 0;
    }

    Edge(int _source, int _dest, int _cost, bool _bi)
    {
        source = _source;
        dest = _dest;
        cost = _cost;
        bi = _bi;
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






class Node
{
public:
    std::vector<Edge> connections;
    int group = -1;
    int index = -1;
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

class Node_Djikstra : public Node
{
public:
    double dist;
    int prev;
    bool active;

    Node_Djikstra(Node node)
    {
        connections = node.connections;
        index = node.index;
        dist = std::numeric_limits<int>::max();
        prev = -1;
        active = true;
    }

    Node_Djikstra()
    {
        dist = -1;
        prev = -1;
        active = false;
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
        group_index = -1;
    }
};

bool cost_comp(const Edge& e1, const Edge& e2)
{
    return e1.cost < e2.cost;
}

void edge_transform(std::vector<std::vector<int>>& data)
{
    for (size_t i = 0; i < data.size(); i++)
    {
        data[i][0]--;
        data[i][1]--;
    }
}

bool dist_comp(const ind_cost_pair& n1, const ind_cost_pair& n2)
{
    return n1.dist > n2.dist;
}

std::vector<std::vector<int>> gather_data(std::vector<int> source, std::vector<int> dest, std::vector<int> cost = {})
{
    std::vector<std::vector<int>> data;
    for (size_t i = 0; i < source.size(); i++)
    {
        std::vector<int> tmp_vector;
        tmp_vector.push_back(source[i]);
        tmp_vector.push_back(dest[i]);
        if (cost.size() != 0)
        {
            tmp_vector.push_back(cost[i]);
        }
        data.push_back(tmp_vector);
    }
    return data;
}

class Graph
{
public:
    std::vector<Node> Nodes;
    std::vector<Group> Groups;
    std::vector<Edge> Edges;
    int sum_cost;

    Graph()
    {
        sum_cost = 0;
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
                Groups[i] = (new_group);
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
        create_nodes(size, false);
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

    Group group_connected(int start_index, std::vector<bool>& grouped, int group_index)
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
        for (size_t i = 0; i < Groups[max_group_index].nodes.size(); i++)
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
        min_graph.create_nodes(Nodes.size(), true);
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


    int find_smallest_cost(const std::vector<Node_Djikstra>& d_nodes)
    {
        double smallest = std::numeric_limits<int>::max();
        int min_index = 0;
        for (auto node : d_nodes)
        {
            if (node.active == true)
            {
                if (node.dist <= smallest)
                {
                    smallest = node.dist;
                    min_index = node.index;
                }
            }
        }
        return min_index;
    }

    void insert_at_proper_index(std::vector<ind_cost_pair>& p_nodes, ind_cost_pair p_node)
    {
        std::vector<ind_cost_pair>::iterator low, up;
        up = std::upper_bound(p_nodes.begin(), p_nodes.end(), p_node, dist_comp);
        p_nodes.insert(up, p_node);
    }

    double dijkstra(int source, int dest)
    {
        std::vector<Node_Djikstra> d_nodes(Nodes.size());
        std::vector<ind_cost_pair> queued_nodes;
        for (size_t i = 0; i < d_nodes.size(); i++)
        {
            d_nodes[i] = Node_Djikstra(Nodes[i]);
        }
        ind_cost_pair source_node(0, source);
        queued_nodes.push_back(source_node);

        d_nodes[source].dist = 0;

        double dist = std::numeric_limits<int>::max();

        while (true)
        {
            ind_cost_pair last = queued_nodes.back();
            queued_nodes.pop_back();

            if (d_nodes[last.ind].active == true)
            {
                d_nodes[last.ind].active = false;
                Node_Djikstra u_node = d_nodes[last.ind];

                if (u_node.index == dest)
                {
                    dist = u_node.dist;
                    break;
                }

                for (auto connection : u_node.connections)
                {
                    double alt = std::max(u_node.dist, double(connection.cost));
                    if (alt < d_nodes[connection.dest].dist)
                    {
                        ind_cost_pair p_node(alt, connection.dest);
                        insert_at_proper_index(queued_nodes, p_node);
                        d_nodes[connection.dest].dist = alt;
                        d_nodes[connection.dest].prev = u_node.index;
                    }
                }
            }
            if (queued_nodes.size() == 0)
            {
                break;
            }
        }
        return dist;
    }
};


int main()
{
    std::vector<std::vector<int>> vect{ { 0, 1 ,30 },
                                        { 1, 2 ,50}, 
                                        { 2, 3 ,70}, 
                                        { 3, 4 , 90}, 
                                        { 0, 2 , 70}, 
                                        { 2,4 , 85} };
    Graph New_Graph;
    New_Graph.populate_graph(6,vect);
    New_Graph.print_edges();
    Graph min_graph = New_Graph.find_min_graph();
    min_graph.print_edges();
    std::cout << "Edge cost sum is " << min_graph.sum_cost << "\n";
    std::cout << New_Graph.dijkstra(0, 4) << "\n";

    std::vector<int> lol;
    
}
