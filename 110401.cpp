// 110401 - Vito's family
//
// This is very simple - just take the median of all street numbers and calculate the sum of distances from it.
//

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


int main()
{
    int n_test_cases;
    cin >> n_test_cases;
    
    for(int i = 0; i < n_test_cases; i++)
    {
        int n_numbers;
        cin >> n_numbers;
        
        vector<int> v;
        
        for(int j = 0; j < n_numbers; j++)
        {
            int temp;
            cin >> temp;
            v.push_back(temp);
        }
        
        int min_distance = n_numbers / 2;
        nth_element(v.begin(), v.begin() + min_distance, v.end());
        
        int distance = 0;
        
        for(int j = 0; j < n_numbers; j++)
            distance += abs(v[j] - v[min_distance]);
        
        cout << distance << endl;
    }
    
    return 0;
}
