#include <math.h>
#include <riscv_vector.h>

float fvector128_norm(const float vec[128])
{
    unsigned int vl = __riscv_vlenb() / sizeof(float) * 2;
    unsigned int loop = 128 / vl;
    vfloat32m2_t vin;
    vfloat32m2_t vresult;

    vresult = __riscv_vfmv_v_f_f32m2(0.0f, vl);

    while (loop--)
    {
        vin = __riscv_vle32_v_f32m2(vec, vl);
        vresult = __riscv_vfmacc_vv_f32m2(vresult, vin, vin, vl);
        vec += vl;
    }

    vfloat32m1_t vsum = __riscv_vfmv_s_f_f32m1(0.0f, 1);
    vsum = __riscv_vfredusum_vs_f32m2_f32m1(vresult, vsum, vl);

    return sqrtf(__riscv_vfmv_f_s_f32m1_f32(vsum));
}

void fvector128_normalize(float vec[128])
{
    unsigned int vl = __riscv_vlenb() / sizeof(float) * 2;
    unsigned int loop;
    vfloat32m2_t vin;
    vfloat32m2_t vresult;
    float *p;

    vresult = __riscv_vfmv_v_f_f32m2(0.0f, vl);

    loop = 128 / vl;
    p = vec;
    while (loop--)
    {
        vin = __riscv_vle32_v_f32m2(p, vl);
        vresult = __riscv_vfmacc_vv_f32m2(vresult, vin, vin, vl);
        p += vl;
    }

    vfloat32m1_t vtmp = __riscv_vfmv_s_f_f32m1(0.0f, 1);
    vtmp = __riscv_vfredusum_vs_f32m2_f32m1(vresult, vtmp, vl);
    float x = __riscv_vfmv_f_s_f32m1_f32(vtmp);
    vtmp = __riscv_vfrsqrt7_v_f32m1(vtmp, 1);

    float y = __riscv_vfmv_f_s_f32m1_f32(vtmp);
    float rnorm  = y * ( 1.5f - ( x * 0.5f * y * y ) );

    loop = 128 / vl;
    p = vec;
    while (loop--)
    {
        vin = __riscv_vle32_v_f32m2(p, vl);
        vresult = __riscv_vfmul_vf_f32m2(vin, rnorm, vl);
        __riscv_vse32_v_f32m2(p, vresult, vl);
        p += vl;
    }
}
