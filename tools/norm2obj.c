#include <stdio.h>

int main(int argc, char **argv) {
	int nb_tri;
	scanf("%d", &nb_tri);

	printf("# convert with %d triangles\n", nb_tri);

	for (int i = 0; i < nb_tri; i ++) {
		for (int j = 0; j < 3; j ++) {
			float x, y, z;
			float nx, ny, nz;

			scanf("%f %f %f", &x, &y, &z);
			scanf("%f %f %f", &nx, &ny, &nz);

			printf("v  %f %f %f\n", x, y, z);
			printf("vn %f %f %f\n", nx, ny, nz);
		}

		int fa = i*3+1;
		int fb = i*3+2;
		int fc = i*3+3;
		printf("f  %d/0/%d %d/0/%d %d/0/%d\n", fa, fa, fb, fb, fc, fc);
	}
}
