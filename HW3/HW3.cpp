#include <bits/stdc++.h>

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
#define INF 1000000

int N;
double ans;
double **dist;
// vector<vector<double> > dp;
vector<vector<double>> dp(1 << 25, vector<double>(25));

void init() {
    for (int i = 0; i < (1 << N); i++) {
        dp[i].clear();
        for (int j = 0; j < N; j++) {
            dp[i].push_back(0);
        }
    }
    for (int i = 0; i < N; ++i)
        dp[0][i] = dist[0][i];
    // ans = INF;
    return;
}

double TSP(int x, int status) {
    if (dp[status][x])
        return dp[status][x];

    dp[status][x] = INF;
    for (int i = 0; i < N - 1; ++i) {
        int mask = (1 << i);
        if (status & mask) {
            if (dp[status][x] > TSP(i + 1, status - mask) + dist[x][i + 1]) {
                dp[status][x] = TSP(i + 1, status - mask) + dist[x][i + 1];
            }
        }
    }
    return dp[status][x];
}

int main() {
    double start = clock();
    int n, j;
    double t1, t2;
    double x[25], y[25];

    string file;
    ifstream in;
    // 讀檔(測資)
    cout << "File name:";
    cin >> file;

    in.open(file);

    char out[20];
    stringstream ss;
    // 輸出檔(最短路徑+順序)
    ss << "ans_dt" << file[5] << ".txt";
    string s;
    ss >> s;
    strcpy(out, s.c_str());
    ofstream os(out);
    n = 0;
    vector<int> id;
    while (in >> j >> t1 >> t2) {
        id.push_back(j);
        x[n] = t1;
        y[n] = t2;
        n++;
        // cout << j << " " << t1 << " " << t2 << endl;
    }
    
    in.close();
    N = n;
    // 算出城市之間的距離
    dist = new double *[N];
    for (int i = 0; i < n; i++) {
        dist[i] = new double[N];
        for (int j = 0; j < n; j++) {
            dist[i][j] =
                sqrt(pow(abs(x[i] - x[j]), 2) + pow(abs(y[i] - y[j]), 2));
            // cout << dist[i][j] << " ";
        }
        // cout << endl;
    }

    init();

    // 最短路徑
    ans = TSP(0, (1 << (N - 1)) - 1);

    cout << "distance: " << ans << endl;
    os << "distance: " << ans << endl;

    int nowPos = 0, nowStatus = (1 << (N - 1)) - 1;

    for (int i = 0; i < N - 1; i++) {
        for (int j = 0; j < N - 1; j++) {
            int mask = (1 << j);
            if (fabs(dp[nowStatus][nowPos] - dp[nowStatus - mask][j + 1] - dist[j + 1][nowPos]) < 1e-5) {
                cout << nowPos + 1 << '\n';
                os << nowPos + 1 << '\n';
                nowPos = j + 1;
                nowStatus = nowStatus - mask;
                break;
            }
        }
    }

    cout << nowPos + 1 << '\n';
    os << nowPos + 1 << '\n';
    delete[] dist;

    
    os.close();
    double end = clock();
    cout << "time: "<< (end - start)/CLOCKS_PER_SEC << endl;

    return 0;
}
