#ifndef ICOPTIMIZE_H
#define ICOPTIMIZE_H

#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)


#endif // ICOPTIMIZE_H
