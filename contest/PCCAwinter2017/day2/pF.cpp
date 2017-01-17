#include <bits/stdc++.h>
using namespace std;

const int INF = 1<<29;

struct Dinic{	//O(VVE)
	static const int MAXV = 5003;
	struct Edge{
		int from, to, cap, flow;
	};

	int n, m, s, t, d[MAXV], cur[MAXV];
	vector<Edge> edges;
	vector<int> G[MAXV];

	void init(int _n=MAXV){
		edges.clear();
		for (int i=0; i<_n; i++)G[i].clear();
	}

	void AddEdge(int from, int to, int cap){
		edges.push_back( {from,to,cap,0} );
		edges.push_back( {to,from,0,0} );
		m = edges.size();
		G[from].push_back(m-2);
		G[to].push_back(m-1);
	}
	
	bool dinicBFS(){
		memset(d,-1,sizeof(d));
		queue<int> que;
		que.push(s); d[s]=0;
		while (!que.empty()){
			int u = que.front(); que.pop();
			for (int ei:G[u]){
				Edge &e = edges[ei];
				if (d[e.to]<0 && e.cap>e.flow){
					d[e.to]=d[u]+1;
					que.push(e.to);
				}
			}
		}
		return d[t]>=0;
	}

	int dinicDFS(int u, int a){
		if (u==t || a==0)return a;
		int flow=0, f;
		for (int &i=cur[u]; i<(int)G[u].size(); i++){
			Edge &e = edges[ G[u][i] ];
			if (d[u]+1!=d[e.to])continue;
			f = dinicDFS(e.to, min(a, e.cap-e.flow) );
			if (f>0){
				e.flow += f;
				edges[ G[u][i]^1 ].flow -=f;
				flow += f;
				a -= f;
				if (a==0)break;
			}
		}
		return flow;
	}

	int maxflow(int s, int t){
		this->s = s, this->t = t;
		int flow=0, mf;
		while ( dinicBFS() ){
			memset(cur,0,sizeof(cur));
			while ( (mf=dinicDFS(s,INF)) )flow+=mf;
		}
		return flow;
	}
}dinic;

// s=0, t=1;
int fnd(int id ,int out=0){
	// out=0 入點 out=1 出點
	static int spr=1;
	//spr=2 時每個點分成入點,出點
	return id*spr+out+2;
}

struct Edge2{
    int u, v, w;
};

const int MAXN = 51;
const int MAXM = 502;

int N, M, X, minw;
Edge2 link[MAXM];

void input(){
    minw=INF;

    scanf("%d %d %d", &N, &M, &X);

    for (int i=0; i<M; i++){
        scanf("%d %d %d", &link[i].u, &link[i].v, &link[i].w);
        link[i].u--;
        link[i].v--;
        minw = min(minw,link[i].w);
    }
}

bool ok(double W){
    dinic.init();
    
    for (int i=0; i<M; i++){
        int u = link[i].u;
        int v = link[i].v;
        int w = link[i].w;
        
        w = floor( (double)w/W );

        dinic.AddEdge(u,v,w);
    }

    return dinic.maxflow(0,N-1)>=X;
}

int main(){
    input();

    double l = (double)minw, r = INF;

    for (int i=0; i<60; i++){
        double mid = (l+r)/2.0;
        if ( ok(mid/X) )l=mid;
        else r=mid;
    }

    printf("%.10f\n",l);
}
