// The 3n+1 problem

#include <iostream>
#include <algorithm>

using namespace std;


// Given a cycle starting with i,
// calculate the cycle length
unsigned long cycle_length(int i)
{
    if(i == 1) // base of recursion
        return 1;
    
    else if(i % 2 == 0) // even number
        return 1 + cycle_length(i >> 1);
    
    else  // odd number
        return 1 + cycle_length(i*3 + 1);
}


// Given two integers a and b,
// calculate the length of the longest cycle among all cycles between a and b
unsigned long maximum_cycle_length(int a, int b)
{
    unsigned long max = 0;
    unsigned long cl;
    
    for(int i = a; i <= b; i++)
    {
        cl = cycle_length(i); 
        if(cl > max) max = cl;
    }
    
    return max;
}


int main()
{
    int a, b;
    
    while(cin >> a >> b)
        cout << a << " " << b << " " << maximum_cycle_length(min(a, b), max(a, b)) << endl;

    
    return 0;
}
