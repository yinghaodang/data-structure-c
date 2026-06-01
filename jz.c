#include <stdint.h>
#include <stdio.h>
#include <string.h>

void printFloatBinary(float f) {
    unsigned int *p = (unsigned int *)&f;
    for (int i = 31; i >= 0; i--) {
        printf("%d", (*p >> i) & 1);
        if (i == 31 || i == 23) printf(" ");
    }
    printf("\n");
}

float mathShiftLeftBitLevel(float f, int n) {
    if (n <= 0) return f;

    uint32_t u;
    memcpy(&u, &f, sizeof(f));

    uint32_t sign = u & 0x80000000;
    int exp = (u >> 23) & 0xFF;
    uint32_t frac = u & 0x7FFFFF;

    // 特殊值（NaN / Inf）直接返回
    if (exp == 0xFF) return f;  // NaN or Inf

    for (int i = 0; i < n; i++) {
        if (exp == 0) {
            // 非正规数 或 零
            if (frac == 0) {
                // 0 乘任何次 2 还是 0，直接结束循环
                break;
            }
            // 检查尾数的最高位（第22位，即 0x400000）
            if (frac & 0x400000) {
                // 左移后最高位进入隐含1，变为正规数
                frac = (frac << 1) & 0x7FFFFF;  // 去掉移出的1
                exp = 1;                        // 进入最小正规数
            } else {
                // 仍在非正规数内
                frac <<= 1;
            }
        } else if (exp < 254) {
            // 正规数，指数未满，直接加1
            exp++;
        } else {
            // exp == 254，再左移一次就溢出到无穷大
            exp = 0xFF;
            frac = 0;
            break;  // 之后不再变化（已经是无穷大）
        }
    }

    // 重新组装
    u = sign | ((uint32_t)exp << 23) | frac;
    memcpy(&f, &u, sizeof(f));
    return f;
}

float mathShiftRightBitLevel(float f, int n) {
    if (n <= 0) return f;

    uint32_t u;
    memcpy(&u, &f, sizeof(f));

    uint32_t sign = u & 0x80000000;
    int exp = (u >> 23) & 0xFF;
    uint32_t frac = u & 0x7FFFFF;

    // 特殊值（NaN / Inf / 零）直接返回
    if (exp == 0xFF) return f;            // NaN or Inf
    if (exp == 0 && frac == 0) return f;  // zero

    for (int i = 0; i < n; i++) {
        if (exp > 1) {
            // 正规数，指数足够大，直接减指数
            exp--;
        } else if (exp == 1) {
            // 正规数的最后一次右移，进入非正规数
            // 将隐含的 1 显式地合并到尾数前，形成 24 位有效数
            uint32_t significand = (1u << 23) | frac;
            significand >>= 1;              // 除以 2
            exp = 0;                        // 指数变为 0
            frac = significand & 0x7FFFFF;  // 低 23 位作新尾数
        } else {                            // exp == 0
            // 已经在非正规数内，尾数直接右移
            frac >>= 1;
            if (frac == 0) {
                // 移空则变为零（保留符号位）
                exp = 0;
                break;
            }
        }
    }

    // 重新组装
    u = sign | ((uint32_t)exp << 23) | frac;
    memcpy(&f, &u, sizeof(f));
    return f;
}

int main(void) {
    float y = -126.0f;
    float x = 15.72f;
    printFloatBinary(x);
    printFloatBinary(y);
    printFloatBinary(mathShiftRightBitLevel(x, 3));

    return 0;
}