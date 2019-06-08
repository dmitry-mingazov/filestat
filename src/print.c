#include "print.h"

void printFstat(file_info temp_file){
	printf("Acquired in date: %s \n",ctime(&temp_file.date));
	printf("Size: %ld \n",temp_file.size);
	printf("Mode file: %d \n",temp_file.mode);
	printf("Access time: %s \n",ctime(&temp_file.atime));
	printf("Modified time: %s \n",ctime(&temp_file.mtime));
	printf("Change time: %s \n",ctime(&temp_file.ctime));
	printf("Number of links: %hu \n",temp_file.nlink);
	printf("User ID: %d \n",temp_file.uid);
	printf("Group ID: %d \n",temp_file.gid);

	char perm[10];

	parse_mode(temp_file.mode, perm);
	printf("Permissions: %s\n", perm);
}
