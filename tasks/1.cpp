#include <iostream>
#include <vector>
#include <map>
using namespace std;
typedef long long ll;

int find_len(vector<int>& dpA, vector<int>& dpB, map<int, char>& lang, vector<int>& bosses, int i, char our_lang)
{

    if (our_lang == lang[bosses[i]])
    {
        if (our_lang == 'A') 
            dpA[i] = 0;
        else 
            dpB[i] = 0;
        return 0;
    }

    if (dpB[bosses[i]] != -1)
    {
        if (our_lang == 'A') 
            return dpA[bosses[i]] + 1;
        else   
            return dpB[bosses[i]] + 1;
    }

    if (our_lang == 'A') 
    {
        dpA[i] = find_len(dpA, dpB, lang, bosses, bosses[i], our_lang) + 1;
        return dpA[i];
    }
    else 
    {
        dpB[i] = find_len(dpA, dpB, lang, bosses, bosses[i], our_lang) + 1;
        return dpB[i];
    }
}
int main()
{
    int n;
    cin >> n;

    map<int, char> lang;
    lang[0] = 'D';
    for (int i = 0; i < n; ++i)
    {
        char tmp;
        cin >> tmp;
        lang[i+1] = tmp;
    }

    vector<int> bosses(n + 1, 0);
    vector<int> stack {0}; 
    int last;
    cin >> last;
    do
    {
        cin >> last;
        if (stack.back() == last)
            stack.pop_back();
        else
        {
            bosses[last] = stack.back();
            stack.push_back(last);
        }
    } while (stack.size() != 0);


    vector<int> dpA(n+1, -1), dpB(n+1, -1);

    dpA[0] = 0; dpB[0] = 0;
    for (int i = 1; i <= n; ++i)
        cout << find_len(dpA, dpB, lang, bosses, i, lang[i]) << ' ';
    for (int i = 1; i <= n ; ++i)
        cout << dpA[i] << ' ' << dpB[i] << endl; 
}