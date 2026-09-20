#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define coc_c_void 0
#define coc_c_int 0

auto coc_c_print_int = [](auto _) {
    return [](int x) {
        return [=](auto r) {
            printf("%d\n", x);
            return r;
        };
    };
};

template <typename A, typename B>
struct Pair_ { A a; B b; };

auto coc_pair_fst = [](auto x) {
    return [](auto y) {
        return [](auto p) {
            return p.a;
        };
    };
};

auto coc_Pair_snd = [](auto x) {
    return [](auto y) {
        return [](auto p) {
            return p.b;
        };
    };
};

auto coc_Pair = [](auto x) {
    return [](auto y) {
        return 0;
    };
};

auto coc_c_while_loop = [](auto R) {
    return [=](auto x) {
        return [=](auto cond) {
            return [=](auto body) {
                auto y = x;
                while (cond(y)) {
                    y = body(y);
                }
                return y;
            };
        };
    };
};

auto coc_Pair_mk = [](auto x) {
    return [](auto y) {
        return [](auto a) {
            return [=](auto b) {
                return Pair_{a,b};
            };
        };
    };
};

#define coc_c_int_0 0
#define coc_c_int_1 1
#define coc_c_int_32 32
#define coc_c_int_64 64


auto coc_c_int_lt = [](int x) {
    return [=](int y) {
        return x < y;
    };
};

auto coc_c_int_add = [](int x) {
    return [=](int y) {
        return x + y;
    };
};

auto coc_c_run = [](auto f) {
    f(0);
};

#define coc_c_float 0
#define coc_c_float_0 0.0f
#define coc_c_float_1 1.0f
#define coc_c_float_0_25 0.25f


auto coc_c_float_add = [](float x) {
    return [=](float y) {
        return x + y;
    };
};

auto coc_c_float_sub = [](float x) {
    return [=](float y) {
        return x - y;
    };
};

auto coc_c_float_mul = [](float x) {
    return [=](float y) {
        return x * y;
    };
};

auto coc_c_float_sqrt = [](float x) {
    return sqrtf(x);
};

auto coc_c_float_print = [](auto _) {
    return [](float x) {
        return [=](auto r) {
            printf("%f\n", x);
            return r;
        };
    };
};
