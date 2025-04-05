#include <math.h>

float fvector128_norm(const float vec[128])
{
    float sum = 0;
    for (unsigned int i = 0; i < 128; i++)
    {
        sum += vec[i] * vec[i];
    }
    return sqrtf(sum);
}

void fvector128_normalize(float vec[128])
{
    float rnorm = 1 / fvector128_norm(vec);
    for (unsigned int i = 0; i < 128; i++)
    {
        vec[i] *= rnorm;
    }
}

