#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <fstream>
#include <bits/stdc++.h>
using namespace std;

const double esp = 1e-6;

int main(){
	int n, i, j, t1, t2;
	vector<float> x, y;	
	char file[20];
	ifstream in;
	cout << "file name:";
	cin >> file;
	
	in.open(file);
	
	char out[20];
	stringstream ss;
	ss << "ans_dt"<< file[5] << ".txt";
	string s;
	ss >> s; 
	strcpy(out, s.c_str());
	ofstream os(out);
	
	vector<int> v;
	
	while(in >> j >> t1 >> t2){
		v.push_back(j-1);
		x.push_back(t1);
		y.push_back(t2);
		n++;
		//cout << j << " " << t1 << " " << t2 << endl; 
	}
	
	int city[n];
	int ans[n];
	float dis[n][n];
	float sum = 0;
	float temp = 0;
	
	for(i = 0;i < n;i++){
		for(j = 0;j < n;j++){
			dis[i][j] = sqrt(pow(abs(x[i] - x[j]), 2) + pow(abs(y[i] - y[j]), 2));
			//cout << dis[i][j] << " ";
		}
		//cout << endl;
	}
	
	for(i = 0;i < n-1;i++){
		sum += dis[v[i]][v[i+1]];
		//cout << v[i] << " " << v[i+1]  << " " << dis[v[i]][v[i+1]]<< endl;
	}
	//cout << v[i] << " " << 0  << " " << dis[v[i]][0]<< endl;
	sum += dis[v[i]][0];
	//cout << sum << endl;

	while(next_permutation(v.begin(), v.end())){
		temp = 0;
		
		for(i = 0;i < n-1;i++){
			temp += dis[v[i]][v[i+1]];
		}
		temp += dis[v[i]][v[0]];
		
		if(temp < sum && fabs(sum - temp) > esp){
			sum = temp;
			for(i = 0;i < n;i++){
				ans[i] = v[i];
			}
		}
	
	}
	cout << "distance: " << sum << endl;
	os << "distance: " << sum << endl;

	for(j = 0;j < n;j++){
		cout << ans[j]+1 << endl;
		os << ans[j]+1 << endl;
	}
	
	in.close();
	os.close();	
	
	
	return 0;
}
