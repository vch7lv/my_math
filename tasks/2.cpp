#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

int main()
{
    int N,M,Q;
    cin >> N >> M >> Q;

    map<string, int> labels;
    vector<vector<int>> table(N, vector<int>(M));
    vector<bool> check(N, true);

    for (int i = 0; i < M; ++i)
    {
        string tmp;
        cin >> tmp;
        labels[tmp] = i;
    }
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
        {
            int tmp;
            cin >> tmp;
            table[i][j] = tmp;
        }
    for (int i = 0; i < Q; ++i)
    {
        string tmp;
        cin >> tmp;

        int ind = labels[tmp];

        cin >> tmp;
        int sign = (tmp[0] == '>' ? 1 : -1);

        cin >> tmp;
        int value = stoi(tmp);

        for(int j = 0; j < N; ++j)
            if ( check[j] && ((sign == 1 && table[j][ind] <= value) || 
                             (sign == -1 && table[j][ind] >= value)) )
                check[j] = false;
    }


    long long sum = 0;
    for (int i = 0; i < N; ++i)
    {
        if (check[i])
            for (int j = 0; j < M; ++j)
                sum += table[i][j];
    }

    cout << sum;
}