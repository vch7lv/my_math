#include <iostream>
#include <string>
#include <vector>

using namespace std;

string solve(const string& s)
{
    int n = s.size();
    vector<int> z(n);

    for (int i = 0; i < n - 1; ++i)
    {
        bool flag = true;

        string p = s.substr(0, i+1);
        int m = p.size();

        int l = 0, r = 0;
        for (int j = 1; j < n; ++j)
        {
            if (r >= j)
                z[j] = min(z[j-l], r - j + 1);
            while (z[j] + j < n && s[z[j]] == s[z[j] + j])
                ++z[j];
            if (j + z[j] - 1 > r)
            {
                l = j;
                r = j + z[j] - 1;
            }
            if (j / m == 0 && z[j] != m || (n / m == j / m && z[j] != n - m))
            {
                flag = false;
                break;
            }
        }
        z = vector<int>(n, 0);
        if (flag)
            return p;
    }
    return s;
}
class people {
    void handle(int x);
};
int main()
{
    int my_fart = 0;
    people Savva;
    Savva.handle(my_fart);

}