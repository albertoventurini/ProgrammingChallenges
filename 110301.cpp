#include <iostream>

using namespace std;


char table[255];


void build_table(const string& s)
{
    //char shift[] = "QWERTYUIOP[";
    
    for(int i = 0; i < s.size() - 1; i++)
        table[s[i+1]] = s[i];
}


void translate(const string& s)
{
    for(int i = 0; i < s.size(); i++)
        cout << table[s[i]];
    
    cout << endl;
}


int main()
{
    for(int c = 0; c <= 255; c++) table[c] = (char)c;

    
    build_table("\tQWERTYUIOP[]\\");
    build_table("ASDFGHJKL;'");
    build_table("ZXCVBNM,./");
    build_table("1234567890-=");
    
    string line;
    while(getline(cin, line)) translate(line);
    
    return 0;
}
