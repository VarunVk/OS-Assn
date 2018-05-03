#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

int KEEP_RUNNING = 1;

void foo() {

	if (KEEP_RUNNING) {
		static int count = 0;
		printf("%d: Count = %d\n", (int)time(NULL), ++count);
	}
}
void sigIntHandler(int sig)
{
	KEEP_RUNNING = KEEP_RUNNING?0:1;
}
int main() {

	// Add handler for SIGINT
    struct sigaction act;
    act.sa_handler = sigIntHandler;
    sigaction(SIGINT, &act, NULL);

	/* Print infinitely. */
	while (1) foo();
}
