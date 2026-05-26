#include <fstream>
#include <vector>
#include <stack>
using namespace std;
ifstream cin("in.txt");
ofstream cout("out.txt");
const int maxLen = 300005;

int count = 0;
int lvl[maxLen];
int low[maxLen];
bool mark[maxLen];
vector <pair<int,int>> bridge;
int critical[maxLen];
stack <int> components;
vector <int> roots;
vector <int> adj[maxLen];
vector <vector <int>> biconComp;

bool verifyEdge(int node, int parent)
{
    return lvl[parent] < low[node];
}

void biconectDFS(int node, int parent, int root)
{
    lvl[node] = lvl[parent] + 1;
    low[node] = lvl[node];
    components.push(node);
    mark[node] = true;
    
    int childrenCount = 0;
    
    for(int next : adj[node])
    {
        if(next == parent)
        {
            continue;
        }
        
        if(mark[next])
        {
            low[node] = min(low[node],lvl[next]);
            continue;
        }
        
        childrenCount++;
        biconectDFS(next,node,root);
        low[node] = min(low[node],low[next]);
        
        if(verifyEdge(next, node))
        {
            bridge.push_back({next,node});
        }
        
        if(lvl[node] <= low[next])
        {
            roots.push_back(next);
            vector <int> comp;
            count ++;
            while(components.top() != next)
            {
                int topNode = components.top();
                comp.push_back(topNode);
                components.pop();
            }
            
            if(node != root)
                critical[node] = 1;
            
            comp.push_back(next);
            components.pop();
            comp.push_back(node);
            
            biconComp.push_back(comp);
        }
    }
    
    if(node == root)
    {
        if(childrenCount > 1)
            critical[node] = 1;
    }
}

void showComponents()
{
    cout << "There are " << count << " biconnected components of the graph:" << '\n';
    
    int it = 1;
    for(auto comp : biconComp)
    {
        string end = "th";
        if(it == 1)
            end = "st";
        else if(it == 2)
            end = "nd";
        else if(it == 3)
            end = "rd";

        cout << "The " << it << end << " component has " << comp.size() << " verticies: ";
        for(int node : comp)
        {
            cout << node << " ";
        }
        cout << '\n';
        it ++;
    }
}

void showCritNodes(int n)
{
    int nr = 0;
    
    for(int i = 1; i <= n; i++)
    {
        if(critical[i])
            nr++;
    }
    
    cout << "There are " << nr << " critical nodes: " << '\n';
    
    for(int i = 1; i <= n; i++)
    {
        if(critical[i])
            cout << i << " ";
    }
}

void showBridges()
{
    cout << "There are " << bridge.size() << " critical edges: " << '\n';
    
    for(int i = 0; i < bridge.size(); i++)
    {
        int x = bridge[i].first;
        int y = bridge[i].second;
        cout << "Edge " << i+1 << ": " << "between " << x << " and " << y << '\n';
    }
}

int n,m;
int main()
{
    cin >> n >> m;
    fill(mark, mark + n + 3, false);
    for(int i = 1; i <= m; i++)
    {
        int x,y;
        cin >> x >> y;
        adj[x].push_back(y);
        adj[y].push_back(x);
    }
    
    for(int i = 1; i <= n; i++)
    {
        if(!mark[i])
        {
            biconectDFS(i,0,i);
        }
    }
    showComponents();
    cout << '\n';
    showCritNodes(n);
    cout << '\n';
    showBridges();
    return 0;
}