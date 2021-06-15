#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/select.h>
#include <dirent.h>
#include <string.h>


#define MAX_PATH 2048
#define SPEED 80000
#define PATH "/usr/local/lib/parrot/frames/"


int is_number(char *arg) {
	int flag = 1;
	for(int i = 0; i < (int)strlen(arg); i++) {
		if(!isdigit(arg[i])) {
			flag = 0;
			break;
		}
	}
	return flag;
}


int main(int argc, char *argv[]) {
	struct dirent **dir;
	char c, path[MAX_PATH] = PATH, full_path[MAX_PATH];
	fd_set readfds;
	struct timeval tv;
	int ch, n, iter = 1, ret, frame = 2, speed, speed_flag = 0;
	FILE **files;

	strcat(path, "text");
	
	for(int i = 1; i < argc; i++) {
		if(!strcmp("-c", argv[i])) {
			strcpy(path, PATH);
			if(!strcmp("ascii", argv[++i]))
				strcat(path, "ascii_text");
			else
				strcat(path, "text");
			continue;
		}

		if(!strcmp("-s", argv[i])) {
			if(is_number(argv[++i])) {
				speed = atoi(argv[i]);
				speed_flag = 1;
			} else {
				perror("ERROR: incorrect value for `speed`...");
				exit(1);
			}

			if(speed < 1) {
				perror("ERROR: speed is too fast...");
				exit(1);
			} else if(speed > 15) {
				perror("ERROR: speed is too slow...");
				exit(1);
			}

			speed *= 10000;
			continue;
		}
	}
	
	if(!speed_flag)
		speed = SPEED;
	
	n = scandir(path, &dir, 0, alphasort);
	if(n < 0) {
		perror("ERROR: scandir error...\n");
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
		usleep(speed);
		rewind(files[frame]);

		frame++;
		if(frame >= n)
			frame = 2;
		
		ret = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv);
		if(ret < 0) {
			perror("ERROR: select error...");
			exit(1); 
		} else if(FD_ISSET(STDIN_FILENO, &readfds)) {
			ch = fgetc(stdin);
			if(ch == 'q') {
				iter = 0;
				system("clear");
			}
		}
	}

	free(files);
	return 0;
}
