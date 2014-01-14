// The Erdos number problem
//
// This is still giving wrong answer, but I suppose it's some input parsing issue.
// The algorithm should be working OK.
//
// I use a collaboration graph to represent the database of authors and their relationships.

#include <iostream>
#include <string>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <climits>
#include <sstream>
#include <vector>

#include <cstdlib>

using namespace std;


// Given a string s and a list of delimiter characters,
// break s into substrings separated by delimiters.
vector<string> tokenize(const string& s, const char *delimiter)
{
    vector<string> res;

    size_t prev = 0;
    size_t next = 0;

    while ((next = s.find_first_of(delimiter, prev)) != string::npos)
    {
        if (next - prev != 0) res.push_back(s.substr(prev, next - prev));

        prev = next + 1;
    }

    if (prev < s.size())
    {
        res.push_back(s.substr(prev));
    }
    
    // Remove trailing whitespaces at the beginning of each substring
    for(vector<string>::iterator it = res.begin(); it != res.end(); it++)
    {
        while((*it)[0] == ' ') (*it).erase(0, 1);
    }

    return res;
}



///////////////////////////////////////////////////////////////
// A class for a vertex in the graph

class Vertex
{
    // A vertex contains a name
    string name;
    
    // State and distance are needed for breadth-first search
    int state;
    int distance;

    // Adjaciency list for this vertex
    list<Vertex *> edges;


public:
    Vertex(const string& myName) : name(myName), state(UNDISCOVERED) { }
    
    const string& getName() const { return name; }
    void addEdge(Vertex* v);
    
    // Set and Get state - for BFS
    void setState(int state) { this->state = state; }
    int getState() const { return state; }
    
    // States for BFS
    enum { UNDISCOVERED, DISCOVERED, PROCESSED };
    
    // Set and Get distance - for BFS
    int setDistance(int distance) { this->distance = distance; }
    int getDistance() const { return distance; }
    
    // We declare the Graph class friend, because it needs to access
    // the adjaciency list of each Vertex
    friend class Graph;

};


void Vertex::addEdge(Vertex *v)
{
    if(v != NULL)
        edges.push_back(v);
    
    //else
        // throw some exception... 
}


///////////////////////////////////////////////////////////////
// A class for a graph (undirected, unweighted)

class Graph
{
    // List of vertices in the graph
    list<Vertex *> vertices;
    
public:
    Graph() {}
    ~Graph();
    
    void addVertex(const string& name);
    void addEdge(const string& v1, const string& v2);
    
    // Find a vertex, given a name
    Vertex *findVertex(const string& name) const;
    
    void BFS(const string& rootName);
};


Graph::~Graph()
{
    for(list<Vertex *>::const_iterator it = vertices.begin(); it != vertices.end(); it++)
        delete *it;
}


Vertex *Graph::findVertex(const string& name) const
{
    for(list<Vertex *>::const_iterator it = vertices.begin(); it != vertices.end(); it++)
    {
        // Obtain pointer from iterator
        Vertex *v = *it;
        
        // If we find a vertex with the given string, we return it
        if(v->getName() == name) return v;
    }
    
    // If we haven't found a vertex with the given string, we return a null pointer
    return NULL;
}


void Graph::addVertex(const string& name)
{
    if(findVertex(name) == NULL) // Add a vertex only if it's not already existing
    {
        Vertex *v = new Vertex(name);
        vertices.push_back(v);    
    }
}


void Graph::addEdge(const string& name1, const string& name2)
{
    Vertex *v1, *v2;
    
    v1 = findVertex(name1);
    v2 = findVertex(name2);
    
    if(v1 == NULL || v2 == NULL)
    {
        cout << "Error: non-existing vertex." << endl;
        return;
    }
    
    v1->addEdge(v2);
    v2->addEdge(v1);
}


// Standard breadth-first search algorithm
void Graph::BFS(const string& rootName)
{
    Vertex *root = NULL;
    
    for(list<Vertex *>::iterator it = vertices.begin(); it != vertices.end(); it++)
    {
        Vertex *u = *it;
        
        if(u->getName() == rootName)
        {
            u->setState(Vertex::DISCOVERED);
            u->setDistance(0);
            root = u;
        }
        else
        {
            u->setState(Vertex::UNDISCOVERED);
            u->setDistance(INT_MAX);
        }
    }
    
    if(root == NULL)
    {
    //    cout << "BFS: root vertex not found" << endl;
    //    throw 0;
    
    // If root is null, it means that Erdos was not found in the list of papers.
    // Therefore nothing else needs to be done: all nodes have distance infinite.
        return;
    }
    
    queue<Vertex *> q;
    q.push(root);
    
    while(!q.empty())
    {
        Vertex *u = q.front();
        q.pop();
        
        for(list<Vertex *>::iterator it = u->edges.begin(); it != u->edges.end(); it++)
        {
            Vertex *v = *it;
            
            if(v->getState() == Vertex::UNDISCOVERED)
            {
                v->setState(Vertex::DISCOVERED);
                //if(u->getDistance() == INT_MAX) v->setDistance(INT_MAX); else v->setDistance(u->getDistance() + 1);
                v->setDistance(u->getDistance() + 1);
                q.push(v);
            }
        }
        
        u->setState(Vertex::PROCESSED);        
    }
    
}



///////////////////////////////////////////////////////////////
// A class representing a single paper

class Paper
{
public:
    Paper() {}
    
    void print()
    {
        for(set<string>::iterator it = authors.begin(); it != authors.end(); it++)
            cout << *it << endl;
    }
    
    // Read a paper from cin and returns the new object
    void readFromCin();
    
    // Helper method to parse a name from cin
    static bool parseName(string& name);

    set<string> authors;
};


// Read a paper from cin
void Paper::readFromCin()
{     
    string line;
    getline(cin, line);
    
    vector<string> names = tokenize(tokenize(line, ":")[0], ",");
    
    vector<string>::iterator it = names.begin();
    while(it != names.end())
    {
        string author = *it + ", " + *(it + 1);
        authors.insert(author);
        
        it += 2;
    }
    
}


// Parse an author name from cin
/*bool Paper::parseName(string& name)
{
    char c;
    
    bool last_name = false;
    
    enum { SURNAME, INITIALS, DONE };
    int state = SURNAME;
    
    while(state != DONE)
    {
        cin >> c;
        
        if(c == ',' && state == SURNAME)
        {
            name += c;
            name += ' ';
            state = INITIALS;
        }
        else if(c == ',' && state == INITIALS)
        {
            state = DONE;
        }
        else if(c == ':' && state == INITIALS)
        {
            state = DONE;
            last_name = true;
        }
        else
        {
            name += c;
        }
        
    }
    
    return last_name;
}
*/


///////////////////////////////////////////////////////////////
// A class representing a scenario

class Scenario
{
    Graph authorsGraph; // Collaboration graph of authors
    int n_papers;
    int n_queries;

public:
    Scenario() {}
    
    void process();
};


void Scenario::process()
{
    // Read the number of papers and queries
    string line;
    getline(cin, line);
    
    stringstream ss(line);
    
    ss >> n_papers;
    ss >> n_queries;
    
    // Process papers and build the authors graph
    for(int i = 0; i < n_papers; i++)
    {
        Paper p;
        p.readFromCin();

        set<string>::iterator it1, it2;
        
        for(it1 = p.authors.begin(); it1 != p.authors.end(); it1++)
            authorsGraph.addVertex(*it1);
        
        for(it1 = p.authors.begin(); it1 != p.authors.end(); it1++)
            for(it2 = p.authors.begin(); it2 != p.authors.end(); it2++)
                if(*it1 != *it2) authorsGraph.addEdge(*it1, *it2);
    }
    
    // Run BFS on the authors graph
    authorsGraph.BFS("Erdos, P.");
    
    // Process queries
    for(int i = 0; i < n_queries; i++)
    {
        string query;
        getline(cin, query);
        
        cout << query << " ";
        
        Vertex *v = authorsGraph.findVertex(query);
        if(v == NULL) cout << "infinity" << endl;
        else if(v->getDistance() == INT_MAX) cout << "infinity" << endl;
        else cout << v->getDistance() << endl;
    }
}


///////////////////////////////////////////////////////////////



int main()
{   
    // Read the number of scenarios in the input
    int n_scenarios;
    
    string line;
    getline(cin, line);
    
    stringstream ss(line);    
    ss >> n_scenarios;
    
    // Process each scenario
    for(int i = 0; i < n_scenarios; i++)
    {      
        cout << "Scenario " << i+1 << endl;
        Scenario s;
        s.process();
        
        //cout << endl;
    }

    return 0;
}
