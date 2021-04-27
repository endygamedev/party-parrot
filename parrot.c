#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/select.h>


int main(int argc, char **argv) {
	FILE **files = malloc(argc*sizeof(FILE*));
	FILE *file;
	char c;
	int frame = 0;

	fd_set readfds;
	struct timeval tv;
	int ch;
	int b = 1, ret;


	for(int i = 1; i < argc; i++) {
		files[i] = fopen(argv[i], "r");
	}

	FD_ZERO(&readfds);
	while (b) {
		FD_SET(STDIN_FILENO, &readfds);
		tv.tv_sec = 0;
		tv.tv_usec = 0;

		system("clear");
		while((c = fgetc(files[frame%argc])) != EOF)
			printf("%c", c);
		usleep(300000);
		rewind(files[frame%argc]);
		frame++;

		ret = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv);
		if (ret < 0) {
			perror("select error");
			exit(1); 
		} else if (FD_ISSET(STDIN_FILENO, &readfds)) {
			ch = fgetc(stdin);
			if (ch == 'q') {
				b = 0;
			}
		}
		fprintf(stderr, ".");
	}

	return 0;
}
