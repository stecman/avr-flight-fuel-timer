// Return the number of items in a statically allocated array
#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

// Return the passed value as a string literal
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
