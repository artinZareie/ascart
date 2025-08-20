#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

int main(int argc, char *argv[])
{
	for (int i = 0; i < argc; i++) {
		printf("FMT: %s\n", argv[i]);
	}

    return 0;
}
