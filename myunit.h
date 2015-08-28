/* file: myunit.h */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ass(test) do { \
    _asses_run++; \
    char *message; \
    asprintf(&message, "%s:%d:%s() Assertion '%s' failed", \
            __FILE__, __LINE__, __func__, #test); \
    if (!(test)) return message; \
} while (0)

#define ass_int(X, COMP, Y) do { \
    _asses_run++; \
    long long int __x = X, __y = Y;  /* eval once */ \
    char *message; \
    asprintf(&message, \
            "%s:%d:%s() Comparison '%s %s %s' failed, " \
            "!(%lld %s %lld)", \
            __FILE__, __LINE__, __func__, #X, #COMP, #Y, __x, #COMP, __y); \
    if (!(__x COMP __y)) return message; \
} while (0)

#define ass_str(X, COMP, Y) do { \
    _asses_run++; \
    /* eval once */ \
    char * __x = strdup(X); \
    char * __y = strdup(Y);  \
    char *message; \
    asprintf(&message, \
            "%s:%d:%s() Comparison '%s %s %s' failed, " \
            "!(%s %s %s)", \
            __FILE__, __LINE__, __func__, #X, #COMP, #Y, __x, #COMP, __y); \
    if (!(strcmp(__x, __y) COMP 0)) return message; \
} while (0)

#define run_test(test) do { char *message = test(); _tests_run++; \
    if (message) return message; \
    else free(message); \
} while (0)

#define run_group(group) do { \
    int prev_tests_run = _tests_run; \
    char *result = group(); \
    if (result) { /* a test failed */ \
        printf("%s/%s -> FAIL\n", __FILE__, #group); \
        printf("%s\n", result); \
        return 1; \
    } else { \
        printf("%s/%s -> %d PASSED\n", __FILE__, #group, \
                _tests_run - prev_tests_run); \
    } \
} while(0)

int _tests_run = 0, _asses_run = 0;
