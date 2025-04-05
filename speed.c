#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <math.h>

extern float fvector128_norm(const float vec[128]);
extern void fvector128_normalize(float vec[128]);

uint64_t timespec_diff(const struct timespec *a, const struct timespec *b)
{
    uint64_t res;
    if (a->tv_nsec < b->tv_nsec)
    {
        res = 1000000000 + a->tv_nsec - b->tv_nsec;
        res += 1000000000 * (a->tv_sec - b->tv_sec - 1);
    }
    else
    {
        res = a->tv_nsec - b->tv_nsec;
        res += 1000000000 * (a->tv_sec - b->tv_sec);
    }
    return res;
}

float elapsed_average(const struct timespec *start, const struct timespec *end, unsigned int divisor)
{
    return (float)timespec_diff(end, start) / (float)divisor;
}

float fvector128_init(float vec[128])
{
    uint32_t sum = 0;
    for (unsigned int i = 0; i < 128; i++)
    {
        vec[i] = (float)i;
        sum += i*i;
    }
    return sqrtf((float)sum);
}

void speedtest_fvector128_normalize()
{
    const float allow_error = 0.00001;
    const unsigned int times = 5000000;
    struct timespec start, end;
    float vec[128];
    float norm;
    float error;

    fvector128_init(vec);
    fvector128_normalize(vec);
    error = fvector128_norm(vec) - 1;
    if (!(-allow_error <= error && error <= allow_error))
    {
        goto err;
    }

    clock_gettime(CLOCK_MONOTONIC, &start);

    for (unsigned int i = 0; i < times; i++)
    {
        fvector128_normalize(vec);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    error = fvector128_norm(vec) - 1;
    if (!(-allow_error <= error && error <= allow_error))
    {
        goto err;
    }
    printf("%.1f\n", elapsed_average(&start, &end, times));
    return;

err:
    fprintf(stderr, "error: %f\n", error);
    exit(1);
}

void speedtest_fvector128_norm()
{
    const float allow_error = 0.00001f;
    const unsigned int times = 5000000;
    struct timespec start, end;
    float vec[128];
    float norm;
    float error;

    const float expected = fvector128_init(vec);

    error = fvector128_norm(vec) - expected;
    if (!( -allow_error <= error && error <= allow_error ))
    {
        goto err;
    }

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (unsigned int i = 0; i < times; i++)
    {
        norm = fvector128_norm(vec);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);


    error = fvector128_norm(vec) - 1.0f;
    if ( -allow_error <= error && error <= allow_error )
    {
        goto err;
    }
    printf("%.1f\n", elapsed_average(&start, &end, times));
    return;

err:
    fprintf(stderr, "error: %f\n", error);
    exit(1);
}


int main(int argc, char *argv[])
{
    if (argc < 2 || strcmp(argv[1], "norm") == 0)
    {
        speedtest_fvector128_norm();
    }
    else if (strcmp(argv[1], "normalize") == 0)
    {
        speedtest_fvector128_normalize();
    }
    else
    {
        return 1;
    }
    return 0;
}

