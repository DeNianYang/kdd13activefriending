#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
	int r;
	int src_id,tar_id;
	double theta = 0.0001;
	char dp_command[1024], edge_greedy_command[1024], node_greedy_command[1024],file_name[128];
	double result[3][3];
	FILE *system_out;

	// exe a.exe tar_id, \theta
	sscanf(argv[1], "%d", &tar_id);
	sscanf(argv[2], "%lf", &theta);
	printf ("%d %lf\n",tar_id, theta);
	for(r = 1; r < 1000; r ++){
		//for(tar_id = 100; tar_id <= 110; tar_id++){
			for(src_id = 1; src_id <= 60000; src_id++){
				printf("%d %d\n",src_id, tar_id);
				sprintf(dp_command, "dp.exe %d 1 %d %lf 0 %d ../graph/facebook-links.txt > t_%d_%d_out.txt",src_id, tar_id, theta, r, src_id, tar_id);
				puts("KK");
				system(dp_command);
				puts("KK");
				sprintf(file_name,"t_%d_%d_out.txt", src_id, tar_id);
				
				system_out = fopen(file_name, "r");
				puts("ZZ");
				fscanf(system_out, "%lf %lf",&result[0][0], &result[0][1]);
				fclose(system_out);
				printf("%lf %lf",result[0][0], result[0][1]);
				
				sprintf(edge_greedy_command, "edge_greedy.exe %d 1 %d %lf 0 %d ../graph/facebook-links.txt > t_%d_%d_out.txt",src_id, tar_id, theta, r, src_id, tar_id);
				system(edge_greedy_command);
				sprintf(file_name,"t_%d_%d_out.txt", src_id, tar_id);
				system_out = fopen(file_name, "r");
				fscanf(system_out, "%lf %lf",&result[1][0], &result[1][1]);
				fclose(system_out);

				sprintf(node_greedy_command, "node_greedy.exe %d 1 %d %lf 0 %d ../graph/facebook-links.txt > t_%d_%d_out.txt",src_id, tar_id, theta, r, src_id, tar_id);
				system(node_greedy_command);
				sprintf(file_name,"t_%d_%d_out.txt", src_id, tar_id);
				system_out = fopen(file_name, "r");
				fscanf(system_out, "%lf %lf",&result[2][0], &result[2][1]);
				fclose(system_out);

				double temp = (result[0][0] - result[1][0]);
				if(result[1][0] > 0 && temp / result[1][0] >= 0.1){
					printf("%d, %d, %d, %lf, %lf, %lf, %lf, %lf, %lf\n", src_id, tar_id, r, result[0][0], result[1][0], result[2][0], result[0][1],result[1][1], result[2][1]);
				}
			}
		//}
	}




	return 0;
}