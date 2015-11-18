#ifndef versor_h
#define versor_h

#include <math.h>

#define ONE_DEG_IN_RAD (2.0 * M_PI) / 360.0 // 0.017444444

void quat_to_mat4 (float* m, float* q);
void create_versor (float* q, float a, float x, float y, float z);
void normalise_quat (float* q);
void mult_quat_quat (float* result, float* r, float* s);

#endif
