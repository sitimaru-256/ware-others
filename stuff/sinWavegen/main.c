#include <stdio.h>
#include <stdint.h>
#include <math.h>
#define PI2 6.2831853
int max;
int length;
float rad;
float phase;
float est;
int main(void) {
    printf("maximum value(uint16>32767)\n");
    scanf("%d", &max);
    printf("element length\n");
    scanf("%d", &length);
    printf("start phase(degree)\n");
    scanf("%f", &phase);

    FILE *file;
    file = fopen("data.txt", "w");
    if (file == NULL) {
        return 1;
    }
    phase = phase * PI2 / 360;
    for(int i = 0; i < length-1; i++){
        rad = PI2 * (float)i / (float)length + phase;
        est = sin(rad);
        est = (uint16_t)((est + 1.0) * max * 0.5);
        fprintf(file, "%g,", est);
    }
    rad = PI2 * (float)(length-1) / (float)length + phase;
    est = sin(rad);
    est = (uint16_t)((est + 1.0) * max * 0.5);
    fprintf(file, "%g",est);
    fclose(file);

    return 0;
}
