#include <iostream>
#include <fstream>
#include<vector>
#include<queue>
#include<iomanip>

using namespace std;
//在张迁瑞同学的代码基础之上完成，用于生成必要的json文件


const double inf = 10000000.00; //用一个很大的数字来代表无穷大
								//三个基础算法
								//最短路径函数，使用Dijkstra算法
double Dijkstra(int src, int target, int n, const vector<vector<double>>&graph)
{
	//起点与终点相同
	if (src == target)
	{
		return 0;
	}
	vector<double>distance(n); //距离
	vector<bool>visited(n); //是否访问过
	vector<vector<int>> path(n); //路径
								 //初始化
	for (int i = 0; i < n; i++)
	{
		distance[i] = inf;
		visited[i] = false;
	}
	for (int i = 0; i < n; i++)
	{
		path[i] = vector<int>(n);
		for (int j = 0; j < n; j++)
		{
			path[i][j] = -1;
		}
	}
	int current = -1;
	distance[src] = 0;

	int times = 0;
	while (times < n)
	{
		times++;
		//找到距离最小的点current
		current = -1;
		for (int i = 0; i < n; i++)
		{
			if (visited[i] == true)
				continue;
			if (current == -1 || distance[i] < distance[current])
			{
				current = i;
			}
		}
		if (current == -1)
			return 0;
		visited[current] = true;

		if (current != target)
		{
			for (int i = 0; i < n; i++)
			{
				//t是current到某点的距离
				double t = distance[current] + graph[current][i];
				if (t < distance[i])
				{
					distance[i] = t;
					int k = 0;
					//记录路径
					while (path[current][k] != -1)
					{
						path[i][k] = path[current][k];
						k++;
					}
					path[i][k] = current;
				}
			}
		}
	}
	//输出结果部分（可视化时可删去）
	if (distance[target] != inf)
	{
		cout << "path:";
		int k;
		for (k = 0; path[target][k] != -1; k++)
		{
			cout << path[target][k] << "-";
		}
		cout << target << endl;
		return distance[target];
	}
	else
	{
		cout << "无";
		return inf;
	}
}
//连通分量,使用了广度优先搜索的方法
int ConnectedComponent(int n, const vector<vector<double>>& graph, double limit, vector<int>&start, vector<int>&end)
{
	int count = 0;
	vector<bool> visited(n);
	//用于暂存的向量和队列
	queue<int> temp_q;
	vector<int> temp_v;
	for (int i = 0; i < n; i++)
	{
		visited[i] = false;
	}
	for (int i = 0; i < n; i++)
	{
		if (visited[i] == true)
			continue;
		visited[i] = true;
		temp_q.push(i);
		while (!temp_q.empty())
		{
			int t;
			t = temp_q.front();
			for (int j = t + 1; j < n; j++)
			{
				if (visited[j] == false && graph[t][j] != inf && graph[t][j] >= limit)
				{
					start[count] = t;
					end[count] = j;
					count++;
					temp_q.push(j);
					visited[j] = true;
				}
			}
			temp_q.pop();
		}
	}
	return count;
}

//最小支撑树，使用了prim算法,返回最小生成树中边的个数
int spanningTree(int n, const vector<vector<double>> & graph, vector<int>&start, vector<int>&end)
{
	vector<bool> visited(n);
	vector<double> distance(n);
	vector<int> prior(n); //储存前驱
	for (int i = 0; i < n; i++)
	{
		visited[i] = false;
		distance[i] = inf;
		prior[i] = -1;
	}
	//current用于指示当前选择点
	int current = -1;
	int times = 0;
	int choosenEdgeNum = 0;
	while (times < n)
	{
		times++;
		current = -1;
		for (int i = 0; i < n; i++)
		{
			if (visited[i] == true)
				continue;
			if (current == -1 || distance[i] < distance[current])
				current = i;
		}
		if (current == -1)
			break;
		visited[current] = true;
		//将对应的边加入到两个结果向量中
		if (prior[current] != -1)
		{
			start[choosenEdgeNum] = prior[current];
			end[choosenEdgeNum] = current;
			choosenEdgeNum++;
		}
		//更新距离
		for (int i = 0; i < n; i++)
		{
			if (visited[i] == true)
				continue;
			double t = graph[current][i];
			if (t < distance[i])
			{
				distance[i] = t;
				prior[i] = current;
			}
		}
	}
	return choosenEdgeNum;
}

//一个拓展算法：Kernighan-Lin算法，n是要分区的图的规模，将这个图分成两个社区
void kernighanLin(int n, const vector<vector<double>> & graph)
{
	//增益函数Q,大小等于两个社团内部的边权值减去两个社团之间的边权值
	double Q = 0;
	//是否已经交换过
	vector<bool> visited(n);
	vector<int> sign(n); //用1和2代表点的类型
	for (int i = 0; i < n; i++)
	{
		visited[i] = false;
	}
	//初始时将0-n/2这些点视为一个社区
	for (int i = 0; i < n / 2; i++)
	{
		sign[i] = 1;
	}
	for (int i = n / 2; i < n; i++)
	{
		sign[i] = 2;
	}
	vector<int> sign1(n / 2 + 1);
	vector<int> sign2(n / 2 + 1);
	int num1 = 0;
	int num2 = 0;
	for (int i = 0; i < n; i++)
	{
		if (sign[i] == 1)
		{
			sign1[num1] = i;
			num1++;
		}
		else
		{
			sign2[num2] = i;
			num2++;
		}
	}
	//初始更新Q
	for (int i = 0; i < n; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			if (graph[i][j] != inf)
			{
				if (sign[i] == sign[j])
				{
					Q = Q + graph[i][j];
				}
				else
				{
					Q = Q - graph[i][j];
				}
			}
		}
	}
	//开始进行交换
	//进行交换的两个结点
	int change1;
	int change2;
	int flag;
	double tempQ;
	tempQ = Q;
	while (1)
	{
		flag = 0;
		for (int i = 0; i < sign1.size(); i++)
		{
			for (int j = 0; j < sign2.size(); j++)
			{
				if (visited[i] == false && visited[j] == false)
				{
					//交换并计算Q值
					sign[sign1[i]] = 2;
					sign[sign2[j]] = 1;
					double t = 0;
					for (int k = 0; k < n; k++)
					{
						for (int m = k + 1; m < n; m++)
						{
							if (graph[k][m] != inf)
							{
								if (sign[k] == sign[m])
								{
									t = t + graph[k][m];
								}
								else
								{
									t = t - graph[k][m];
								}
							}
						}
					}
					if (t > tempQ)
					{
						change1 = sign1[i];
						change2 = sign2[j];
						tempQ = t;
						flag = 1;
					}
					sign[sign1[i]] = 1;
					sign[sign2[j]] = 2;
					t = 0;
				}
			}
		}
		if (flag == 0)
		{
			break;
		}
		else if (flag == 1)
		{
			visited[change1] = true;
			visited[change2] = true;
			sign[change1] = 2;
			sign[change2] = 1;
			for (int i = 0; i < sign1.size(); i++)
			{
				if (sign1[i] == change1)
					sign1[i] = change2;
			}
			for (int i = 0; i < sign2.size(); i++)
			{
				if (sign2[i] == change2)
					sign2[i] = change1;
			}
			Q = tempQ;
		}

	}
	for (int i = 0; i < sign1.size() - 2; i++)
	{
		cout << sign1[i] << '-';
	}
	cout << sign1[sign1.size() - 2];
	cout << endl;
	for (int i = 0; i < sign2.size() - 2; i++)
	{
		cout << sign2[i] << '-';
	}
	cout << sign2[sign2.size() - 2];
}

int main()
{
	//读取图，储存在向量中
	ifstream inFile;
	ofstream outFile3;
	ofstream outFile4;
	outFile4.open("link.json");
	outFile3.open("trees.json");
	inFile.open("1.movie.txt", ios::in);
	if (!inFile.good())
		cout << "文件读取错误";
	int nodeNum;
	int edgeNum;
	inFile >> nodeNum;
	inFile >> edgeNum;
	vector<vector<double>> graph(nodeNum);
	for (int i = 0; i < nodeNum; i++)
		graph[i] = vector<double>(nodeNum);
	//对图的初始化
	for (int i = 0; i < nodeNum; i++)
	{
		for (int j = 0; j < nodeNum; j++)
		{
			if (i == j)
				graph[i][j] = 0;
			else
				graph[i][j] = inf;
		}
	}
	//读取输入的图的数据
	int i, j;
	double t;
	int count = edgeNum;
	while (!inFile.eof())
	{
		inFile >> i;
		inFile >> j;
		inFile >> t;
		graph[i][j] = t;
		graph[j][i] = t;
		//编辑json文件
	}
	//Dijkstra算法
	double result_Dijkstra;
	int node1, node2;
	cout << "输入两个节点号码，最大值：" << nodeNum - 1 << endl;
	cin >> node1 >> node2;
	result_Dijkstra = Dijkstra(node1, node2, nodeNum, graph);
	cout << "weight:" << result_Dijkstra;

	//最小生成树算法,得到的结果是边的集合
	//始边和终边的向量
	vector<int> start(nodeNum);
	vector<int> end(nodeNum);
	int choosenEdgeNum = spanningTree(nodeNum, graph, start, end);
	//输出部分，可视化时可以删除，choosenEdgeNum代表最小生成树中边的条数
	cout << choosenEdgeNum;
	outFile3 << "{\"edges\":[";
	for (int i = 0; i < choosenEdgeNum; i++)
	{
			cout << start[i] << ' ' << end[i] << endl;
			outFile3 << "{\"source\":" << start[i] << ",\"target\":" << end[i] << ",\"w\":" << graph[start[i]][end[i]] << '}';
			if (i < choosenEdgeNum - 1)
				outFile3 << ",";
	}
	outFile3 << "]}";
	outFile3.close();
	//求连通分量
	double limit;
	cout << "输入连通分量的边阈值";
	cin >> limit;

	vector<int> start1(nodeNum);
	vector<int> end1(nodeNum);
	int counter = ConnectedComponent(nodeNum, graph, limit, start1, end1);
	outFile4 << "{\"edges\":[";
	for (int i = 0; i < counter; i++)
	{
		cout << start1[i] << ' ' << end1[i] << endl;
		outFile4 << "{\"source\":" << start1[i] << ",\"target\":" << end1[i] << ",\"w\":" << graph[start1[i]][end1[i]] << '}';
		if (i < counter - 1)
			outFile4 << ",";
	}
	outFile4 << "]}";

	outFile4.close();
	//拓展：分割子社区的Kernighan-Lin算法
	cout << "请输入希望进行子社区分割的结点的数目，由于算法复杂度较高，所以输入最好小于50,否则执行时间会较长" << endl;
	int srcNum;
	cin >> srcNum;
	kernighanLin(srcNum, graph);
	system("pause");
	return 0;
}