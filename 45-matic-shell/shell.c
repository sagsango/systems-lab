/* Author: Sagar Singh
 * Email: ssing214@uic.edu
 *
 * Minimal shell which supports:
 * 1. Builtin command cd
 * 2. Buildin command exit
 * 3. Running simple cmds with args by fork() and exec()
 *
 * We dont support & or |
 * Which is left for future support.
 *
 * To see the debug message use #define DEBUG
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <wait.h>
#include <fcntl.h>
#include <sys/stat.h>

//#define DEBUG

#define CMD_SIZE 512
#define STACK_SIZE 512
#define SINGLE_QUOTE '\''
#define DOUBLE_QUOTE '\"'
#define SPACE ' '
#define TAB '\t'
#define PROMT "$ "
#define LINE_DELIMITER_LEN 1

enum ret_status {
	SUCCESS,
	ERR_QUOTE_MISMATCH,
	ERR_FORK,
	ERR_EXEC,
	ERR_CHILD_CMD,
	ERR_TOO_MANY_ARGS,
	ERR_CD,
	ERR_EMPTY_CMD
};

char *cmd[CMD_SIZE];
size_t n_cmd;

bool is_space(char c) { return c == SPACE || c == TAB; }
bool is_quote(char c) { return c == SINGLE_QUOTE || c == DOUBLE_QUOTE; }

int get_token(char *str, size_t *start, size_t size, char **ret_token) {
	size_t l = *start, r = *start;
	size_t start_idx, end_idx;
	size_t token_len;
	char * token;
	bool is_quoted;
	int ret_status;

	ret_status = SUCCESS;
	*ret_token = NULL;

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

				ret_status |= ERR_QUOTE_MISMATCH;
			}
		}
		if (SINGLE_QUOTE == str[r]) {
			r += 1;
			while (r < size && str[r] != SINGLE_QUOTE) {
				r += 1;
			}
			if (!(r < size && str[r] == SINGLE_QUOTE)) {

				ret_status |= ERR_QUOTE_MISMATCH;
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
	*ret_token = token;
	return ret_status;
}

int get_cmds(char *line, size_t len) {
	char *token;
	size_t start;
	int ret_status;

	start = 0;
	n_cmd = 0;
	ret_status = SUCCESS;
	while (start < len) {
		ret_status |= get_token(line, &start, len, &token);
#ifdef DEBUG
		fprintf(stderr, "Token:%s\n", token ? token : "<NULL>");
#endif 
		if (token) {
			if (n_cmd < CMD_SIZE - 1) { /* last arg[] will point to NULL */
				cmd[n_cmd++] = token;
			} else {
				fprintf(stderr, "Too many arguments\n");
				ret_status |= ERR_TOO_MANY_ARGS;
				free(token);
			}
		}
	}
	cmd[n_cmd] = NULL;
	return ret_status;
}

int cd_buildin_cmd() {
	struct stat sb;
	char *dir;
	int ret_status;

	ret_status = SUCCESS;
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
		ret_status |= ERR_CD;
	}
	return ret_status;
}

void exit_buildin_cmd() {
	exit(EXIT_SUCCESS);
}

int fork_and_exec() {
	pid_t child;
	int ret_status;

	ret_status = SUCCESS;

	switch ((child = fork())) {
	case -1: {
		perror("Cannot fork to run command");
		ret_status |= ERR_FORK;
		break;
	}
	case 0: {
		execvp(cmd[0], cmd);
		perror("execvp failed");
		ret_status |= ERR_EXEC;
	}
	default: {
		int status;
		wait(&status);
		/* https://www.geeksforgeeks.org/exit-status-child-process-linux/ */
		if (status) {
			ret_status |= ERR_CHILD_CMD;
			fprintf(stderr, "error: command exited with code %d\n", WEXITSTATUS(status));
		}
	}
	}
	return ret_status;
}

int free_tokens() {
	for (size_t i = 0; i < n_cmd; ++i) {
		free(cmd[i]);
		cmd[i] = NULL;
	}
	return SUCCESS;
}
int main() {
	size_t line_size;
	char *token;
	char *line;
	int len, status;

	line_size = CMD_SIZE;
	line = malloc(CMD_SIZE);
	if (!line) {
		perror("Cannot allocate line buffer");
		exit(EXIT_FAILURE);
	}

	for (;;) {
		status = SUCCESS;
		/* XXX: 0. show the PROMT */
		fprintf(stderr, PROMT);

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

		/* XXX: 2. Extract cmnd and agrs (tokens) */
		status |= get_cmds(line, len - LINE_DELIMITER_LEN); /* Skip the last char with is '\n' */

		if (status & ERR_QUOTE_MISMATCH) {
			fprintf(stderr, "error: mismatched quotes\n");
			free_tokens();
			continue;
		}

		/* XXX: 3. Execute the cmd */
		if (n_cmd == 0) {
			status |= ERR_EMPTY_CMD;
			continue;
		}else if (strcmp(cmd[0], "cd") == 0) { /* Handle cd */
			status |= cd_buildin_cmd();
		} else if (strcmp(cmd[0], "exit") == 0) { /* Handle exit */
			exit_buildin_cmd();
		} else{ /* exec the binary */
			status |= fork_and_exec();
		}

		/* Free tokens */
		free_tokens();
	}

	free(line);
	return 0;
}
