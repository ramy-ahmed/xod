#include <emscripten.h>
#include "catch.hpp"

namespace llvm_intrinsics {
    #include <stdio.h>
    #include <sys/types.h>

    extern "C" {
        extern unsigned short llvm_bswap_i16(unsigned short x);
        extern unsigned int llvm_bswap_i32(unsigned int x);
        extern int32_t llvm_ctlz_i8(int8_t x, int izZeroUndef);
        extern int32_t llvm_ctlz_i16(int16_t x, int izZeroUndef);
        extern int32_t llvm_ctlz_i32(int32_t x, int izZeroUndef);
        extern int64_t llvm_ctlz_i64(int64_t x, int izZeroUndef);
        extern int32_t llvm_cttz_i32(int32_t x, int izZeroUndef);
        extern int64_t llvm_cttz_i64(int64_t x, int izZeroUndef);
        extern int32_t llvm_ctpop_i32(int32_t x);
        extern int64_t llvm_ctpop_i64(int64_t x);
        extern int llvm_expect_i32(int x, int y);
        extern float llvm_powi_f32(float x, int32_t y);
        extern double llvm_powi_f64(double x, int32_t y);
        extern float llvm_trunc_f32(float x);
        extern double llvm_trunc_f64(double x);
        extern float llvm_ceil_f32(float x);
        extern double llvm_ceil_f64(double x);
        extern float llvm_floor_f32(float x);
        extern double llvm_floor_f64(double x);
        extern float llvm_cos_f32(float x);
        extern double llvm_cos_f64(double x);
        extern float llvm_sin_f32(float x);
        extern double llvm_sin_f64(double x);
        extern float llvm_exp2_f32(float x);
        extern double llvm_exp2_f64(double x);
        extern float llvm_log2_f32(float x);
        extern double llvm_log2_f64(double x);
        extern float llvm_log10_f32(float x);
        extern double llvm_log10_f64(double x);

        extern float llvm_copysign_f32(float x, float y);
        extern double llvm_copysign_f64(double x, double y);

        extern float llvm_round_f32(float x);
        extern double llvm_round_f64(double x);
        extern float llvm_minnum_f32(float x, float y);
        extern double llvm_minnum_f64(double x, double y);
        extern float llvm_maxnum_f32(float x, float y);
        extern double llvm_maxnum_f64(double x, double y);
        extern float llvm_nearbyint_f32(float x);
        extern double llvm_nearbyint_f64(double x);
    }

    int main(void) {
        unsigned short x = 0xc8ef;
        printf("%x,%x\n", x & 0xff, x >> 8);
        x = llvm_bswap_i16(x);
        printf("%x,%x\n", x & 0xff, x >> 8);

        unsigned int y = 0xc5de158a;
        printf("%x,%x,%x,%x\n", y & 0xff, (y >> 8) & 0xff, (y >> 16) & 0xff,
                (y >> 24) & 0xff);
        y = llvm_bswap_i32(y);
        printf("%x,%x,%x,%x\n", y & 0xff, (y >> 8) & 0xff, (y >> 16) & 0xff,
                (y >> 24) & 0xff);

        printf("%d,%d\n", (int)llvm_ctlz_i64(((int64_t)1) << 40, 0),
                llvm_ctlz_i32(1 << 10, 0));
        printf("%d,%d\n", (int)llvm_cttz_i64(((int64_t)1) << 40, 0),
                llvm_cttz_i32(1 << 10, 0));
        printf("%d,%d\n", (int)llvm_ctpop_i64((0x3101ULL << 32) | 1),
                llvm_ctpop_i32(0x3101));

        printf("llvm_cttz_i32:\n");
        printf("(0, 0)=%d\n", llvm_cttz_i32(0, 0));
        printf("(1, 0)=%d\n", llvm_cttz_i32(1, 0));
        printf("(2, 0)=%d\n", llvm_cttz_i32(2, 0));
        printf("(0x0000FFFF, 0)=%d\n", llvm_cttz_i32(0x0000FFFF, 0));
        printf("(0x7FFF0000, 0)=%d\n", llvm_cttz_i32(0x7FFF0000, 0));
        printf("(0xFFFF0000, 0)=%d\n", llvm_cttz_i32(0xFFFF0000, 0));
        printf("(0x7FFFFFFF, 0)=%d\n", llvm_cttz_i32(0x7FFFFFFF, 0));
        printf("(0xFFFFFFFE, 0)=%d\n", llvm_cttz_i32(0xFFFFFFFE, 0));
        printf("(0xFFFFFFFF, 0)=%d\n", llvm_cttz_i32(0xFFFFFFFF, 0));
        printf("small ctlz: %d,%d\n", (int)llvm_ctlz_i8(2, 0), llvm_ctlz_i16(2, 0));

        printf("llvm_ctpop_i32:\n");
        printf("%d\n", (int)llvm_ctpop_i32(-594093059)); // 22
        printf("%d\n", (int)llvm_ctpop_i32(0xdeadbeef)); // 24
        printf("%d\n", (int)llvm_ctpop_i32(0x00000000)); // 0
        printf("%d\n", (int)llvm_ctpop_i32(0xffffffff)); // 32
        printf("%d\n", (int)llvm_ctpop_i32(0x55555555)); // 16
        printf("%d\n", (int)llvm_ctpop_i32(0xa55a5aa5)); // 16
        printf("%d\n", (int)llvm_ctpop_i32(0xaaaaaaaa)); // 16
        printf("%d\n", (int)llvm_ctpop_i32(0x80000000)); // 1
        printf("%d\n", (int)llvm_ctpop_i32(0x00000001)); // 1
        printf("llvm_expect_i32:\n");
        printf("%d\n", llvm_expect_i32(x % 27, 3));

        int64_t a = 1;
        a = __builtin_bswap64(a);
        printf("%lld\n", a);

        printf("%d\n", (int)llvm_powi_f32(5.0f, 3));
        printf("%d\n", (int)llvm_powi_f64(3.0, 5));
        printf("%d\n", (int)llvm_trunc_f32(18.0987f));
        printf("%d\n", (int)llvm_trunc_f64(-12.42));
        printf("%d\n", (int)llvm_floor_f32(27.665f));
        printf("%d\n", (int)llvm_floor_f64(-8.95));
        printf("%.1f\n", llvm_cos_f32(0.0f * 3.14/180));
        printf("%.1f\n", llvm_cos_f64(180.0 * 3.14/180));
        printf("%.1f\n", llvm_sin_f32(90.0f * 3.14/180));
        printf("%.1f\n", llvm_sin_f64(270.0 * 3.14/180));

        printf("exp2_f32 %.1f\n", llvm_exp2_f32(3));
        printf("exp2_f64 %.1f\n", llvm_exp2_f64(4.5));
        printf("log2_f32 %.1f\n", llvm_log2_f32(16));
        printf("log2_f64 %.1f\n", llvm_log2_f64(20));
        printf("log10_f32 %.1f\n", llvm_log10_f32(1000));
        printf("log10_f64 %.1f\n", llvm_log10_f64(2000));

        printf("llvm_ceil_f32 %.1f\n", llvm_ceil_f32(1.4f));
        printf("llvm_ceil_f32 %.1f\n", llvm_ceil_f32(1.5f));
        printf("llvm_ceil_f32 %.1f\n", llvm_ceil_f32(1.6f));
        printf("llvm_ceil_f32 %.1f\n", llvm_ceil_f32(-1.4f));
        printf("llvm_ceil_f32 %.1f\n", llvm_ceil_f32(-1.5f));
        printf("llvm_ceil_f32 %.1f\n", llvm_ceil_f32(-1.6f));

        printf("llvm_ceil_f64 %.1f\n", llvm_ceil_f64(1.4));
        printf("llvm_ceil_f64 %.1f\n", llvm_ceil_f64(1.5));
        printf("llvm_ceil_f64 %.1f\n", llvm_ceil_f64(1.6));
        printf("llvm_ceil_f64 %.1f\n", llvm_ceil_f64(-1.4));
        printf("llvm_ceil_f64 %.1f\n", llvm_ceil_f64(-1.5));
        printf("llvm_ceil_f64 %.1f\n", llvm_ceil_f64(-1.6));

        printf("llvm_floor_f32 %.1f\n", llvm_floor_f32(1.4f));
        printf("llvm_floor_f32 %.1f\n", llvm_floor_f32(1.5f));
        printf("llvm_floor_f32 %.1f\n", llvm_floor_f32(1.6f));
        printf("llvm_floor_f32 %.1f\n", llvm_floor_f32(-1.4f));
        printf("llvm_floor_f32 %.1f\n", llvm_floor_f32(-1.5f));
        printf("llvm_floor_f32 %.1f\n", llvm_floor_f32(-1.6f));

        printf("llvm_floor_f64 %.1f\n", llvm_floor_f64(1.4));
        printf("llvm_floor_f64 %.1f\n", llvm_floor_f64(1.5));
        printf("llvm_floor_f64 %.1f\n", llvm_floor_f64(1.6));
        printf("llvm_floor_f64 %.1f\n", llvm_floor_f64(-1.4));
        printf("llvm_floor_f64 %.1f\n", llvm_floor_f64(-1.5));
        printf("llvm_floor_f64 %.1f\n", llvm_floor_f64(-1.6));

        printf("llvm_round_f64 %.1f\n", llvm_round_f64(20.49));
        printf("llvm_round_f64 %.1f\n", llvm_round_f64(20.5));
        printf("llvm_round_f64 %.1f\n", llvm_round_f64(42));
        printf("llvm_round_f64 %.1f\n", llvm_round_f64(-20.49));
        printf("llvm_round_f64 %.1f\n", llvm_round_f64(-20.5));
        printf("llvm_round_f64 %.1f\n", llvm_round_f64(-20.51));

        printf("llvm_round_f32 %.1f\n", llvm_round_f32(20.49));
        printf("llvm_round_f32 %.1f\n", llvm_round_f32(20.5));
        printf("llvm_round_f32 %.1f\n", llvm_round_f32(42));
        printf("llvm_round_f32 %.1f\n", llvm_round_f32(-20.49));
        printf("llvm_round_f32 %.1f\n", llvm_round_f32(-20.5));
        printf("llvm_round_f32 %.1f\n", llvm_round_f32(-20.51));

        printf("llvm_nearbyint_f64 %.1f\n", llvm_nearbyint_f64(20.50));
        printf("llvm_nearbyint_f64 %.1f\n", llvm_nearbyint_f64(20.51));
        printf("llvm_nearbyint_f64 %.1f\n", llvm_nearbyint_f64(42));
        printf("llvm_nearbyint_f64 %.1f\n", llvm_nearbyint_f64(-20.49));
        printf("llvm_nearbyint_f64 %.1f\n", llvm_nearbyint_f64(-20.5));
        printf("llvm_nearbyint_f64 %.1f\n", llvm_nearbyint_f64(-20.51));

        printf("llvm_nearbyint_f32 %.1f\n", llvm_nearbyint_f32(20.50));
        printf("llvm_nearbyint_f32 %.1f\n", llvm_nearbyint_f32(20.51));
        printf("llvm_nearbyint_f32 %.1f\n", llvm_nearbyint_f32(42));
        printf("llvm_nearbyint_f32 %.1f\n", llvm_nearbyint_f32(-20.49));
        printf("llvm_nearbyint_f32 %.1f\n", llvm_nearbyint_f32(-20.5));
        printf("llvm_nearbyint_f32 %.1f\n", llvm_nearbyint_f32(-20.51));

        printf("llvm_minnum_f32 %.1f\n", llvm_minnum_f32(5.7, 10.2));
        printf("llvm_minnum_f32 %.1f\n", llvm_minnum_f32(8.5, 2.3));
        printf("llvm_minnum_f64 %.1f\n", llvm_minnum_f64(5.7, 10.2));
        printf("llvm_minnum_f64 %.1f\n", llvm_minnum_f64(8.5, 2.3));

        printf("llvm_maxnum_f32 %.1f\n", llvm_maxnum_f32(5.7, 10.2));
        printf("llvm_maxnum_f32 %.1f\n", llvm_maxnum_f32(8.5, 2.3));
        printf("llvm_maxnum_f64 %.1f\n", llvm_maxnum_f64(5.7, 10.2));
        printf("llvm_maxnum_f64 %.1f\n", llvm_maxnum_f64(8.5, 2.3));

        printf("llvm_copysign_f32 %.1f\n", llvm_copysign_f32(-1.2, 3.4));
        printf("llvm_copysign_f32 %.1f\n", llvm_copysign_f32(5.6, -7.8));
        printf("llvm_copysign_f32 %.1f\n", llvm_copysign_f32(-1.3, -2.4));
        printf("llvm_copysign_f32 %.1f\n", llvm_copysign_f32(5.7, 6.8));
        printf("llvm_copysign_f64 %.1f\n", llvm_copysign_f64(-1.2, 3.4));
        printf("llvm_copysign_f64 %.1f\n", llvm_copysign_f64(5.6, -7.8));
        printf("llvm_copysign_f64 %.1f\n", llvm_copysign_f64(-1.3, -2.4));
        printf("llvm_copysign_f64 %.1f\n", llvm_copysign_f64(5.7, 6.8));

        printf("ok.\n");

        return 1;
    }
}

namespace llvm_fabs {
    #include <math.h>
    #include <stdio.h>
    #include <float.h>
    #include <emscripten/emscripten.h>

    float __attribute__((noinline)) negative10()
    {
        return (emscripten_random() < -1) ? 0.f : -10.5f;
    }

    float __attribute__((noinline)) positive42()
    {
        return (emscripten_random() < -1) ? 0.f : 42.25f;
    }

    double __attribute__((noinline)) negative_dbl_max()
    {
        return (emscripten_random() < -1) ? 0.f : -DBL_MAX;
    }

    int main()
    {
        printf("%f\n", __builtin_fabsf(negative10()));
        printf("%f\n", __builtin_fabsf(positive42()));
        printf("%f\n", __builtin_fabsf((float)negative_dbl_max()));

        printf("%f\n", __builtin_fabs(negative10()));
        printf("%f\n", __builtin_fabs(positive42()));
        printf("%f\n", __builtin_fabs(negative_dbl_max()));
        return 1;
    }
}

namespace llvmswitch {
    #include <stdio.h>
    #include <string.h>

    int switcher(int p) {
    switch (p) {
        case 'a':
        case 'b':
        case 'c':
        return p - 1;
        case -15:
        return p + 1;
    }
    return p;
    }

    int main(int argc, const char argv[]) {
        unsigned int x = 0xfffffff1;
        x >>= (argc - 1);  // force it to be unsigned for purpose of checking our
                            // switch comparison in signed/unsigned
        printf("*%d,%d,%d,%d,%d,%d*\n", switcher('a'), switcher('b'), switcher('c'),
                switcher(x), switcher(-15), switcher('e'));
        return 1;
    }
}

namespace atomic_cxx {
    #include <atomic>
    #include <cstdio>

    template<typename TYPE, typename UNSIGNED_TYPE> void test(TYPE mask0, TYPE mask1, TYPE mask2) {
        typedef TYPE dog;
        
        const TYPE numMemoryOrders = 6;
        std::memory_order memoryOrder[numMemoryOrders] = {
            std::memory_order_relaxed,
            std::memory_order_consume,
            std::memory_order_acquire,
            std::memory_order_release,
            std::memory_order_acq_rel,
            std::memory_order_seq_cst,
        };

        // test atomic<int>
        std::atomic<dog> atomicDog(5);
        printf("atomic<int>.is_lock_free(): %s\n", atomicDog.is_lock_free() ? "true" : "false");
        printf("atomic<int> value: %lld\n", (long long)TYPE(atomicDog));

        // test store/load
        for (TYPE i = 0; i < numMemoryOrders; i++) {
            atomicDog.store(i, memoryOrder[i]);
            printf("store/load %lld: %lld\n", (long long)i, (long long)atomicDog.load(memoryOrder[i]));
        }

        // test exchange
        for (TYPE i = 0; i < numMemoryOrders; i++) {
            TYPE old = atomicDog.exchange(i, memoryOrder[i]);
            printf("exchange %lld: old=%lld new=%lld\n", (long long)i, (long long)old, (long long)TYPE(atomicDog));
        }

        // compare_exchange_weak
        for (TYPE i = 0; i < numMemoryOrders; i++) {
            bool success = atomicDog.compare_exchange_weak(i, i + 1, memoryOrder[i], memoryOrder[i]);
            printf("compare_exchange_weak %lld: success = %s\n", (long long)i, success ? "true" : "false");
        }

        // compare_exchange_strong
        for (TYPE i = 0; i < numMemoryOrders; i++) {
            bool success = atomicDog.compare_exchange_strong(i, i + 1, memoryOrder[i], memoryOrder[i]);
            printf("compare_exchange_strong %lld: success = %s\n", (long long)i, success ? "true" : "false");
        }

        // fetch_add
        atomicDog = mask2;
        for (TYPE i = 0; i < numMemoryOrders; i++) {
            TYPE old = atomicDog.fetch_add(1, memoryOrder[i]);
            printf("fetch_add %lld: old=%llx new=%llx\n", (long long)i, (long long)old, (long long)TYPE(atomicDog));
        }

        // fetch_sub
        for (TYPE i = 0; i < numMemoryOrders; i++) {
            TYPE old = atomicDog.fetch_sub(1, memoryOrder[i]);
            printf("fetch_sub %lld: old=%llx new=%llx\n", (long long)i, (long long)old, (long long)TYPE(atomicDog));
        }

        // fetch_and
        for (TYPE i = 0; i < numMemoryOrders; i++) {
            atomicDog.store(mask0, memoryOrder[i]);
            TYPE old = atomicDog.fetch_and((1<<i), memoryOrder[i]);
            printf("fetch_and %lld: old=%llx, new=%llx\n", (long long)i, (unsigned long long)UNSIGNED_TYPE(old), (unsigned long long)UNSIGNED_TYPE(atomicDog));
        }

        // fetch_or
        atomicDog = 0;
        for (TYPE i = 0; i < numMemoryOrders; i++) {
            TYPE old = atomicDog.fetch_or((1<<i), memoryOrder[i]);
            printf("fetch_or %lld: old=%llx, new=%llx\n", (long long)i, (unsigned long long)UNSIGNED_TYPE(old), (unsigned long long)UNSIGNED_TYPE(atomicDog));
        }

        // fetch_xor
        atomicDog = 0;
        for (int i = 0; i < numMemoryOrders; i++) {
            int old = atomicDog.fetch_xor((1<<i), memoryOrder[i]);
            printf("fetch_xor %lld: old=%llx, new=%llx\n", (long long)i, (unsigned long long)UNSIGNED_TYPE(old), (unsigned long long)UNSIGNED_TYPE(atomicDog));
        }

        // operator++, --
        atomicDog = 0;
        atomicDog++;
        printf("operator++: %lld\n", (long long)TYPE(atomicDog));
        atomicDog--;
        printf("operator--: %lld\n", (long long)TYPE(atomicDog));

        // operator +=, -=, &=, |=, ^=
        atomicDog += 10;
        printf("operator+=: %lld\n", (long long)TYPE(atomicDog));
        atomicDog -= 5;
        printf("operator-=: %lld\n", (long long)TYPE(atomicDog));
        atomicDog |= mask0;
        printf("operator|=: %llx\n", (unsigned long long)UNSIGNED_TYPE(atomicDog));
        atomicDog &= mask1;
        printf("operator&=: %llx\n", (unsigned long long)UNSIGNED_TYPE(atomicDog));
        atomicDog ^= mask2;
        printf("operator^=: %llx\n", (unsigned long long)UNSIGNED_TYPE(atomicDog));

    }

    int main() {

        // test 8, 16, 32 and 64-bit data types
        printf("\n8 bits\n\n");
        test<char, unsigned char>(0xFF, 0xF0, 0x0F);
        printf("\n16 bits\n\n");
        test<short, unsigned short>(0xFFFF, 0xF0F0, 0x0F0F);
        printf("\n32 bits\n\n");
        test<int, unsigned int>(0xFFFFFFFF, 0xF0F0F0F0, 0x0F0F0F0F);
        printf("\n64 bits\n\n");
        test<long long, unsigned long long>(0xFFFFFFFFFFFFFFFF, 0xF0F0F0F0F0F0F0F0, 0x0F0F0F0F0F0F0F0F);

        // test atomic_flag (should also have memory_orders, but probably doesn't matter 
        // to find the missing atomic functions)
        std::atomic_flag af;
        af.clear();
        bool b = af.test_and_set();
        printf("atomic_flag: %s\n", b ? "true" : "false");

        printf("done.\n");
        return 1;
    }
}

namespace strftime_time {
    #include <time.h>
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>

    void test(int result, const char* comment, const char* parsed = "") {
        printf("%s: %d\n", comment, result);
        if (!result) {
            printf("\nERROR: %s (\"%s\")\n", comment, parsed);
        }
    }

    int cmp(const char* s1, const char* s2) {
        for (; *s1 == *s2; s1++, s2++) {
            if (*s1 == '\0') break;
        }

        return (*s1 - *s2);
    }

    int main() {
        struct tm tm;
        char s[1000];
        size_t size;

        tm.tm_sec = 4;
        tm.tm_min = 23;
        tm.tm_hour = 20;
        tm.tm_mday = 21;
        tm.tm_mon = 1;
        tm.tm_year = 74;
        tm.tm_wday = 4;
        tm.tm_yday = 51;
        tm.tm_isdst = 0;

        size = strftime(s, 1000, "", &tm);
        test((size == 0) && (*s == '\0'), "strftime test #1", s);

        size = strftime(s, 1000, "%a", &tm);
        test((size == 3) && !cmp(s, "Thu"), "strftime test #2", s);

        size = strftime(s, 1000, "%A", &tm);
        test((size == 8) && !cmp(s, "Thursday"), "strftime test #3", s);

        size = strftime(s, 1000, "%b", &tm);
        test((size == 3) && !cmp(s, "Feb"), "strftime test #4", s);

        size = strftime(s, 1000, "%B", &tm);
        test((size == 8) && !cmp(s, "February"), "strftime test #5", s);

        size = strftime(s, 1000, "%d", &tm);
        test((size == 2) && !cmp(s, "21"), "strftime test #6", s);

        size = strftime(s, 1000, "%H", &tm);
        test((size == 2) && !cmp(s, "20"), "strftime test #7", s);

        size = strftime(s, 1000, "%I", &tm);
        test((size == 2) && !cmp(s, "08"), "strftime test #8", s);

        size = strftime(s, 1000, "%j", &tm);
        test((size == 3) && !cmp(s, "052"), "strftime test #9", s);

        size = strftime(s, 1000, "%m", &tm);
        test((size == 2) && !cmp(s, "02"), "strftime test #10", s);

        size = strftime(s, 1000, "%M", &tm);
        test((size == 2) && !cmp(s, "23"), "strftime test #11", s);

        size = strftime(s, 1000, "%p", &tm);
        test((size == 2) && !cmp(s, "PM"), "strftime test #12", s);

        size = strftime(s, 1000, "%S", &tm);
        test((size == 2) && !cmp(s, "04"), "strftime test #13", s);

        size = strftime(s, 1000, "%U", &tm);
        test((size == 2) && !cmp(s, "07"), "strftime test #14", s);

        size = strftime(s, 1000, "%w", &tm);
        test((size == 1) && !cmp(s, "4"), "strftime test #15", s);

        size = strftime(s, 1000, "%W", &tm);
        test((size == 2) && !cmp(s, "07"), "strftime test #16", s);

        size = strftime(s, 1000, "%y", &tm);
        test((size == 2) && !cmp(s, "74"), "strftime test #17", s);

        size = strftime(s, 1000, "%Y", &tm);
        test((size == 4) && !cmp(s, "1974"), "strftime test #18", s);

        size = strftime(s, 1000, "%%", &tm);
        test((size == 1) && !cmp(s, "%"), "strftime test #19", s);

        size = strftime(s, 5, "%Y", &tm);
        test((size == 4) && !cmp(s, "1974"), "strftime test #20", s);

        size = strftime(s, 4, "%Y", &tm);
        test((size == 0), "strftime test #21", s);

        tm.tm_mon = 0;
        tm.tm_mday = 1;
        size = strftime(s, 10, "%U", &tm);
        test((size == 2) && !cmp(s, "00"), "strftime test #22", s);

        size = strftime(s, 10, "%W", &tm);
        test((size == 2) && !cmp(s, "00"), "strftime test #23", s);

        // 1/1/1973 was a Sunday and is in CW 1
        tm.tm_year = 73;
        size = strftime(s, 10, "%W", &tm);
        test((size == 2) && !cmp(s, "01"), "strftime test #24", s);

        // 1/1/1978 was a Monday and is in CW 1
        tm.tm_year = 78;
        size = strftime(s, 10, "%U", &tm);
        test((size == 2) && !cmp(s, "01"), "strftime test #25", s);

        // 2/1/1999
        tm.tm_year = 99;
        tm.tm_yday = 1;
        size = strftime(s, 10, "%G (%V)", &tm);
        test((size == 9) && !cmp(s, "1998 (53)"), "strftime test #26", s);

        size = strftime(s, 10, "%g", &tm);
        test((size == 2) && !cmp(s, "98"), "strftime test #27", s);

        // 30/12/1997
        tm.tm_year = 97;
        tm.tm_yday = 363;
        size = strftime(s, 10, "%G (%V)", &tm);
        test((size == 9) && !cmp(s, "1998 (01)"), "strftime test #28", s);

        size = strftime(s, 10, "%g", &tm);
        test((size == 2) && !cmp(s, "98"), "strftime test #29", s);

        // timezones
        time_t xmas2002 = 1040786563ll;
        time_t summer2002 = 1025528525ll;
        localtime_r(&summer2002, &tm);
        int ahead = timegm(&tm) >= summer2002;
        size = strftime(s, 50, "%z", &tm);
        test((size == 5) && strchr(s, ahead ? '+' : '-'), "strftime zone test #1", s);
        size = strftime(s, 50, "%Z", &tm);
        test(strcmp(s, tzname[tm.tm_isdst]) == 0, "strftime zone test #2", s);

        localtime_r(&xmas2002, &tm);
        ahead = timegm(&tm) >= xmas2002;
        size = strftime(s, 50, "%z", &tm);
        test((size == 5) && strchr(s, ahead ? '+' : '-'), "strftime zone test #3", s);
        size = strftime(s, 50, "%Z", &tm);
        test(strcmp(s, tzname[tm.tm_isdst]) == 0, "strftime zone test #4", s);

        // AM/PM
        tm.tm_sec = 0;
        tm.tm_min = 0;

        tm.tm_hour = 0;
        size = strftime(s, 10, "%I %p", &tm);
        test(!cmp(s, "12 AM"), "strftime test #32", s);

        tm.tm_hour = 12;
        size = strftime(s, 10, "%I %p", &tm);
        test(!cmp(s, "12 PM"), "strftime test #33", s);

        tm.tm_min = 1;

        tm.tm_hour = 0;
        size = strftime(s, 10, "%I %M %p", &tm);
        test(!cmp(s, "12 01 AM"), "strftime test #34", s);

        tm.tm_hour = 12;
        size = strftime(s, 10, "%I %M %p", &tm);
        test(!cmp(s, "12 01 PM"), "strftime test #35", s);

        return 1;
    }
}

namespace unary_literal {
    #include <cstdio>

    class UnaryTest {
        public:
            static const UnaryTest STATIC_INSTANCE;
            static const double STATIC_PROPERTY;

        double p;

        UnaryTest(double p);
    };

    const UnaryTest UnaryTest::STATIC_INSTANCE(-1);
    const double UnaryTest::STATIC_PROPERTY(-1);
    const double STATIC_DOUBLE(-1);

    UnaryTest::UnaryTest(double inP) : p(inP) { }

    int main(int, char){
        const double t1 = -1;
        double t2 = -1;

        double p1 = -UnaryTest::STATIC_INSTANCE.p;
        double p2 = -UnaryTest::STATIC_PROPERTY;
        double p3 = -STATIC_DOUBLE;
        double p4 = -t1;
        double p5 = -t2;

        printf("%.2f\n", p1);
        printf("%.2f\n", p2);
        printf("%.2f\n", p3);
        printf("%.2f\n", p4);
        printf("%.2f\n", p5);

        return 1;
    }
}

TEST_CASE("@/bigRuntime") {
    setup();
    REQUIRE(llvm_intrinsics::main());
    REQUIRE(llvm_fabs::main());
    char llvmargv = {'a'};
    REQUIRE(llvmswitch::main(1, &llvmargv));
    REQUIRE(atomic_cxx::main());
    REQUIRE(strftime_time::main());
    REQUIRE(unary_literal::main(1, 'a'));
}
