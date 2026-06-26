#ifndef COMMON_H
#define COMMON_H

#define EXTRACT_BITS(val, high, low) \
(((val) >> (low)) & ((1U << ((high)-(low)+1)) - 1))

#endif