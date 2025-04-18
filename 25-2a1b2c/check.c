#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>


int read5(int fd, char *buf) {
	int have = 0, bytes;
	while (have != 5) {
		bytes = read(fd, buf + have, 5 - have);
		if (bytes < 0) {
			perror("can not read");
			exit(2);
		}
		if (bytes == 0) {
			return 0;
		}
	  have += bytes;
	}
	return have;
}

int check5(char * buf) {
	int cnt[3] = {0};
	for (int i=0; i<5; ++i) {
		char c = buf[i];
		cnt[c - 'A'] += 1;
	}
	if (!(cnt[0] == 2 && cnt[1] == 1 && cnt[2] == 2)) {
		printf("%d%d%d\n", cnt[0], cnt[1], cnt[2]);
		return 1;
	}
	return 0;
}

int main() {
	int fd = open("out", O_RDONLY);
	if (fd < 0) {
		perror("cant open");
		exit (1);
	}
	char buf[5];
	while (read5(fd, buf)) {
		if (check5(buf)) {
			perror("test failed\n");
			exit(1);
		}
	}
}

