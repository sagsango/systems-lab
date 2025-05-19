void daemonize() {
    pid_t pid;

    // Step 1: Fork the parent process
    // This is done so the child is not a process group leader,
    // which is required before we can call setsid().
    pid = fork();

    if (pid < 0) {
        // Fork failed: print error and exit
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        // Parent process: exit immediately so child can run in background
        // This also ensures the shell returns immediately after launching the daemon.
        printf("Daemon PID: %d\n", pid);
        exit(EXIT_SUCCESS);
    }

    // Step 2: Create a new session using setsid()
    // This makes the process the leader of a new session,
    // detaches it from the controlling terminal,
    // and makes it immune to terminal-generated signals (like SIGHUP).
    if (setsid() < 0) {
        perror("setsid");
        exit(EXIT_FAILURE);
    }

    // Step 3: Ignore certain signals
    // SIGHUP: Hangup signal (sent when terminal closes) — we ignore it
    // SIGCHLD: Sent to parent when a child terminates — we ignore it to avoid zombies
    signal(SIGHUP, SIG_IGN);
    signal(SIGCHLD, SIG_IGN);

    // Step 4: Fork again to ensure the daemon cannot regain a controlling terminal
    // The second child is not a session leader, so it can't accidentally reacquire a TTY.
    pid = fork();
    if (pid < 0) {
        perror("second fork");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        // First child exits — only second child (real daemon) continues
        exit(EXIT_SUCCESS);
    }

    // Step 5: Reset file mode creation mask (umask)
    // This allows the daemon to create files with desired permissions.
    // A umask of 0 gives full access (0666 for files, 0777 for dirs),
    // so permissions must be explicitly set when creating files.
    umask(0);

    // Step 6: Change working directory to root (/)
    // This prevents the daemon from locking the current working directory,
    // which could interfere with unmounting filesystems.
    if (chdir("/") < 0) {
        perror("chdir");
        exit(EXIT_FAILURE);
    }

    // Step 7: Close all standard file descriptors
    // Daemons should not be attached to a terminal or inherit standard I/O
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // Optional: Redirect stdin, stdout, stderr to /dev/null
    // This prevents accidental reads/writes to the terminal
    // and ensures file descriptor 0, 1, 2 are valid.
    open("/dev/null", O_RDONLY); // stdin -> /dev/null
    open("/dev/null", O_WRONLY); // stdout -> /dev/null
    open("/dev/null", O_RDWR);   // stderr -> /dev/null
}

