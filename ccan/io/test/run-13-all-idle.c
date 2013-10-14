#include <ccan/io/io.h>
/* Include the C files directly. */
#include <ccan/io/poll.c>
#include <ccan/io/io.c>
#include <ccan/tap/tap.h>
#include <sys/wait.h>
#include <stdio.h>
#include <signal.h>

static struct io_plan start(struct io_conn *conn, void *unused)
{
	return io_idle();
}

int main(void)
{
	int status;

	plan_tests(3);

	if (fork() == 0) {
		int fds[2];

		ok1(pipe(fds) == 0);
		io_new_conn(fds[0], start, NULL, NULL);
		io_loop();
		exit(1);
	}

	ok1(wait(&status) != -1);
	ok1(WIFSIGNALED(status));
	ok1(WTERMSIG(status) == SIGABRT);

	/* This exits depending on whether all tests passed */
	return exit_status();
}
