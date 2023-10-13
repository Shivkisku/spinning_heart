#include <math.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

void clearScreen() {
    printf("\033[2J\033[H");
}

int main() {
    // Clear screen and hide cursor
    clearScreen();
    printf("\033[?25l");

    float t = 0;
    while (1) {
        float zb[50 * 20] = {0};
        float maxz = 0, c = cos(t), s = sin(t);
        for (float y = -0.5f; y <= 0.5f; y += 0.02f) {
            // Add beating effect
            float r = 0.4f + 0.05f * pow(0.5f + 0.5f * sin(t * 6 + y * 2), 8);
            for (float x = -0.5f; x <= 0.5f; x += 0.02f) {
                // Heart formula
                float z = -x * x - pow(1.2f * y - fabs(x) * 2 / 3, 2) + r * r;
                if (z < 0)
                    continue;
                z = sqrt(z) / (2 - y);
                for (float tz = -z; tz <= z; tz += z / 6) {
                    // Rotate
                    float nx = x * c - tz * s;
                    float nz = x * s + tz * c;

                    // Add perspective
                    float p = 1 + nz / 2;
                    int vx = lroundf((nx * p + 0.5f) * 40 + 10);
                    int vy = lroundf((-y * p + 0.5f) * 19 + 2);
                    int idx = vx + vy * 50;
                    if (zb[idx] <= nz) {
                        zb[idx] = nz;
                        if (maxz <= nz)
                            maxz = nz;
                    }
                }
            }
        }

        clearScreen();
        for (int i = 0; i < 50 * 20; i++)
            putchar(i % 50 ? " .,-~:;=!*#$@"[lroundf(zb[i] / maxz * 9)] : '\n');

        t += 0.003f;
        usleep(3000);
    }

    return 0;
}
