#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;


bool is_jolly_jumper(vector<int> v)
{
    vector<bool> j(v.size(), false);
    
    for(int i = 0; i < v.size() - 1; i++)
    {
        int idx = abs(v[i] - v[i+1]);
        j[idx] = true;
    }
    
    for(int i = 1; i < j.size(); i++)
        if(!j[i]) return false;
    
    return true;
    
}


int main()
{
    int n;
    
    while(cin >> n)
    {
        vector<int> v(n);
        for(int i = 0; i < n; i++) cin >> v[i];
        
        if(is_jolly_jumper(v)) cout << "Jolly"; else cout << "Not jolly";
        cout << endl;
    }
    
    return 0;
}
