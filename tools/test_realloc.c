#include <stdlib.h>

int main(int argc, char **argv) {
	float *pf = NULL;
	for (int i = 1; i < 10; i ++) {
		pf = realloc(pf, i * sizeof(float) * 3);
	}

	pf = NULL;
	for (int i = 1; i < 6; i ++) {
		pf = realloc(pf, i * sizeof(float) * 3);
	}

	pf = NULL;
	for (int i = 1; i < 150; i ++) {
		pf = realloc(pf, i * sizeof(float) * 3);
	}
}
