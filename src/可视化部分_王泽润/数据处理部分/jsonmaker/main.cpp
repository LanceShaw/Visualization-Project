#include <iostream>
#include <fstream>
#include<vector>
#include<queue>
#include<iomanip>

using namespace std;
//����Ǩ��ͬѧ�Ĵ������֮����ɣ��������ɱ�Ҫ��json�ļ�


const double inf = 10000000.00; //��һ���ܴ�����������������
								//���������㷨
								//���·��������ʹ��Dijkstra�㷨
double Dijkstra(int src, int target, int n, const vector<vector<double>>&graph)
{
	//������յ���ͬ
	if (src == target)
	{
		return 0;
	}
	vector<double>distance(n); //����
	vector<bool>visited(n); //�Ƿ���ʹ�
	vector<vector<int>> path(n); //·��
								 //��ʼ��
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
		//�ҵ�������С�ĵ�current
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
				//t��current��ĳ��ľ���
				double t = distance[current] + graph[current][i];
				if (t < distance[i])
				{
					distance[i] = t;
					int k = 0;
					//��¼·��
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
	//���������֣����ӻ�ʱ��ɾȥ��
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
		cout << "��";
		return inf;
	}
}
//��ͨ����,ʹ���˹�����������ķ���
int ConnectedComponent(int n, const vector<vector<double>>& graph, double limit, vector<int>&start, vector<int>&end)
{
	int count = 0;
	vector<bool> visited(n);
	//�����ݴ�������Ͷ���
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

//��С֧������ʹ����prim�㷨,������С�������бߵĸ���
int spanningTree(int n, const vector<vector<double>> & graph, vector<int>&start, vector<int>&end)
{
	vector<bool> visited(n);
	vector<double> distance(n);
	vector<int> prior(n); //����ǰ��
	for (int i = 0; i < n; i++)
	{
		visited[i] = false;
		distance[i] = inf;
		prior[i] = -1;
	}
	//current����ָʾ��ǰѡ���
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
		//����Ӧ�ı߼��뵽�������������
		if (prior[current] != -1)
		{
			start[choosenEdgeNum] = prior[current];
			end[choosenEdgeNum] = current;
			choosenEdgeNum++;
		}
		//���¾���
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

//һ����չ�㷨��Kernighan-Lin�㷨��n��Ҫ������ͼ�Ĺ�ģ�������ͼ�ֳ���������
void kernighanLin(int n, const vector<vector<double>> & graph)
{
	//���溯��Q,��С�������������ڲ��ı�Ȩֵ��ȥ��������֮��ı�Ȩֵ
	double Q = 0;
	//�Ƿ��Ѿ�������
	vector<bool> visited(n);
	vector<int> sign(n); //��1��2����������
	for (int i = 0; i < n; i++)
	{
		visited[i] = false;
	}
	//��ʼʱ��0-n/2��Щ����Ϊһ������
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
	//��ʼ����Q
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
	//��ʼ���н���
	//���н������������
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
					//����������Qֵ
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
	//��ȡͼ��������������
	ifstream inFile;
	ofstream outFile3;
	ofstream outFile4;
	outFile4.open("link.json");
	outFile3.open("trees.json");
	inFile.open("1.movie.txt", ios::in);
	if (!inFile.good())
		cout << "�ļ���ȡ����";
	int nodeNum;
	int edgeNum;
	inFile >> nodeNum;
	inFile >> edgeNum;
	vector<vector<double>> graph(nodeNum);
	for (int i = 0; i < nodeNum; i++)
		graph[i] = vector<double>(nodeNum);
	//��ͼ�ĳ�ʼ��
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
	//��ȡ�����ͼ������
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
		//�༭json�ļ�
	}
	//Dijkstra�㷨
	double result_Dijkstra;
	int node1, node2;
	cout << "���������ڵ���룬���ֵ��" << nodeNum - 1 << endl;
	cin >> node1 >> node2;
	result_Dijkstra = Dijkstra(node1, node2, nodeNum, graph);
	cout << "weight:" << result_Dijkstra;

	//��С�������㷨,�õ��Ľ���Ǳߵļ���
	//ʼ�ߺ��ձߵ�����
	vector<int> start(nodeNum);
	vector<int> end(nodeNum);
	int choosenEdgeNum = spanningTree(nodeNum, graph, start, end);
	//������֣����ӻ�ʱ����ɾ����choosenEdgeNum������С�������бߵ�����
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
	//����ͨ����
	double limit;
	cout << "������ͨ�����ı���ֵ";
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
	//��չ���ָ���������Kernighan-Lin�㷨
	cout << "������ϣ�������������ָ�Ľ�����Ŀ�������㷨���ӶȽϸߣ������������С��50,����ִ��ʱ���ϳ�" << endl;
	int srcNum;
	cin >> srcNum;
	kernighanLin(srcNum, graph);
	system("pause");
	return 0;
}