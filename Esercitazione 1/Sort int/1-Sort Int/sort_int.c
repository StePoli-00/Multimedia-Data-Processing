
#include <stdio.h>
#include <stdlib.h>


int* bubble_sort(int* v, int n) {
	int temp;
	for (int i = 0; i < n - 1; ++i)
	{
		for (int j = 0; j < n - i - 1; ++j)
		{
			if (v[j] > v[j + 1]) {

				temp = v[j];
				v[j] = v[j + 1];
				v[j + 1] = temp;
			}

		}

	}
	return v;
}


//int main(int argc, char** argv)
int main(int argc, char** argv)
{
	if (argc != 3)
	{
		return -1;
	}

	FILE* f = fopen(argv[1], "r");
	int* v = malloc(sizeof(int));
	int i = 0, n = 0;

	if (f == NULL)
	{
		return -1;
	}

	while (!feof(f))
	{
		int ret = fscanf(f, "%d", &v[i]);

		if (ret != 0)
		{
			++i;
			v = realloc(v, (i + 1) * sizeof(int));
		}
		else
		{
			break;
		}

	}
	n = i;
	v = bubble_sort(v, n);
	FILE* g = fopen(argv[2], "w");
	for (int i = 0; i < n; ++i) {

		fprintf(g, "%d", v[i]);
		fprintf(g, "\n");

	}
	free(v);
	fclose(f);
	fclose(g);
	return 0;

}