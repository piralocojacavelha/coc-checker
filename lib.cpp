#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define coc_c_void 0
#define coc_c_bool 0
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

struct Empty {};

template <typename B>
struct Pair_<B, Empty> {
    B a;
    Empty b;

    template <typename A>
    operator Pair_<B, A>() const {
        return {a, {}};
    }
};

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

auto coc_c_if = [](auto R) {
    return [=](bool x) {
        return [=](auto a) {
            return [=](auto b) {
                return x ? a : b;
            };
        };
    };
};

auto coc_opt_match = [](auto R) {
    return [](auto x) {
        return [=](auto x) {
            return [=](auto none) {
                return [=](auto some) {
                    if (x.a) {
                        return some(x.b);
                    }
                    return (decltype(some(x.b)))none;
                };
            };
        };
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

auto coc_some_opt = [](auto x) {
    return [](auto a) {
        return Pair_{true,a};
    };
};

auto coc_nil_opt = [](auto x) {
    return Pair_{false,Empty{}};
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

auto coc_c_float_lt = [](float x) {
    return [=](float y) {
        return x < y;
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

#define coc_vec3 0

struct Vec3 {
    float x,y,z;
};

auto coc_vec3_mk = [](float x) {
    return [=](float y) {
        return [=](float z) {
            return Vec3{x,y,z};
        };
    };
};

auto coc_vec3_x = [](Vec3 v) {
    return v.x;
};

auto coc_vec3_y = [](Vec3 v) {
    return v.y;
};

auto coc_vec3_z = [](Vec3 v) {
    return v.z;
};

#define coc_c_float_0_01 0.01f
