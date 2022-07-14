#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
int main()
{
	srand(time(NULL));
	long long A, B;
	double w;
	char s[100];
	
	for ( int i = 1 ; i <= 5 ; i++ ) {
		strcpy(s, "../graph/facebook-links.txt");
		FILE * fin = fopen(s, "r");
		sprintf(s, "../graph/facebook-links-w-%d.txt", i);
		FILE * fout = fopen(s, "w");
		if ( !fin || !fout )
			puts("OAO");
		while ( fscanf(fin, "%lld%lld", &A, &B) != EOF ) {
			int seed = rand()%3;
			if ( seed == 0 )
				w = 0.1;
			else if ( seed == 1 )
				w = 0.01;
			else
				w = 0.001;
			fprintf(fout, "%lld %lld %lf\n", A, B, w);
		}
		fclose(fin);
		fclose(fout);
	}
	
	for ( int i = 1 ; i <= 5 ; i++ ) {
		strcpy(s, "../graph/release-flickr-links.txt");
		FILE * fin = fopen(s, "r");
		sprintf(s, "../graph/release-flickr-links-w-%d.txt", i);
		FILE * fout = fopen(s, "w");
		while ( fscanf(fin, "%lld%lld", &A, &B) != EOF ) {
			int seed = rand()%3;
			if ( seed == 0 )
				w = 0.1;
			else if ( seed == 1 )
				w = 0.01;
			else
				w = 0.001;
			fprintf(fout, "%lld %lld %lf\n", A, B, w);
		}
		fclose(fin);
		fclose(fout);
	}
	
	return 0;
}
