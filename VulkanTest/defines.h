#define ASSERT(fn, x) assert(VK_SUCCESS == fn && x);
#define ASSERT_VALUE(st1, st2, x) assert(st1 == st2 && x);