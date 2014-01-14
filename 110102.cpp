// Minesweep problem

#include <iostream>
#include <vector>


using namespace std;


const int MINE = -1;


/* =================================================== */

class Field
{
    int id;
    vector<vector<int> > m_field;
    int size_n, size_m; // size_n = number of lines, size_m = number of columns
    
public:
    Field(int i, int n, int m); // constructor
    
    void set_mine(int n, int m); // set mine at line = n and column = m
    void print();
};


Field::Field(int i, int n, int m) : id(i), size_n(n), size_m(m), m_field(n + 2, vector<int>(m + 2, 0))
{
    // I add two lines and two columns, respectively, to handle boundary cases more easily.
    // So the actual grid will start from 1, 1 and will end at n, m.
    // The borders are just the boundary cases, which will not be printed in the output.
}


void Field::set_mine(int n, int m)
{
    for(int i = n - 1; i <= n + 1; i++)
        for(int j = m - 1; j <= m + 1; j++)
            if(i == n && j == m) m_field[i][j] = MINE;
            else if(m_field[i][j] != MINE) m_field[i][j]++;
}


void Field::print()
{
    cout << "Field #" << id << ":" << endl;
    
    for(int i = 1; i <= size_n; i++)
    {
        for(int j = 1; j <= size_m; j++)
            //cout << (m_field[i][j] == MINE ? '*' : m_field[i][j]);
            if(m_field[i][j] == MINE) cout << '*';
            else cout << m_field[i][j];
        
        cout << endl;
    }
}

/* =================================================== */


int main()
{
    int id = 1;
    
    int n, m;
    char c;
    
    while(true)
    {
        cin >> n >> m;
        if(n == 0 && m == 0) break;
        
        Field f(id, n, m);
        
        for(int i = 1; i <= n; i++)
            for(int j = 1; j <= m; j++)
            {
                cin >> c;
                if(c == '*') f.set_mine(i, j);
            }
        
        if(id > 1) cout << endl;
        f.print();
        
        id++;
    }
    
    return 0;
}
