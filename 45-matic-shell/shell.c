#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <wait.h>
#include <fcntl.h>
#include <sys/stat.h>

#define DEBUG

#define CMD_SIZE 512
#define STACK_SIZE 512
#define SINGLE_QUOTE '\''
#define DOUBLE_QUOTE '\"'
#define SPACE ' '
#define TAB '\t'


char *cmd[CMD_SIZE];
size_t n_cmd;
bool quote_mismach;

bool is_space(char c) { return c == SPACE || c == TAB; }
bool is_quote(char c) { return c == SINGLE_QUOTE || c == DOUBLE_QUOTE; }

char *get_token(char *str, size_t *start, size_t size) {
	size_t l = *start, r = *start;
	size_t start_idx, end_idx;
	size_t token_len;
	char * token;
	bool is_quoted;

	/* Skip spaces */
	while (l < size && is_space(str[l])) {
		++l;
	}

	/* Expand the window */
	for (r = l; r < size && !is_space(str[r]); r += 1) {
		if (DOUBLE_QUOTE == str[r]) {
			r += 1;
			while (r < size && str[r] != DOUBLE_QUOTE) {
				r += 1;
			}
			if (!(r < size && str[r] == DOUBLE_QUOTE)) {
				quote_mismach = 1;
			}
		}
		if (SINGLE_QUOTE == str[r]) {
			r += 1;
			while (r < size && str[r] != SINGLE_QUOTE) {
				r += 1;
			}
			if (!(r < size && str[r] == SINGLE_QUOTE)) {
				quote_mismach = 1;
			}
		}
	}

	/* Extract token from l to r-1 */
	if (l < r) {
		token_len = r - l;
		is_quoted = (token_len >= 2 && is_quote(str[l]) && str[l] == str[r - 1]);

		/* Adjust start and end for quotes */
		start_idx = l + (is_quoted ? 1 : 0);
		end_idx = r - (is_quoted ? 1 : 0);

		token = malloc(end_idx - start_idx + 1);
		if (token) {
			memcpy(token, str + start_idx, end_idx - start_idx);
			token[end_idx - start_idx] = '\0';
		} else {
			perror("malloc failed");
			exit(EXIT_FAILURE);
		}
	}
#ifdef DEBUG
	// fprintf(stderr, "%ld:<%ld,%ld>", *start, l, r - 1);
#endif
	*start = r;
	return token;
}

void get_cmds(char *line, size_t len) {
	char *token;
    size_t start;
        
	start = 0;
	n_cmd = 0;
	quote_mismach = false;
	while (start < len) {
		token = get_token(line, &start, len);
#ifdef DEBUG
		fprintf(stderr, "Token:%s\n", token ? token : "<NULL>");
#endif 
		if (token) {
			if (n_cmd < CMD_SIZE - 1) {
				cmd[n_cmd++] = token;
			} else {
				fprintf(stderr, "Too many arguments\n");
				free(token);
			}
		}
	}
	cmd[n_cmd] = NULL;
}

int main() {
    size_t line_size;
    pid_t child;
	char *token;
	char *line;
    char *dir;
	struct stat sb;
	int len;

    line_size = CMD_SIZE;
	line = malloc(CMD_SIZE); 
	if (!line) {
		perror("Cannot allocate line buffer");
		exit(EXIT_FAILURE);
	}

	for (;;) {
		fprintf(stderr, "# ");
		/* XXX: 1. Get the input */
		len = getline(&line, &line_size, stdin);
		if (len < 0) {
			if (feof(stdin)) {
				free(line);
				exit(EXIT_SUCCESS); /* Exit on EOF */
			}
			perror("getline failed");
			continue;
		}

		/* XXX: 2. Extract cmnd and agrs */
		get_cmds(line, len-1); /* Skip the last char with is '\n' */

		if (quote_mismach) {
			fprintf(stderr, "error: mismatched quotes\n");
			goto free_tokens;
		}

		/* XXX: 3. Execute the cmd */
		if (n_cmd == 0) {
			continue;
		}else if (strcmp(cmd[0], "cd") == 0) { /* Handle cd */
            /* cd with no arguments goes to HOME
			 * No Need to free the memory return by getenv()
			 */
			dir = (n_cmd > 1) ? cmd[1] : getenv("HOME");
			if (!dir) {
				fprintf(stderr, "cd: HOME not set\n");
			} else if ((stat(dir, &sb) != 0) ||
				   !S_ISDIR(sb.st_mode) ||
				   (chdir(dir) != 0)) {
				fprintf(stderr, "error: cd failed\n");
			}
		} else if (strcmp(cmd[0], "exit") == 0) { /* Handle exit */
			exit(EXIT_SUCCESS);
		} else{ /* exec the binary */
			switch ((child = fork())) {
				case -1: {
					perror("Cannot fork to run command");
					break;
				}
				case 0: {
					execvp(cmd[0], cmd);
					perror("execvp failed");
					exit(EXIT_FAILURE);
				}
				default: {
					int status;
					wait(&status);
					/* https://www.geeksforgeeks.org/exit-status-child-process-linux/ */
					if (status) {
						fprintf(stderr, "error: command exited with code %d\n", WEXITSTATUS(status));
					}
				}
			}
		}

		/* Free tokens */
free_tokens:
		for (size_t i = 0; i < n_cmd; ++i) {
			free(cmd[i]);
			cmd[i] = NULL;
		}
	}

	free(line);
	return 0;
}