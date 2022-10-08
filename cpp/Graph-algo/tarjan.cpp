#include<iostream>
#include<vector>
#include<stack>
using namespace std;
void dfs(vector<int>* graph, int sv, vector<int>& DiscTime, vector<int>& low, vector<bool>& inStack, stack<int>& s,vector<vector<int>>& components) {
	static int time = 0;
	DiscTime[sv] = low[sv] = time++; //store the discovery time of node and increment time
	s.push(sv);
	inStack[sv] = true;
	for (auto v : graph[sv]) {
		//if v is not visited
		if (DiscTime[v] == -1) {
			dfs(graph, v, DiscTime, low, inStack, s,components);
			low[sv] = min(low[sv], low[v]);
		}
		else if (inStack[v]) {//if node is visited but is present in stack means it is a back edge else cross edge
			low[sv] = min(low[sv], DiscTime[v]);//we ignore cross edge here as it resembles differnet component 
		}
	}
	if (DiscTime[sv] == low[sv]) {//means current node is a head node
		vector<int> v;
		while (s.top() != sv) {
			v.push_back(s.top());
			inStack[s.top()] = false;
			s.pop();
		}
		v.push_back(s.top());
		inStack[s.top()] = false;
		s.pop();
		components.push_back(v);
	}
}
vector<vector<int>> tarjans(vector<int>* graph, int V) {
	// DiscTime for storing the discovery of nodes
	vector<int> DiscTime(V, -1);//initially it is -1;
	//low for storing lowest node reachable to current node
	vector<int> low(V, -1);
	vector<bool> inStack(V, 0);//for checking if there is backedge or not
	stack<int> s;
	//travel on graph and call dfs if a node is not visited
	vector<vector<int>> components;
	for (int i = 0; i < V; i++) {
		if (DiscTime[i] == -1) 
			dfs(graph, i, DiscTime, low, inStack, s,components);
	}
	return components;
}
int main(){
	int V, E;
	cin >> V >> E;
	vector<int>* graph = new vector<int>[V];
	for (int i = 0; i < E; i++) {
		int a, b;
		cin >> a >> b;
		graph[a].push_back(b);
	}
	vector<vector<int>> SCC = tarjans(graph, V);
	for (int i = 0; i < SCC.size(); i++) {
		for (int j = 0; j < SCC[i].size(); j++)
			cout << SCC[i][j] << " ";
		cout << endl;
	}

	//code for finding bridged in graph using tarjan
	class Solution {
	public:
		int time = 0;
		vector<vector<int>> bridge;
		void dfs(vector<int>* graph, int sv, vector<int>& DiscTime, vector<int>& low, vector<int>& parent) {
			DiscTime[sv] = low[sv] = time++;
			for (int i = 0; i < graph[sv].size(); i++) {
				if (DiscTime[graph[sv][i]] == -1) {
					parent[graph[sv][i]] = sv;
					dfs(graph, graph[sv][i], DiscTime, low, parent);
					low[sv] = min(low[sv], low[graph[sv][i]]);
					if (low[graph[sv][i]] > DiscTime[sv]) {
						vector<int>v;
						v.push_back(sv);
						v.push_back(graph[sv][i]);
						bridge.push_back(v);
					}
				}
				else if (parent[sv] != graph[sv][i]) {
					low[sv] = min(low[sv], DiscTime[graph[sv][i]]);
				}
			}
		}
		vector<vector<int>> criticalConnections(int n, vector<vector<int>>& connections) {
			vector<int>* graph = new vector<int>[n];
			for (int i = 0; i < connections.size(); i++) {
				graph[connections[i][0]].push_back(connections[i][1]);
				graph[connections[i][1]].push_back(connections[i][0]);
			}
			vector<int> DiscTime(n, -1);
			vector<int> low(n, -1);
			vector<int> parent(n, -1);
			for (int i = 0; i < n; i++) {
				if (DiscTime[i] == -1)
					dfs(graph, i, DiscTime, low, parent);
			}
			return bridge;
		}
	};
}
