#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include<algorithm>
#include<map>
#include<queue>
#include<vector>
#define inf 2147483647
#define NodeBound 70000
using namespace std;
struct edge {
	int A;
	int B;
	edge() {}
	edge(int a, int b) {
		A = a;
		B = b;
		/*
		if ( a < b ) {
			A = a;
			B = b;
		}
		else {
			A = b;
			B = a;
		}*/
	}
	bool operator < (const edge &t) const {
		if ( A != t.A )	return A < t.A;
		return B < t.B;
	}
};
struct node {
	int B;
	double w;
	node() {}
	node(int _B, double _w) {
		B = _B;
		w = _w;
	}
	bool operator < (const node &t) const {
		return w > t.w;
	}
} ;
struct key2 {
	int v, r;
	key2(){}
	key2(int _v, int _r) {
		v = _v;
		r = _r;
	}
	bool operator < (const key2 &t) const {
		return ( v != t.v )	? v < t.v : r < t.r;
	}
} ;
struct key3 {
	int v, k, r;
	key3(){}
	key3(int _v, int _k, int _r) {
		v = _v;
		k = _k;
		r = _r;
	}
	bool operator < (const key3 &t) const {
		return ( v != t.v )	? v < t.v : (k != t.k) ? k < t.k : r < t.r;
	}
} ;

struct Stack {
	int * S;
	int ptr;
	Stack(int N) {
		ptr = 0;
		S = (int*)malloc(sizeof(int)*(N+1));
	}
	void push(int a) {
		S[ptr++] = a;
	}
	int pop() {
		return S[--ptr];
	}
	int empty() {
		return !ptr;
	}
	~Stack() {
		free(S);
	}
} ;

int * label;
int * visit;
int * nv;
int * par;
vector<node> link[NodeBound];
Stack * stack;

int Topo(int A) {
	visit[A] = 1;
	if ( par[A] && !visit[par[A]] ) {	// not root & unvisited
		if ( !Topo(par[A]) ) {
			return 0;
		}
	}
	stack->push(A);
	return !label[A];
}

int countNv(int A) {
	if ( !link[A].size() )
		return 0;
	nv[A] = 0;
	for ( int i = 0 ; i < link[A].size() ; i++ ) {
		nv[A] += countNv(link[A][i].B);
	}
	return ++nv[A];
}

map<key3, double> m;
map<key2, double> f;
map<key3, double> pi;
inline void setm(int v, int k, int r, double w) {
	m[key3(v,k,r)] = w;
}
inline double getm(int v, int k, int r) {
	return m[key3(v,k,r)];
}
inline void setf(int v, int r, double w) {
	f[key2(v,r)] = w;
}
inline double getf(int v, int r) {
	return f[key2(v,r)];
}
inline void setpi(int v, int r, int x, int xi) {
	pi[key3(v,r,x)] = xi;
}
inline int getpi(int v, int r, int x) {
	return pi[key3(v,r,x)];
}
inline void DPReset() {
	m.clear();
	f.clear();
	pi.clear();
}

inline double min(double a, double b) {
	return (a<b)?a:b;
}

int nI;
map<long long, int> Relabel;
vector<long long> Recover;
int toI(long long a) {
	if ( Relabel.find(a) == Relabel.end() ) {
		Relabel.insert(pair<long long, int>(a, nI++));
		Recover.push_back(a);
	}
	return Relabel.find(a)->second;
}

char graphPath[100];
char tarTreePath[300];
int degree[NodeBound];
map<edge, double> E;

void constructTree(vector<long long> tarId, double threshold) {
	clock_t start = clock();
	nI = 0;
	FILE * fin = fopen(graphPath, "r");
	if ( !fin ) {
		fprintf(stderr, "No graph\n");
		return ;
	}
	toI(-1);		// pseudo root
	for ( int i = 0 ; i < tarId.size() ; i++ ) {
		toI(tarId[i]);
	}
	long long A, B;
	double w;
	fill(degree,degree+NodeBound,0);
	
	while ( fscanf(fin, "%lld%lld", &B, &A) != EOF ) {
		A = toI(A);
		B = toI(B);
		E.insert(pair<edge,double>(edge(A,B), 0));
		if ( A >= NodeBound )	fprintf(stderr, "Out of node bound\n");
		degree[A]++;
		// degree[B]++;
	}
	fclose(fin);
	
	for ( map<edge,double>::iterator it = E.begin() ; it != E.end() ; it++ ) {
		A = it->first.A;
		B = it->first.B;
		w = it->second = (double)1/(double)degree[A];
		//fprintf(stderr, "%lf\n", w);
		if ( w > 1 )
			fprintf(stderr, "Impossible weight\n");
		link[A].push_back(node(B, -log(w)));
	}
	// pseudo link from root to target
	for ( int i = 0 ; i < tarId.size() ; i++ ) {
		link[toI(-1)].push_back(node(toI(tarId[i]), -log(1)));
		E.insert(pair<edge,double>(edge(toI(-1), toI(tarId[i])), 1));
	}
	
	// Dijkstra
	double thres = -log(threshold);
	int N = Recover.size();
	double * L = (double*)malloc(sizeof(double)*N);
	int * path = (int*)malloc(sizeof(int)*N);
	visit = (int*)malloc(sizeof(int)*N);
	fill(L,L+N,inf);
	fill(visit,visit+N,0);
	int S = toI(-1);
	priority_queue<node> Q;
	L[S] = 0;
	Q.push(node(S, 0));
	while ( !Q.empty() ) {
		A = Q.top().B;
		Q.pop();
		if ( visit[A] )
			continue;
		visit[A] = 1;
		for ( int i = 0 ; i < link[A].size() ; i++ ) {
			B = link[A][i].B;
			double dist = L[A] + link[A][i].w;
			if ( !visit[B] && dist < thres && dist < L[B] ) {
				L[B] = dist;
				path[B] = A;
				Q.push(node(B, dist));
			}
		}
	}
	
	// count nodes
	int count = 0;
	for ( int i = 0 ; i < N ; i++ ) {
		count += visit[i];
	}
	
	char input[50];
	FILE * fout = fopen(tarTreePath, "w");
	fprintf(fout, "%d\n", count);
	for ( int i = 0 ; i < N ; i++ ) {
		if ( visit[i] && i != S ) {
			fprintf(fout, "%lld %lld %lf\n", Recover[i], Recover[path[i]], E[edge(path[i],i)]);
		}
	}
	fclose(fout);
	Relabel.clear();
	Recover.clear();
	free(L);
	free(path);
	free(visit);
	for ( int i = 0 ; i < N ; i++ ) {
		link[i].clear();
	}
	E.clear();
	//puts("constructDone");
	//printf("%lf\n", ((double)(clock()-start))/CLOCKS_PER_SEC);
	fout = fopen("../tree/treeTime.txt", "a");
	fprintf(fout, "%d, %.8lf, %lf, %d", tarId.size(), threshold, ((double)(clock()-start))/CLOCKS_PER_SEC, count);
	for ( int i = 0 ; i < tarId.size() ; i++ )
		fprintf(fout, ", %lld", tarId[i]);
	fprintf(fout, "\n");
	fclose(fout);
}

void backTrace(int v, int x) {
	visit[v] = 1;
	x--;
	int count = 0;
	int dv = link[v].size();
	for ( int i = 0 ; i < dv ; i++ ) {
		if ( getpi(v, dv-i, x-count) ) {
			backTrace(link[v][dv-i-1].B, getpi(v, dv-i, x-count));
			count += getpi(v, dv-i, x-count);
		}
	}
}

void findFriends(long long srcId) {
	clock_t start = clock();
	char input[50];
	FILE * fin = fopen(graphPath, "r");
	sprintf(input, "../graph/%lld-friend.txt", srcId);
	FILE * fout = fopen(input, "w");
	long long A, B;
	while ( fscanf(fin, "%lld%lld", &A, &B) != EOF ) {
		if ( A == srcId ) fprintf(fout, "%lld\n", B);
		else if ( B == srcId ) fprintf(fout, "%lld\n", A);
	}
	fclose(fin);
	fclose(fout);
	//puts("findFriends Done");
	//printf("%lf\n", ((double)(clock()-start))/CLOCKS_PER_SEC);
}

// a 30 1 0.01 7

int main(int args, char* argv[])
{
	long long srcId;
	int NT;
	double threshold;
	double ap;
	int R;
	char input[50];
	sscanf(argv[1], "%lld", &srcId);
	sscanf(argv[2], "%d", &NT);
	vector<long long> tarId (NT, 0);
	for ( int i = 0 ; i < NT ; i++ ) {
		sscanf(argv[i+3], "%lld", &tarId[i]);
	}
	sscanf(argv[3+NT], "%lf", &threshold);
	sscanf(argv[4+NT], "%lf", &ap);
	sscanf(argv[5+NT], "%d", &R);
	if ( args == 7+NT )
		strcpy(graphPath, argv[6+NT]);
	else
		strcpy(graphPath, "../graph/graph.txt");
	
	sort(tarId.begin(), tarId.end());	// Make the order unique
	
	// construct tree
	strcpy(tarTreePath, "../tree/pseudo-");
	for ( int i = 0 ; i < NT ; i++ ) {
		sprintf(input, "%lld-", tarId[i]);
		strcat(tarTreePath, input);
	}
	sprintf(input, "%.8lf.txt", threshold);
	strcat(tarTreePath, input);
	FILE * fin = fopen(tarTreePath, "r");
	if ( !fin )
		constructTree(tarId, threshold);
	else
		fclose(fin);
	
	// construct friend list
	sprintf(input, "../graph/%lld-friend.txt", srcId);
	fin = fopen(input, "r");
	if ( !fin )
		findFriends(srcId);
	else
		fclose(fin);
	
	clock_t start = clock();
	
	fin = fopen(tarTreePath, "r");
	
	nI = 1;	//////////////////////////////////////// 1 or 0 ?
	
	int N;
	long long A, B;
	double w;
	fscanf(fin, "%d", &N);
	par = (int*)malloc(sizeof(int)*(N+1));
	fill(par,par+N+1,0);
	
	int T = toI(-1);
	par[T] = 0;
	for ( int i = 1 ; i < N ; i++ ) {
		fscanf(fin, "%lld%lld%lf", &A, &B, &w);
		B = toI(B);
		A = toI(A);
		par[A] = B;
		link[B].push_back(node(A, w));
	}
	fclose(fin);
	
	// load friends
	sprintf(input, "../graph/%lld-friend.txt", srcId);
	fin = fopen(input, "r");
	
	label = (int*)malloc(sizeof(int)*(N+1));
	fill(label,label+N+1,0);
	while ( fscanf(fin, "%lld", &A) != EOF ) {
		if ( toI(A) <= N ) {
			label[toI(A)] = 1;
		}
	}
	fclose(fin);
	
	// Topologic
	stack = new Stack(N);
	visit = (int*)malloc(sizeof(int)*(N+2));
	fill(visit,visit+N+1,0);
	for ( int i = 1 ; i <= N ; i++ ) {
		if ( !visit[i] ) {
			Topo(i);
		}
	}
	// free(visit);		// Will be used again in DP
	
	// Count nv
	nv = (int*)malloc(sizeof(int)*(N+1));
	fill(nv,nv+N+1,0);
	countNv(T);
	//puts("DP start");
	// DP
	while ( !stack->empty() ) {
		int v = stack->pop();
		if ( v == T )
			continue;
		int n = (nv[v] < R) ? nv[v] : R;
		for ( int i = 0 ; i < n ; i++ ) {
			setm(v,0,i,0);
		}
		setm(v,0,0,label[v]);
		int dv = link[v].size();
		int sumNv = 0;
		for ( int k = 1 ; k <= dv ; k++ ) {
			if ( label[link[v][k-1].B] )
				setm(v,k,0,1-(1-getm(v,k-1,0))*(1-getf(link[v][k-1].B, 0)*link[v][k-1].w));
			else
				setm(v,k,0,getm(v,k-1,0));
			sumNv += nv[link[v][k-1].B];
			int m = (sumNv < R-1) ? sumNv : R-1;
			for ( int x = 1 ; x <= m ; x++ ) {
				int minNx = (nv[link[v][k-1].B] < x) ? nv[link[v][k-1].B] : x;
				for ( int xi = 0 ; xi <= minNx ; xi++ ) {
					w = 1-(1-getm(v,k-1,x-xi))*(1-getf(link[v][k-1].B, xi)*link[v][k-1].w);
					if ( getm(v,k,x) < w ) {
						setm(v,k,x,w);
						setpi(v,k,x,xi);
					}
				}
			}
		}
		setf(v,0,(double)label[v]);
		int m = (sumNv < R-1) ? sumNv : R-1;
		for ( int x = 0 ; x <= m ; x++ ) {
			setf(v,x+1,getm(v,dv,x));
		}
	}
	
	// DP on pseudo root
	setf(T,0,0);
	int n = (nv[T]-1 < R) ? nv[T]-1 : R;
	for ( int i = 0 ; i <= n ; i++ ) {
		setm(T,0,i,0);
	}
	int sumNv = 0;
	for ( int k = 1 ; k <= link[T].size() ; k++ ) {
		setm(T,k,0,getm(T,k-1,0));
		int tk = link[T][k-1].B;
		sumNv += nv[tk];
		int m = (sumNv < R) ? sumNv : R;
		for ( int x = 1 ; x <= m ; x++ ) {
			int minNx = (nv[tk] < x) ? nv[tk] : x;
			int rx = 0;
			while ( getf(tk,rx) < ap && rx <= minNx ) {
				rx++;
			}
			for ( int xi = rx ; xi <= minNx ; xi++ ) {
				w = getm(T,k-1,x-xi)+getf(tk,xi);
				if ( getm(T,k,x) < w ) {
					setm(T,k,x,w);
					setpi(T,k,x,xi);
				}
			}
		}
	}
	setf(T,0,0);
	int m = (sumNv < R) ? sumNv : R;
	for ( int x = 0 ; x <= m ; x++ ) {
		setf(T,x,getm(T,link[T].size(),x));
	}
	//puts("DP Done");
	fill(visit,visit+N+1,0);
	visit[T] = 1;
	//backTrace(T,R);
	for ( int i = 1 ; i <= N ; i++ ) {
		if ( visit[i] )
			;//printf("%lld ", Recover[i-1]);
	}
	//puts("");
	/*
	for ( int i = 1 ; i <= N ; i++ ) {
		for ( int j = 0 ; j <= 20 ; j++ ) {
			fprintf(stderr, "%lf ", getf(i,j));
		}
		puts("");
	}*/
	n = (nv[T]-1 < R) ? nv[T]-1 : R;
	printf("%lf %lf\n", getf(T,n), ((double)(clock()-start))/CLOCKS_PER_SEC);
	
	free(nv);
	free(visit);
	free(label);
	free(par);
	delete stack;
	Relabel.clear();
	Recover.clear();
	DPReset();
	return 0;
}
