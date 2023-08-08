#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <string>
#include <bits/stdc++.h> 
using namespace std;

const double esp = 1e-6;
int n, i, j, k, l;
float t1, t2;
vector<float> x, y;

int main(){
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
	n = 0;
	while(in >> j >> t1 >> t2){
		x.push_back(t1);
		y.push_back(t2);
		n++;
		//cout << j << " " << t1 << " " << t2 << endl; 
	}
	

	int city[n];
	float dis[n][n];
	//int x[n], y[n];
	float sum = 0;
	float temp;
	int flag;
	
	
	
	for(i = 0;i < n;i++){
		for(j = 0;j < n;j++){
			dis[i][j] = sqrt(pow(abs(x[i] - x[j]), 2) + pow(abs(y[i] - y[j]), 2));
			//cout << dis[i][j] << " ";
		}
		//cout << endl;
	}
	
	i = 1;
	city[0] = 0;

	while(i < n){
		k = 1;
		temp = 1000000;
		while(k < n){
			l = 0;
			flag = 0;
			while(l < i){
				if(city[l] == k){
					flag = 1;
					break;
				}else{
					l++;
				}
			}
			if(flag == 0 && dis[city[i - 1]][k] < temp && fabs(dis[city[i - 1]][k] - temp) > esp){
				j = k;
				temp = dis[city[i - 1]][k];
			}
			k++;
		}
		city[i] = j;
		i++;
		sum += temp;
	}

	sum += dis[j][0];

	cout << "distance: " << sum << endl;
	os << "distance: " << sum << endl;

	for(j = 0;j < n;j++){
		cout << city[j]+1 << endl;
		os << city[j]+1 << endl;
	}
	
	in.close();
	os.close();	

	return 0;
}
