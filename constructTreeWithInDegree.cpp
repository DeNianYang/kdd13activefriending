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
#define NodeBound 1870000
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
int degree[NodeBound];
map<edge, double> E;

void constructTree() {
	clock_t start = clock();
	nI = 0;
	FILE * fin = stdin;
	if ( !fin ) {
		fprintf(stderr, "No graph\n");
		return ;
	}
	long long A, B;
	double w;
	fill(degree,degree+NodeBound,0);
	
	while ( fscanf(fin, "%lld%lld", &A, &B) != EOF ) {
		A = toI(A);
		B = toI(B);
		E.insert(pair<edge,double>(edge(A,B), 0));
		if ( A >= NodeBound )	fprintf(stderr, "Out of node bound\n");
		degree[B]++;
		// degree[B]++;
	}
	fclose(fin);
	
	for ( map<edge,double>::iterator it = E.begin() ; it != E.end() ; it++ ) {
		A = it->first.A;
		B = it->first.B;
		w = it->second = (double)1/(double)degree[B];
		//fprintf(stderr, "%lf\n", w);
		printf("%lld %lld %lf\n", Recover[A], Recover[B], w);
	}
	
}

int main(int args, char* argv[])
{
	constructTree();

	return 0;
}
