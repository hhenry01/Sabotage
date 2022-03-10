#include <stdio.h>
#include <string.h>

int main(void)
{
	FILE *fp;
	fp = fopen("/dev/GPS_UART", "r+");
	char prompt = 0;

	if (fp)
	{
		while (prompt != '$')
		{
			prompt = getc(fp);
		}
		do {
			prompt = getc(fp);
			printf("%c", prompt);
		} while (prompt != '*');
		printf("\n");
	} else {
		printf("Error\n");
	}
	fclose(fp);
	printf("Done\n");
	return 0;
}
