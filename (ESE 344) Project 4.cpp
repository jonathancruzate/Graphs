// Jonathan Cruzate
// jcruzate
// 110130802
// ESE 344
// Project 4

#include <fstream>
#include <iostream>
#include <vector>
#include <assert.h>
using namespace std;

const int MAX_SIZE = 100;

struct Node
{
	int vertex1;
	int vertex2;
	double distance = 0;
	Node* next = nullptr;
};

class Vertex
{
public:
	int id;
	Vertex* next = nullptr;
	Node* head = nullptr;
	void insert(Node* e);
	double getDistance(int vertex2);
};

double Vertex::getDistance(int vertex2)
{
	Node* temp = head;

	while (temp != nullptr && temp->vertex2 != vertex2)
	{
		temp = temp->next;
	}

	if (temp == nullptr)
	{
		return 0;
	}
	else if (temp->vertex2)
	{
		return temp->distance;
	}
	else
	{
		return 0;
	}
}

void Vertex::insert(Node* edge)
{
	if (head == nullptr)
	{
		head = edge;
	}
	else if (edge->vertex2 < head->vertex2)
	{
		Node* temp = head;
		edge->next = temp;
		head = edge;
	}
	else
	{
		Node* temp = head;

		while (temp->next != nullptr && !(edge->vertex2 < temp->next->vertex2))
		{
			temp = temp->next;
		}

		edge->next = temp->next;
		temp->next = edge;
	}
}

class MyQueue
{
public:
	void append(Vertex* v);
	Vertex* retrieve();
	void serve();
	bool empty();
private:
	Vertex* front = nullptr;
	Vertex* rear = nullptr;
};

bool MyQueue::empty()
{
	return front == nullptr;
}

void MyQueue::append(Vertex* v)
{
	if (front == nullptr)
	{
		front = v;
		rear = v;
	}
	else
	{
		Vertex* temp = v;
		v->next = rear;
		rear = v;
	}
}

Vertex* MyQueue::retrieve()
{
	return front;
}

void MyQueue::serve()
{
	Vertex* temp = rear;

	if (front == rear)
	{
		front = nullptr;
		rear = nullptr;
		return;
	}

	while (temp->next != nullptr && temp->next != front)
	{
		temp = temp->next;
	}

	front = temp;
}

class DirectedGraph
{
public:
	void printGraph();
	void depthFirst();
	void breadthFirst();
	void traverse(int v, bool visited[]);
	vector<Vertex> adjacencyList;
	void shortestPath();
	int minDistance(vector<double> distance, vector<bool> visited);
};

const double infinity = numeric_limits<double>::max();

int DirectedGraph::minDistance(vector<double> distance, vector<bool> visited)
{
	int size = adjacencyList.size();
	int min = INT_MAX, min_index = -1;

	for (int v = 0; v < size; v++)
	{
		if (!visited[v] && distance[v] <= min)
		{
			min = distance[v];
			min_index = v;
		}
	}

	return min_index;
}

void DirectedGraph::shortestPath()
{
	int n = adjacencyList.size(), x;

	vector<int> parent(n);
	vector<double> distance(n);
	vector<bool> visited(n);

	for (int i = 0; i < n; i++)
	{
		distance[i] = INT_MAX;
		visited[i] = false;
		parent.push_back(0);
	}

	distance[0] = 0;

	for (int count = 0; count < n - 1; count++)
	{
		int u = minDistance(distance, visited);
		visited[u] = true;

		for (int v = 0; v < n; v++)
		{
			if (!visited[v] && adjacencyList[u].getDistance(v) && distance[u] != INT_MAX && distance[u] + adjacencyList[u].getDistance(v) < distance[v])
			{
				distance[v] = distance[u] + adjacencyList[u].getDistance(v);
				parent[v] = u;
			}
		}
	}

	for (int i = n - 1; i > 0; i--)
	{
		cout << "0->";
		x = i;
		while (1)
		{
			x = parent[x];
			if (x == 0)
			{
				break;
			}
			cout << x << "->";
		}

		cout << i << endl;

		cout << "Distance: ";
		cout << distance[i] << endl;
	}
}

void DirectedGraph::breadthFirst()
{
	bool visited[MAX_SIZE];

	MyQueue q;
	Vertex* v, * x;

	for (int i = 0; i < adjacencyList.size(); i++)
	{
		visited[i] = false;
	}

	for (int i = 0; i < adjacencyList.size(); i++)
	{
		if (!visited[i]) {
			v = new Vertex;
			v->id = adjacencyList[i].id;
			q.append(v);
			while (!q.empty())
			{
				if (!visited[q.retrieve()->id])
				{
					visited[q.retrieve()->id] = true;
					cout << q.retrieve()->id << " ";
					Node* temp = adjacencyList[q.retrieve()->id].head;
					while (temp != nullptr)
					{
						x = new Vertex;
						x->id = temp->vertex2;
						q.append(x);
						temp = temp->next;
					}
				}
				q.serve();
			}
		}
	}
	cout << endl;
}

void DirectedGraph::depthFirst()
{
	bool visited[MAX_SIZE];
	Vertex v;

	for (int i = 0; i < adjacencyList.size(); i++)
	{
		visited[i] = false;
	}
	for (int i = 0; i < adjacencyList.size(); i++)
	{
		if (!visited[i])
		{
			traverse(i, visited);
		}
	}

	cout << endl;
}

void DirectedGraph::traverse(int v, bool visited[])
{
	visited[v] = true;
	cout << v << " ";
	Node* temp = adjacencyList[v].head;

	while (temp != nullptr)
	{
		if (!visited[temp->vertex2])
		{
			traverse(temp->vertex2, visited);
		}
		temp = temp->next;
	}
}

void DirectedGraph::printGraph()
{
	for (int i = 0; i < adjacencyList.size(); i++)
	{
		Node* temp = adjacencyList[i].head;

		while (temp != nullptr)
		{
			cout << temp->vertex1 << " ";
			cout << temp->vertex2 << " ";
			cout << temp->distance << endl;
			temp = temp->next;
		}
	}
}

int main()
{
	fstream fin;

	int N, Vi = 0, Vj;
	double W;

	//Adjacency list
	DirectedGraph graph;

	fin.open("input.txt");
	if (fin.fail())
	{
		cerr << "Can't find file" << endl;
		return 0;
	}

	//Get number of vertices for the graph.
	fin >> N;

	for (int i = 0; i < N; i++)
	{
		Vertex v;
		v.id = i;
		graph.adjacencyList.push_back(v);
	}

	cout << "Edges:\n";
	while (Vi != -1)
	{
		fin >> Vi;
		fin >> Vj;
		fin >> W;

		if (Vi == -1)
		{
			break;
		}

		//Create an edge
		Node* edge = new Node;
		edge->vertex1 = Vi;
		edge->vertex2 = Vj;
		edge->distance = W;

		cout << Vi << " ";
		cout << Vj << " ";
		cout << W << endl;

		graph.adjacencyList[Vi].insert(edge);
	}

	cout << endl;

	cout << "Graph:\n";
	graph.printGraph();

	cout << "\nDepth First:\n";
	graph.depthFirst();

	cout << "\nBreadth First:\n";
	graph.breadthFirst();

	cout << "\nShortest Paths:\n";
	graph.shortestPath();

	return 0;
}