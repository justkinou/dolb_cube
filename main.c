#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <unistd.h>

#define SIDE_INCREMENT	0.5
#define SIDE_LENGTH	50
#define ZOOM_LEVEL	10
#define DISTANCE	80
#define SCREEN_W	80
#define SCREEN_H	24

static char buffer[SCREEN_H][SCREEN_W];
static double A = 0, B = 0, C = 0;

void clear_terminal() { printf("\e[1;1H\e[2J"); }
void reset_buffer() {
	for (int i = 0; i < SCREEN_H; ++i) {
		for (int j = 0; j < SCREEN_W; ++j) {
			*(*(buffer + i) + j) = ' ';
		}
	}
}
double rotate_x(double x, double y, double z) {
	return x * cos(B) * cos(C) +
		y * (cos(A) * sin(B) * sin(C) - sin(A) * cos(C)) +
		z * (cos(A) * sin(B) * cos(C) + sin(A) * sin(C));
}
double rotate_y(double x, double y, double z) {
	return x * sin(A) * cos(B) +
		y * (sin(A) * sin(B) * sin(C) + cos(A) * cos(C)) +
		z * (sin(A) * sin(B) * cos(C) - cos(A) * sin(C));
}
double rotate_z(double x, double y, double z) {
	return x * -sin(B) +
		y * cos(B) * sin(C) +
		z * cos(B) * cos(C);
}
void draw_point(double x, double y, double z, char character) {
	double rx = rotate_x(x, y, z);
	double ry = rotate_y(x, y, z);
	double rz = rotate_z(x, y, z) + DISTANCE;

	if (rz < 0) {
		return;
	}

	int screen_x = (int) (SCREEN_W / 2 + (rx * ZOOM_LEVEL) / (ZOOM_LEVEL + DISTANCE + rz));
	int screen_y = (int) (SCREEN_H / 2 + (ry * ZOOM_LEVEL) / (ZOOM_LEVEL + DISTANCE + rz));
	if (screen_x < 0 || screen_x >= SCREEN_W || screen_y < 0 || screen_y >= SCREEN_H) {
		return;
	}

	*(*(buffer + screen_y) + screen_x) = character;
}

int main() {
	while (1) {
		clear_terminal();
		reset_buffer();

		for (double x = -SIDE_LENGTH; x < SIDE_LENGTH; x += SIDE_INCREMENT) {
			for (double y = -SIDE_LENGTH; y < SIDE_LENGTH; y += SIDE_INCREMENT) {
				int is_edge = x + SIDE_INCREMENT >= SIDE_LENGTH || x == -SIDE_LENGTH ||
						y + SIDE_INCREMENT >= SIDE_LENGTH || y == -SIDE_LENGTH;
				if (!is_edge) {
					continue;
				}
				draw_point(x, y, SIDE_LENGTH, '#');
				draw_point(SIDE_LENGTH, y, x, '#');
				draw_point(-SIDE_LENGTH, y, x, '#');
				draw_point(x, SIDE_LENGTH, y, '#');
				draw_point(x, -SIDE_LENGTH, y, '#');
				draw_point(x, y, -SIDE_LENGTH, '#');
			}
		}

		for (int i = 0; i < SCREEN_H; ++i) {
			for (int j = 0; j < SCREEN_W; ++j) {
				printf("%c", *(*(buffer + i) + j));
			}
			printf("\n");
		}

		A += 0.05;
		B += 0.05;
		C += 0.01;

		usleep(1000000);
	}

	return 0;
}
