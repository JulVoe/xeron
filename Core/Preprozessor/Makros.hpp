# if 0
//Default setting:
#define KB *1024
#define CACHE_SIZE 64KB
#define CACHE_LINE_SIZE 64
#define CORES 8
#define THREADS 16
#endif

//Other useful macros for performance improvement
#ifndef VS
#define BRANCH_PREDICT(condition,prob) __builtin_expect(condition,prob)
#define UNREACHABLE __builtin_unreachable
#define ALWAYS_INLINE __attribute__((always_inline)) inline
#define RESTRICT __restrict__
#else
#define BRANCHPREDICT(condition,prob) condition
#define UNREACHABLE __assume(0);
#define ALWAYS_INLINE __forceinline
#define RESTRICT __restrict
#endif

#ifndef VS
#define DEPRECATE(foo, msg) foo __attribute__((deprecated(msg)))
#else
#define DEPRECATE(foo, msg) __declspec(deprecated(msg)) foo
#endif

#define PP_CAT(x,y) PP_CAT1(x,y)
#define PP_CAT1(x,y) x##y
namespace detail
{
    struct true_type {};
    struct false_type {};
    template <int test> struct converter : public true_type {};
    template <> struct converter<0> : public false_type {};
}
#define STATIC_WARNING(cond, msg) \
struct PP_CAT(static_warning,__LINE__) { \
  DEPRECATE(void _(::detail::false_type const& ),msg) {}; \
  void _(::detail::true_type const& ) {}; \
  PP_CAT(static_warning,__LINE__)() {_(::detail::converter<(cond)>());} \
}
