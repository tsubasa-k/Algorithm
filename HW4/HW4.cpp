// main.cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#include <ctime>
#include <stdlib.h>
using namespace std;

int Count = 51;
const int CityCount = 51, AntCount = 70;  //城市數量和螞蟻數量
int besttour[CityCount];  //最佳路徑
const double a = 1, b = 5, P = 0.1, Q = 100;
double  random(int low, double uper)  //生成某區間內隨機數的函式
{
	return (rand() / (double)RAND_MAX)*(uper - low) + low;
};
int random(int uper)   //返回一個從0到最大隨機數的任意整數
{
	return (rand() % uper);
};
class City
{
public:
	string name;
	double x, y;   //每個城市的x、y坐標
	
	void shuchu()
	{
		std::cout << name << " " << x << " " << y << endl;
	}
};
class Graph   
{
public:
	City city[CityCount];
	double Distance[CityCount][CityCount];  //城市之間的距離
	double t[CityCount][CityCount];   //費洛蒙
	double Deltt[CityCount][CityCount];  //費洛蒙變化量
	void shuchu()
	{
		cout << "城市名稱 " << "坐標x" << " " << "坐標y" << endl;
		for (int i = 0; i < Count; i++)
			city[i].shuchu();
		cout << "距離矩陣： "<< endl;
		for (int i = 0; i < Count; i++)
		{
			for (int j = 0; j < Count; j++)
			{
				if (j == Count - 1)
					std::cout << Distance[i][j] << endl;
				else
					std::cout << Distance[i][j] << "  ";
			}
		}
		cout << "費洛蒙： " << endl;
		for (int i = 0; i < Count; i++)
		{
			for (int j = 0; j < Count; j++)
			{
				if (j == Count - 1)
					std::cout << t[i][j] << endl;
				else
					std::cout << t[i][j] << "  ";
			}
		}
		cout << "費洛蒙增量矩陣： " << endl;
		for (int i = 0; i < Count; i++)
		{
			for (int j = 0; j < Count; j++)
			{
				if (j == Count - 1)
					std::cout << Deltt[i][j] << endl;
				else
					std::cout << Deltt[i][j] << "  ";
			}
		}
	}
	void Readcoordinatetxt(string txtfilename);    //讀取城市坐標文件的函式
};
Graph Map_City;    
void Graph::Readcoordinatetxt(string txtfilename)
{
	ifstream myfile(txtfilename, ios::in);
	double x = 0, y = 0;
	int z = 0;
	if (!myfile.fail())
	{
		int i = 0;
		vector<int> t1, t2;
		vector<string> t3;
		while (!myfile.eof() && (myfile >> z >> x >> y))
		{
			t1.push_back(x);
			t2.push_back(y);
			t3.push_back(to_string(z));
		}
		
		Count = t1.size();
		for(int i = 0;i < t1.size();i++)
		{
			city[i].name = t3[i]; //稱轉成字串
			city[i].x = t1[i]; city[i].y = t2[i];
		
		}
	}
	else
		cout << "文件不存在";
	myfile.close();      //計算城市距離
	for (int i = 0; i < Count; i++)
		for (int j = 0; j < Count; j++)
		{
			Distance[i][j] = sqrt(pow((city[i].x - city[j].x), 2) + pow((city[i].y - city[j].y), 2));  //計算城市ij之間的距離
			t[i][j] = 1;
			Deltt[i][j] = 0;
		}
}
class Ant
{
public:
	int tour[CityCount + 1];   //Ant所建構的路徑
	bool can_visit[CityCount];   //Ant是否能visit城市
	int tour_citycount;   //Ant當前走過城市的數量
	double tour_length, tour_length_shortest;   //Ant所走的路徑長度tour_length
	double select_probability[CityCount];   //Ant各城市的選擇路徑機率
	Ant();    //Ant的初始化
	void Build_Trip();    //Ant建構旅行路徑
	void Next_City();     //Ant選擇下一個城市
	void GetFirstCity();  //Ant隨機visit到第一個城市
	void Addcity(int city);   //在路徑上添加城市
	void UpdateTourLength();  //更新旅行路徑長度
	void Clear();   
};
Ant ant[AntCount];  //蟻群
Ant::Ant()   
{
	tour_length = tour_length_shortest = 0;
	tour_citycount = 0;       //起初所旅行過的城市數量為0
	for (int i = 0; i<Count; i++)
	{
		can_visit[i] = true;     //每個城市都可以visit
		select_probability[i] = 0;   //選擇各個城市的概率為0
	}
}
void Ant::Clear()
{
	tour_length = 0;
	for (int i = 0; i<Count; i++)
	{
		select_probability[i] = 0;
		can_visit[i] = true;
	}
	tour_citycount = 0;
}
void Ant::Addcity(int city)
{
	//add city to tabu;
	tour[tour_citycount] = city;
	tour_citycount++;
	can_visit[city] = false;
}
void Ant::GetFirstCity()
{
	srand((unsigned)time(NULL) + rand());
	Addcity(random(Count));
}
void Ant::Next_City()
{
	int currentcity = tour[tour_citycount - 1];   //計算下一步先找到上一步所到的城市
	double temp = 0, sum_probability = 0;
	for (int i = 0; i < Count; i++)
	{
		if (can_visit[i] == true)
			temp += pow((Map_City.t[currentcity][i]), a)*pow(1.0 / Map_City.Distance[currentcity][i], b);
	}
	for (int i = 0; i < Count; i++)
	{
		if (can_visit[i] == false)
			select_probability[i] = 0;    //已經到過的城市選擇概率為0
		else
		{
			select_probability[i] = pow((Map_City.t[currentcity][i]), a)*pow(1.0 / Map_City.Distance[currentcity][i], b) / temp;  //對於未到過的城市，則計算選擇概率
			sum_probability += select_probability[i];
		}
	}
	double r = random(0, sum_probability);  //取概率區間的隨基浮點數
	double p = 0;
	int nextcity = -1;
	for (int j = 0; j < Count; j++)
	{
		if (can_visit[j] == true) p += select_probability[j];
		if (p >= r)
		{
			nextcity = j; break;
		}
	}
	/*if (nextcity == -1)
	{
		temp = -1;
		for (int i = 0; i<CityCount; i++)
		{
			if (can_visit[i] == true)
				if (temp<pow((Map_City.τ[currentcity][i]), α)*pow(1.0 / Map_City.Distance[currentcity][i], β))
				{
					temp = pow((Map_City.τ[currentcity][i]), α)*pow(1.0 / Map_City.Distance[currentcity][i], β);
					nextcity = i;
				}
		}
	}*/
	Addcity(nextcity);
}
void Ant::Build_Trip()
{
	for (int i = 0; i < Count; i++)
	{
		can_visit[i] = true;
	}
	GetFirstCity();
	while (tour_citycount < Count)
	{
		if(tour_citycount < Count) Next_City();   //輪盤賭法選擇下一個城市
		else if (tour_citycount == Count - 1)
		{
			for (int i = 0; i<Count; i++)
				if (can_visit[i] == true)
				{
					Addcity(i);
					break;
				}  //如果還剩下一個城市可以visit，那么這個城市就是最后一個城市，添加到Ant旅行的城市上
		}
	}
	tour[Count] = tour[0];  //Ant旅行的最后一個城市
}
void Ant::UpdateTourLength()  //更新旅行路徑長度的函數
{
	for (int i = 0; i<Count - 1; i++)
		tour_length += Map_City.Distance[tour[i]][tour[i + 1]];
	tour_length += Map_City.Distance[tour[Count - 1]][tour[0]];
}
void UpdatePheromones(Graph Map_City,Ant ant[AntCount])  //更新費洛蒙的函數
{
	for (int i = 0; i<AntCount; i++)
	{
		for (int j = 0; j<Count - 1; j++)
		{
			Map_City.Deltt[ant[i].tour[j]][ant[i].tour[j + 1]] += Q / ant[i].tour_length;
			Map_City.Deltt[ant[i].tour[j + 1]][ant[i].tour[j]] += Q / ant[i].tour_length;
		}
		Map_City.Deltt[ant[i].tour[Count - 1]][ant[i].tour[0]] += Q / ant[i].tour_length;
		Map_City.Deltt[ant[i].tour[0]][ant[i].tour[Count - 1]] += Q / ant[i].tour_length;
	}
	for (int i = 0; i<CityCount; i++)
	{
		for (int j = 0; j<Count; j++)
		{
			Map_City.t[i][j] = (1 - P)*Map_City.t[i][j] + Map_City.Deltt[i][j];
			Map_City.Deltt[i][j] = 0;
		}
	}
}
int main()
{
	int MAX_GEN = 1000;
	double global_tourlength = 10e9,Ljia = MAX_GEN;
	int Tjia[CityCount];   //表示蟻群優化演算法最後發現的最短路徑
	//首先對城市地圖進行初始化
	char file[20];
    	ifstream in;
    	// 讀檔(測資)
    	cout << "file name:";
    	cin >> file;
    	//in.open(file);
    	Map_City.Readcoordinatetxt(file);
    
    	char out[20];
    	stringstream ss;
    	// 輸出檔(最短路徑+順序)
    	ss << "ans_dt"<< file[5] << ".txt";
    	string s;
    	ss >> s; 
    	strcpy(out, s.c_str());
    	ofstream os(out);
    	
	int run = 30;
	Map_City.shuchu();
	int temptour[CityCount];
	double run_ans[run];
	for(int k = 1;k <= run;k++){
		std::cout << "第 "<< k <<" run：" << endl;
		for (int iteratorcishu = 0; iteratorcishu < MAX_GEN; iteratorcishu++)
		{
			for (int k = 0; k < AntCount; k++)
			{
				ant[k].Build_Trip();
				ant[k].UpdateTourLength();  //計算螞蟻k所發現路徑Tk的長度Lk
				if (ant[k].tour_length < Ljia)
				{
					for (int h = 0; h < Count; h++)
						Tjia[h] = ant[k].tour[h];
					Ljia = ant[k].tour_length;
				}
			}
			//找到最短的路徑長度，放到temp裡
			double temp = ant[0].tour_length;
			for (int i = 0; i<Count; i++) temptour[i] = ant[0].tour[i];
			for (int i = 0; i<AntCount; i++)
			{
				if (temp>ant[i].tour_length) 
				{
					temp = ant[i].tour_length;
					for (int j = 0; j< Count; j++)
						temptour[j] = ant[i].tour[j];
				}
			}
			if (temp<global_tourlength) 
			{
				global_tourlength = temp;
				for (int j = 0; j< Count; j++)
					besttour[j] = temptour[j];
			}
			std::cout << "第" << iteratorcishu << "次迭代最短距離：" << global_tourlength << endl;
			UpdatePheromones(Map_City,ant);
			for (int i = 0; i < AntCount; i++) ant[i].Clear();
		}
		run_ans[k-1] = global_tourlength;
	}
	double final_avg_length = 0;
	double temp = 0;
	for(int k = 0;k < run;k++){
		temp += run_ans[k];
	}
	final_avg_length = temp/run;
	std::cout << "The final of average 最短路徑長度為：" << final_avg_length << endl;
	os << "distance: " << final_avg_length << endl;
	
	std::cout << "最短路徑為：" << endl;
	for (int i = 0; i < Count; i++)
	{
		std::cout << Map_City.city[besttour[i]].name << endl;
		os << Map_City.city[besttour[i]].name << endl;
	}
	
	//system("Pause");
	return 0;
}
