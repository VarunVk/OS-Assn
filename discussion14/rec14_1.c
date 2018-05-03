#include <stdio.h>
#include <signal.h>

void foo() {

	static int count = 0;
	printf("%d: Count = %d\n", time(NULL), ++count);
	sleep(1);
}

int main() {

    /* Block SIGINT*/
    sigset_t old;
    sigset_t new;
    sigemptyset(&new);
    sigaddset(&new, SIGINT);

    if (sigprocmask(SIG_BLOCK, &new, &old) == -1) {
        perror("Error blocking SIGINT");
        exit(1);
    }

	/* Print infinitely. */
	while (1) foo();
}
