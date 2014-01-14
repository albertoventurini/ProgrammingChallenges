// Where's Waldorf?

#include <iostream>
#include <algorithm>
#include <cassert>
#include <sstream>

using namespace std;


// Just a simple struct that I use to return a pair of values from a function
struct Pair
{
    int a, b;
    
    Pair() : a(0), b(0) { }
};



// Class that contains the letter grid
class Grid
{
    // The container is simply an array of chars
    char *m_data;
    
    int m; // m = lines
    int n; // n = columns

    string wordAt(int a, int b, int length, int direction_a, int direction_b) const;
    bool isWordHere(int a, int b, const string& word) const;

public:
    Grid(int m, int n) { this->m = m; this-> n = n; m_data = new char[m*n]; }
    ~Grid() { delete [] m_data; }
    
    void readFromCin();
 
    Pair query(string word) const;
    
    void print()
    {
        for(int a = 0; a < m; a++)
        {
            for(int b = 0; b < n; b++)
                cout << m_data[a * n + b];
            
            cout << endl;
        }
    }
};


// Read grid from stdin
void Grid::readFromCin()
{
    string line;
    
    int counter = 0;
    
    for(int i = 0; i < m; i++)
    {
        getline(cin, line);
        
        // Transform the line to uppercase, as we don't care for the case
        //transform(hello.begin(), hello.end(), hello.begin(), [](char c) { return (c >= 'a' && c <= 'z' ? c + 'A' - 'a' : c); });
        transform(line.begin(), line.end(), line.begin(), ::toupper);
        
        if(line.length() != n)
        {
            cout << "Input error" << endl;
            exit(0);
        }
        
        // Copy the line into our char array
        line.copy(m_data + counter, line.length());
        counter += n;
    }
}


// Return a string that contains the word at the given (a, b) position,
// with the given directions
string Grid::wordAt(int a, int b, int length, int direction_a, int direction_b) const
{
    string result;
    
    int i = a;
    int j = b;
    int counter = 0;
    
    while(i < m && j < n && i >= 0 && j >= 0 && counter < length)
    {
        assert(i*n + j < m*n);
        
        result += m_data[i * n + j];
        
        i += direction_a;
        j += direction_b;
        counter++;
    }
    
    return result;
}


// Return true if the word is at position (a, b) in the grid
// a = line, b = column
bool Grid::isWordHere(int a, int b, const string& word) const
{
    // Try all directions
    for(int direction_a = -1; direction_a <= 1; direction_a++)
    for(int direction_b = -1; direction_b <= 1; direction_b++)
    {
        // Skip 0, 0 direction
        if(direction_a == 0 && direction_b == 0) continue;
        
        // If we found the word, we can stop here and return true
        if(wordAt(a, b, word.length(), direction_a, direction_b) == word)
            return true;
    }
    
    // If we reach here, we didn't find the word
    return false;
}


// The reason I pass "word" by value here is that 
// I want to modify it (to uppercase)
Pair Grid::query(string word) const
{
    Pair p;
    
    // To uppercase
    transform(word.begin(), word.end(), word.begin(), ::toupper);
    
    // Scan the whole grid searching for the word
    for(int a = 0; a < m; a++)
    for(int b = 0; b < n; b++)
    {
        if(isWordHere(a, b, word))
        {
            // Return a+1 and b+1 because we want coordinates to show as starting from 1, not 0
            p.a = a + 1;
            p.b = b + 1;
            return p;
        }
    }
    
    // We should never reach this point, because we are ensured the input is always correct,
    // but if we do, just return p
    return p;
}


int main()
{   
    // Get number of test cases
    int n_cases;
    cin >> n_cases;
    
    for(int i = 0; i < n_cases; i++)
    {
        // The output of two consecutive cases must be separated by a blank line.
        if(i >= 1) cout << endl;
        
        // Get dimensions of grid
        int m, n;
        cin >> m >> n;
        
        // Flush the stdin till the newline
        string line;
        getline(cin, line);
        
        // Create a new Grid object from stdin
        Grid g(m, n);
        g.readFromCin();
        
        // Process queries
        int n_queries;
        cin >> n_queries;
                
        for(int j = 0; j < n_queries; j++)
        {
            string word;
            cin >> word;
            
            Pair p = g.query(word);
            cout << p.a << " " << p.b << endl;
        }
    }
    
    return 0;
}
