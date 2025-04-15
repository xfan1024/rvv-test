#include <math.h>
#include <riscv_vector.h>

#ifndef VLENB
#define VLENB __riscv_vlenb()
#endif

#ifndef LMUL
#define LMUL 2
#endif

#define _C2(x, y) x##y
#define _C3(x, y, z) x##y##z

#define C2(x, y) _C2(x, y)
#define C3(x, y, z) _C3(x, y, z)

float fvector128_norm(const float vec[128])
{
    unsigned int vl = VLENB / sizeof(float) * LMUL;
    unsigned int loop = 128 / vl;
    C3(vfloat32m,LMUL,_t) vin;
    C3(vfloat32m,LMUL,_t) vresult;

    vresult = C2(__riscv_vfmv_v_f_f32m, LMUL)(0.0f, vl);

    while (loop--)
    {
        vin = C2(__riscv_vle32_v_f32m, LMUL)(vec, vl);
        vresult = C2(__riscv_vfmacc_vv_f32m, LMUL)(vresult, vin, vin, vl);
        vec += vl;
    }

    vfloat32m1_t vsum = __riscv_vfmv_s_f_f32m1(0.0f, 1);
    vsum = C3(__riscv_vfredusum_vs_f32m, LMUL, _f32m1(vresult, vsum, vl));

    return sqrtf(__riscv_vfmv_f_s_f32m1_f32(vsum));
}

void fvector128_normalize(float vec[128])
{
    unsigned int vl = VLENB / sizeof(float) * LMUL;
    unsigned int loop;
    C3(vfloat32m,LMUL,_t) vin;
    C3(vfloat32m,LMUL,_t) vresult;
    float *p;

    vresult = C2(__riscv_vfmv_v_f_f32m, LMUL)(0.0f, vl);

    loop = 128 / vl;
    p = vec;
    while (loop--)
    {
        vin = C2(__riscv_vle32_v_f32m, LMUL)(p, vl);
        vresult = C2(__riscv_vfmacc_vv_f32m, LMUL)(vresult, vin, vin, vl);
        p += vl;
    }

    vfloat32m1_t vtmp = __riscv_vfmv_s_f_f32m1(0.0f, 1);
    vtmp = C3(__riscv_vfredusum_vs_f32m, LMUL, _f32m1)(vresult, vtmp, vl);
    float x = __riscv_vfmv_f_s_f32m1_f32(vtmp);
    vtmp = __riscv_vfrsqrt7_v_f32m1(vtmp, 1);

    float y = __riscv_vfmv_f_s_f32m1_f32(vtmp);
    float rnorm  = y * ( 1.5f - ( x * 0.5f * y * y ) );

    loop = 128 / vl;
    p = vec;
    while (loop--)
    {
        vin = C2(__riscv_vle32_v_f32m, LMUL)(p, vl);
        vresult = C2(__riscv_vfmul_vf_f32m, LMUL)(vin, rnorm, vl);
        C2(__riscv_vse32_v_f32m, LMUL)(p, vresult, vl);
        p += vl;
    }
}

int should_skip()
{
    if (VLENB > __riscv_vlenb())
    {
        return 1;
    }
    return 0;
}
