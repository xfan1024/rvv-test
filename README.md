# riscv vector 测试

通过不同的方式实现计算向量L2范数，实现单位化向量

来验证riscv vector扩展带来的性能加速

## 测试结果

**SG2044**

- Operation System: openEuler 24.03 (LTS)
- Compiler: clang version 17.0.6 ( 17.0.6-16.oe2403)

| Executable   | norm       | normalize  |
| ------------ | ---------- | ---------- |
| c-rvv-off    | 202.7      | 345.1      |
| c-rvv-on     | 67.1       | 114.0      |
| rvv-m1       | 81.1       | 138.3      |
| rvv-m1-vl128 | 71.8       | 118.1      |
| rvv-m1-vl256 | skip       | skip       |
| rvv-m2       | 60.1       | 91.0       |
| rvv-m2-vl128 | 46.1       | 81.6       |
| rvv-m2-vl256 | skip       | skip       |
| rvv-m4       | 66.6       | 96.6       |
| rvv-m4-vl128 | 45.8       | 81.6       |
| rvv-m4-vl256 | skip       | skip       |
| rvv-m8       | 88.3       | 119.6      |
| rvv-m8-vl128 | 205.9      | 248.3      |
| rvv-m8-vl256 | skip       | skip       |


**Spacemit(R) X60**

- Operation System: Bianbu 2.1.1
- Compiler: Bianbu clang version 18.1.8 (11bb4)

| Executable   | norm       | normalize  |
| ------------ | ---------- | ---------- |
| c-rvv-off    | 408.3      | 906.9      |
| c-rvv-on     | 64.6       | 147.9      |
| rvv-m1       | 84.1       | 173.6      |
| rvv-m1-vl128 | 115.3      | 330.9      |
| rvv-m1-vl256 | 65.3       | 168.0      |
| rvv-m2       | 65.2       | 139.1      |
| rvv-m2-vl128 | 81.6       | 231.1      |
| rvv-m2-vl256 | 48.9       | 129.4      |
| rvv-m4       | 69.6       | 119.1      |
| rvv-m4-vl128 | 82.5       | 191.1      |
| rvv-m4-vl256 | 52.8       | 115.4      |
| rvv-m8       | 85.9       | 129.0      |
| rvv-m8-vl128 | 290.4      | 389.5      |
| rvv-m8-vl256 | 70.6       | 124.0      |
