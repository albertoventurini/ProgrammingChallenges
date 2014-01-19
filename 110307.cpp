// This solves the Doublets problem on the UVa Online Judge (http://uva.onlinejudge.org/)
// It doesn't solve the same problem on the programming-challenges.com website, because that judge has a known bug.
//
// The problem is solved by creating a graph of words, then running an A* search on it.
//
// The graph of words if built such that each doublet is connected.
// Moreover, I have used 'sentinel' vertices. For example, the word 'booster' has the following sentinels:
//   - *ooster
//   - b*oster
//   - bo*ster
//   - boo*ter
//   - boos*er
//   - boost*r
//   - booste*
//
// Therefore, doublets share sentinels.
// I exploit this topography to build the graph rapidly: when a word 'w' is inserted,
// I generate its sentinels and search for them in the graph; each sentinel's neighbors
// will be w's neighbors (and vice versa).

#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>
#include <climits>
#include <algorithm>
#include <set>
#include <stack>


using namespace std;



class Vertex
{
    list<Vertex *> neighbors;
    
    friend class Graph;
        
    bool sentinel;
    string word;

public:
    Vertex(const string& w, bool s) : word(w), sentinel(s) { }
    
    bool isSentinel() { return sentinel; }
    
    list<string> neighborWords()
    {
        list<string> result;

        for(Vertex *n : neighbors)
            result.push_back(n->word);
        
        return result;
    }
    
};


class Graph
{
    map<string, Vertex *> words;
    
    vector<string> calcSentinels(const string& word);

public:
    Graph() {}
    ~Graph() {}
    
    Vertex* newVertex(const string& w, bool sentinel);
    Vertex* findVertex(const string& w) const;
    
    void insertWord(const string& w);
    bool findWord();
    
    int hamming(const string& w1, const string& w2);
    
    void query(const string& source, const string& target);
    bool queryRecursive(const string& source, const string& target, list<string>& solution, set<string>& visited);

    void Astar(const string& source, const string& target);
    void Astar_print_path(map<string, string> came_from, string current);
};


// Counts the number of letters that are the same in the two words
int Graph::hamming(const string& w1, const string& w2)
{
    if(w1.length() != w2.length()) return INT_MAX;
    
    int count = 0;
    
    for(int i = 0; i < w1.length(); i++)
        if(w1[i] != w2[i]) count++;
    
    return count;
}



// Creates a new, empty vertex and add it to the graph
Vertex *Graph::newVertex(const string& word, bool sentinel)
{
    Vertex *vertex = new Vertex(word, sentinel);
    words.insert(pair<string, Vertex *>(word, vertex));
    
    return vertex;
}

// Returns the vertex corresponding to the given word
Vertex* Graph::findVertex(const string& word) const
{
    // Gets an iterator to the word
    auto it = words.find(word);
    
    // If the word was not found, return NULL
    if(it == words.end())
        return NULL;
    else
        return it->second; // else, return the value of the pair
}


// Generates all possible sentinels for the given word
vector<string> Graph::calcSentinels(const string& word)
{
    vector<string> result;
    
    for(int i = 0; i < word.length(); i++)
    {
        string s(word);
        s[i] = '*';
        result.push_back(s);
    }
    
    return result;    
}


void Graph::insertWord(const string& word)
{
    // If the vertex already exists in the graph, there's nothing else to do
    if(findVertex(word) != NULL) return;
    
    // Create a new vertex for the given word
    Vertex *v = newVertex(word, false);
    
    // Calculates all sentinels for the given word
    vector<string> sentinels = calcSentinels(word);
      
    // Now we need to update the graph in the following way:
    //   - for each sentinel word, find the corresponding vertex 'sent_vertex' (or create a new one if it doesn't already exist)
    //   - set the new vertex 'v' as a neighbor of each sent_vertex's neighbor (and vice versa)
    //   - set the new vertex 'v' as a neighbor of sent_vertex
    for(string& sentinel : sentinels)
    {
        Vertex *sent_vertex = findVertex(sentinel);
        if(sent_vertex == NULL) sent_vertex = newVertex(sentinel, true);

        for(Vertex *sent_neighbor : sent_vertex->neighbors)
        {
            v->neighbors.push_back(sent_neighbor);
            sent_neighbor->neighbors.push_back(v);
        }
        
        sent_vertex->neighbors.push_back(v);
    }
}



struct CompareSecond
{
    bool operator()(const pair<string, int>& left, const pair<string, int>& right) const
    {
        return left.second < right.second;
    }
};


// Standard A* search (see Wikipedia for pseudocode).
// The cost function f is given by the following two components:
//   - g = number of steps from the source to the current vertex
//   - h = hamming distance from the current vertex to the target
void Graph::Astar(const string& source, const string& target)
{   
    if(findVertex(source) == NULL || findVertex(target) == NULL)
    {
        cout << "No solution." << endl;
        return;
    }
    
    if(source.length() != target.length())
    {
        cout << "No solution." << endl;
        return;
    }
    
    // 'closedset' contains the strings (i.e. vertices) that have already been processed in the search
    // (it's just handier for me to keep the strings rather than Vertex objects)
    set<string> closedset;
    
    // 'came_from' maps the search path which will be reconstructed at the end
    map<string, string> came_from;
    
    // 'g_score' contains the minimum g score for each word throughout the search
    map<string, int> g_score;
    g_score[source] = 0; // The g score of the source is obviously 0

    // 'openset' contains the list of nodes that need to be processed next.
    // It is an associative array because each node (string) is associated to an f score.
    // At each iteration of the A* search, we will pick the node with the lowest f score for processing.
    // It should be a priority queue, not a map... but I just iterate through it each time to find the minimum element.
    map<string, int> openset;
    openset[source] = g_score[source] + hamming(source, target);
    
    while(!openset.empty())
    {
        // Find the next string (vertex) to be processed, i.e. the vertex in openset with lowest f score
        auto min = min_element(openset.begin(), openset.end(), CompareSecond());
        
        string current = min->first;
        
        // If current == target, we have finished
        if(current == target)
        {
            Astar_print_path(came_from, current);
            return;
        }
        
        closedset.insert(current);
        openset.erase(min);
        
        Vertex *v = findVertex(current);

        for(string& neighbor : v->neighborWords())
        {
            // If we have already processed this neighbor, skip it (we don't want to go back to it)
            if(closedset.find(neighbor) != closedset.end()) continue;
            
            int g = g_score[current] + 1;
            int h = hamming(neighbor, target);
            
            if(openset.find(neighbor) == openset.end() || g < g_score[neighbor])
            {
                came_from[neighbor] = current;
                g_score[neighbor] = g;

                if(openset.find(neighbor) == openset.end()) openset[neighbor] = g + h;
            }
        }
        
    }
    
    cout << "No solution." << endl;
}



void Graph::Astar_print_path(map<string, string> came_from, string current)
{
    if(came_from.find(current) != came_from.end())
    {
        Astar_print_path(came_from, came_from[current]);
        cout << current << endl;
    }
    else
        cout << current << endl;
}




int main()
{
    Graph g;
    
    string line;
    
    while(true)
    {
        getline(cin, line);
        if(line.length() == 0) break;
        g.insertWord(line);
    }

    string source, target;
    int count = 0;
    
    while(!cin.eof())
    {        
        getline(cin, line);
        if(line.length() == 0) break;

        if(count == 0) count++;
        else cout << endl;

        source = line.substr(0, line.find(' '));
        target = line.substr(line.find(' ') + 1);

        g.Astar(source, target);
    }
    
    return 0;
}
