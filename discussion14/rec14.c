#include <stdio.h>
#include <signal.h>

void foo() {
	
	static int count = 0;
	printf("%d: Count = %d\n", time(NULL), ++count);
	sleep(1);
}

int main() {
	
	/* Print infinitely. */
	while (1) foo();
}
