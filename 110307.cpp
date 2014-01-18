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
        for(list<Vertex *>::iterator it = neighbors.begin(); it != neighbors.end(); it++)
            result.push_back( (*it)->word );
        
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
    map<string, Vertex *>::const_iterator it = words.find(word);
    
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
    
    Vertex *v = newVertex(word, false);
    
    vector<string> sentinels = calcSentinels(word);
    
    for(vector<string>::iterator it = sentinels.begin(); it != sentinels.end(); it++)
    {
        Vertex *sent = findVertex(*it);
        if(sent == NULL) sent = newVertex(*it, true);
        
        for(list<Vertex *>::iterator n = sent->neighbors.begin(); n != sent->neighbors.end(); n++)
        {
            v->neighbors.push_back(*n);
            (*n)->neighbors.push_back(v);
        }
        
        sent->neighbors.push_back(v);
    }
    
}


/*
void Graph::query(const string& source, const string& target)
{   
    if(findVertex(source) == NULL || findVertex(target) == NULL)
    {
        cout << "No solution." << endl;
        return;
    }
    
    if(source == target)
    {
        cout << source << endl;
        return;
    }
 
    list<string> solution;
    solution.push_back(source);
    
    string current(source);
    
    int oldMin = INT_MAX;
    Vertex *v;
    
    set<string> visited;
    visited.insert(current);
    
    while(true)
    {
        cout << "===> " << current << endl;
        
        v = findVertex(current);
        
        list<string> nWords = v->neighborWords();
        
        int min = INT_MAX;
        string minString;
        
        for(list<string>::iterator it = nWords.begin(); it != nWords.end(); it++)
        {
            if( visited.find(*it) == visited.end() )
            {
                int h = hamming(target, *it);
                if(h < min)
                {
                    min = h;
                    minString = *it;
                }
            }
        }
        
        if(min == INT_MAX)
        {
            cout << "No solution." << endl;
            return;
        }
        
        //if(oldMin != INT_MAX && oldMin <= min)
        //{
        //    cout << "No solution." << endl;
        //    return;
        //}
        
        if( find(solution.begin(), solution.end(), minString) != solution.end() )
        {
            cout << "No solution." << endl;
            return;            
        }
        
        oldMin = min;

        current = minString;
        solution.push_back(current);
        visited.insert(current);
        
        if(minString == target) break;
    }
    
    for(list<string>::iterator it = solution.begin(); it != solution.end(); it++)
        cout << *it << endl;  
}

*/


/*
bool Graph::queryRecursive(const string& source, const string& target, list<string>& solution, set<string>& visited)
{
    if(source == target) return true;
    
    Vertex *v = findVertex(source);
    
    list<string> nWords = v->neighborWords();
    list<string> sortedWords;
    
    while(nWords.size() != 0)
    {
        int min = INT_MAX;
        list<string>::iterator minString = nWords.begin();
        
        for(list<string>::iterator it = nWords.begin(); it != nWords.end(); it++)
        {           
            int h = hamming(target, *it);
            if(h < min)
            {
                min = h;
                minString = it;
            }
        }
        
        sortedWords.push_back(*minString);
        nWords.erase(minString);
    }

    while(sortedWords.size() != 0)
    {
        string next = sortedWords.front();
        sortedWords.pop_front();
        
        if(visited.find(next) == visited.end())
        {   
            visited.insert(next);
                 
            if(queryRecursive(next, target, solution, visited))
            {
                solution.push_front(next);
                return true;
            }
        }
    }
    
    return false;
}



void Graph::query(const string& source, const string& target)
{
    if(source == target)
    {
        cout << source << endl;
        return;
    }
    
    if(source.length() != target.length())
    {
        cout << "No solution." << endl;
        return;
    }
    
    if(findVertex(source) == NULL || findVertex(target) == NULL)
    {
        cout << "No solution." << endl;
        return;
    }
    
    list<string> solution;
    set<string> visited;
    visited.insert(source);
    
    if(queryRecursive(source, target, solution, visited))
    {
        solution.push_front(source);
        
        for(list<string>::iterator it = solution.begin(); it != solution.end(); it++)
            cout << *it << endl;
    }
    else
        cout << "No solution." << endl;
 
}

*/


struct CompareSecond
{
    bool operator()(const pair<string, int>& left, const pair<string, int>& right) const
    {
        return left.second < right.second;
    }
};



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
    
    set<string> closedset;
    
    map<string, string> came_from;
    
    map<string, int> g_score;
    g_score.insert(pair<string, int>(source, 0));
    
    map<string, int> f_score;
    f_score.insert(pair<string, int>(source, g_score[source] + hamming(source, target)));

    map<string, int> openset;
    openset.insert(pair<string, int>(source, f_score[source]));
    
    while(!openset.empty())
    {
        map<string, int>::iterator min = min_element(openset.begin(), openset.end(), CompareSecond());
        string current = min->first;
        
        //cout << "===> " << current << " " << openset[current] << endl;
        
        if(current == target)
        {
            Astar_print_path(came_from, current);
            return;
        }
        
        closedset.insert(current);
        openset.erase(min);
        
        Vertex *v = findVertex(current);
        list<string> nWords = v->neighborWords();
        
        for(list<string>::iterator it = nWords.begin(); it != nWords.end(); it++)
        {
            if(closedset.find(*it) != closedset.end()) continue;
            
            int g = g_score[current] + 1;
            int h = hamming(*it, target);
            
            if(openset.find(*it) == openset.end() || g < g_score[*it])
            {
                came_from[*it] = current;
                g_score[*it] = g;
                
                //if(openset[*it] == openset.end()) openset.insert(pair<string, int>(*it, g + h));
                if(openset.find(*it) == openset.end()) openset[*it] = g + h;
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
    
    //g.Astar("whiner", "header");
    
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
        
        //g.query(source, target);
        g.Astar(source, target);
    }
    
    
    /*
    
    Vertex *v = g.findVertex("blabla");
    
    g.insertWord("Boaster");
    g.insertWord("Coaster");
    g.insertWord("Roaster");
    g.insertWord("Roasted");
    g.insertWord("Booster");
    g.insertWord("Coasted");
    g.insertWord("Roosted");
    g.insertWord("Raastid");
    
    g.query("Booster", "Roasted");*/
    
    return 0;
}
