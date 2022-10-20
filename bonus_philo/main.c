#include "philo_bonus.h"

int main(int ac, char **av)
{
	int i;
	int status;
	int b = 0;

	i = 0;
	while (i < ac)
	{
		if (fork() == 0)
		{
			while (b < 10)
			{
				printf("child %d -- b = %d\n", i, );
				b++;
			}
			exit(0);
		}
		i++;
	}
	while (waitpid(-1, &status, 0) > 0)
	{
	}
}