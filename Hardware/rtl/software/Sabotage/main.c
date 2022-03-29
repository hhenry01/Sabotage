#include <stdio.h>
#include <string.h>
#include "touchscreen.h"

int main(void)
{
	if (Init_Touch() == -1) printf("Error\n");

	printf("Done\n");
	return 0;
}
