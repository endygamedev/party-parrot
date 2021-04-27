#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/select.h>
#include <dirent.h>
#include <string.h>

#define MAX_PATH 2048
#define SPEED 80000


int main(void) {
	struct dirent **dir;
	char c, path[MAX_PATH] = "/usr/local/lib/parrot/frames/text", full_path[MAX_PATH];
	fd_set readfds;
	struct timeval tv;
	int ch, n, iter = 1, ret, frame = 2;
	FILE **files;
	
	n = scandir(path, &dir, 0, alphasort);
	if(n < 0) {
		perror("ERROR: scandir error...");
		exit(1);
	} else {
		files = malloc(n*sizeof(FILE*));
		for(int i = 2; i < n; i++){
			full_path[0] = '\0';
			strcat(full_path, path);
			strcat(full_path, "/");
			strcat(full_path, dir[i]->d_name);
			files[i] = fopen(full_path, "r");
		}
	}
	
	if(!files) {
		perror("ERROR: files error...");
		exit(1);
	} 
	
	FD_ZERO(&readfds);
	while(iter) {
		FD_SET(STDIN_FILENO, &readfds);
		tv.tv_sec = 0;
		tv.tv_usec = 0;

		system("clear");
		while((c = fgetc(files[frame])) != EOF)
			printf("%c", c);
		usleep(SPEED);
		rewind(files[frame]);

		frame++;
		if(frame >= n)
			frame = 2;
		
		ret = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv);
		if (ret < 0) {
			perror("ERROR: select error...");
			exit(1); 
		} else if (FD_ISSET(STDIN_FILENO, &readfds)) {
			ch = fgetc(stdin);
			if (ch == 'q') {
				iter = 0;
				system("clear");
			}
		}
	}
	return 0;
}
