# riscv vector 测试

通过不同的方式实现计算向量L2范数，实现单位化向量

来验证riscv vector扩展带来的性能加速

## 测试结果

**SG2044**

| Executable           | norm  | normalize |
| -------------------- | ----- | --------- |
| c-rv64gc             | 144.8 | 245.9     |
| c-rv64gcv            | 47.5  | 82.2      |
| rvv-m2-rv64gcv       | 42.9  | 65.0      |
| rvv-m2-vl128-rv64gcv | 32.9  | 58.3      |
