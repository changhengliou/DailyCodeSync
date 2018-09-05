#include <iostream>
#include <memory>
#include <stack>
#include <unordered_set>
#include <vector>

using namespace std;

void t_helper(vector<unordered_set<int>>& adj, unique_ptr<bool>& visited,
              int curr, stack<int> _stack) {
  visited.get()[curr] = true;

  for (auto p : adj[curr])
    if (!visited.get()[p]) t_helper(adj, visited, p, _stack);

  _stack.push(visited.get()[curr]);
}

void topological_sort(vector<unordered_set<int>>& adj) {
  unique_ptr<bool> visited(new bool[adj.size()]);
  stack<int> _stack;
  fill(visited.get(), visited.get() + adj.size(), false);
  for (int i = 0; i < adj.size(); ++i)
    if (!visited.get()[i]) t_helper(adj, visited, i, _stack);

  while (!_stack.empty()) {
    cout << _stack.top() << endl;
    _stack.pop();
  }
}

int main() {
  const int vertex_num = 6;
  const vector<pair<int, int>> adj_pair = {{5, 0}, {5, 2}, {4, 0},
                                           {4, 1}, {2, 3}, {3, 1}};
  vector<unordered_set<int>> adj_set(vertex_num);
  for (auto p : adj_pair) {
    adj_set[p.first].insert(p.second);
  }
  topological_sort(adj_set);
}