#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[]){
	char buffer[300];
	char path[300];
	int numberOfTests = atoi(argv[1]);

	for(int i=1;i<=numberOfTests;i++){
		sprintf(buffer, "mkdir -p %s%d_out ", argv[2],i);
		system(buffer);

		sprintf(buffer, "cp -a %s%d/. %s%d_out ", argv[2],i,argv[2],i);
		system(buffer);

		sprintf(buffer, "cp exec %s%d_out ", argv[2],i);
		system(buffer);

		sprintf(buffer, "chmod u+x ./%s%d_out/exec", argv[2],i);
		system(buffer);

		sprintf(buffer, "cd ./%s%d_out/", argv[2],i);
		system(buffer);

		sprintf(buffer, "(cd ./%s%d_out/ && (./exec < %d.in) > %d.out)",argv[2],i,i,i);
		system(buffer);

		sprintf(buffer, "(cd ./%s%d_out/ && rm exec)",argv[2],i);
		system(buffer);

	}
	sprintf(buffer, "rm -f %sresults.txt", argv[2]);
	system(buffer);

	sprintf(buffer, "cat %sresults.txt", argv[2]);
	system(buffer);

	sprintf(buffer, "echo \"Resultados: \" > %sresults.txt", argv[2]);
	system(buffer);

	for(int i=1;i<=numberOfTests;i++){
		sprintf(buffer, "echo  '%d: ' >> %sresults.txt",i, argv[2]);
		system(buffer);

		sprintf(buffer, "(diff  %s%d_out/%d.out %s%d/%d.out ) >> %sresults.txt", argv[2],i,i,argv[2],i,i,argv[2]);
		system(buffer);

		sprintf(buffer, "echo \" \n \" >> %sresults.txt", argv[2]);
		system(buffer);

	}
	
	return 0;
}
