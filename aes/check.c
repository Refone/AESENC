#include <stdio.h>
#include <stdint.h>
#include <wmmintrin.h>

int Check_CPU_support_AES();

int main()
{
	if (Check_CPU_support_AES())
	{
		printf("AES instructions set is available.\n");
	}
	else
	{
		printf("AES instructions is not supported on this CPU.\n");
	}
	return 0;
}
