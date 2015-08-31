/** \file
 * \defgroup module_myunit <myunit.h>: MyUnit
 * @{
 *
 * \brief      MyUnit interface
 * \version    0.1
 * \author     Dan Fekete <thefekete@gmail.com>
 * \date       August 30, 2015
 *
 * \copyright  Copyright (c) 2015, Dan Fekete <thefekete@gmail.com>,
 *             Released under the WTFPL, see COPYING file for details
 *             or http://www.wtfpl.net/txt/copying
 *
 * \section Installation
 * Copy to your project folder and start writting tests!
 *
 * \section Usage
 * \par Include the header
 * To write tests with MyUnit, you must first include the header file. You can
 * just copy this file to your source directory, symlink it or add the directory
 * where it lives to your compilers search path.
 * \code
 * #include "myunit.h"
 * \endcode
 *
 * \par Add a test
 * Here we add a failing test to make sure it works, or fails in this case:
 * \code
 * static char * fail_test(void) {
 *     ass(0);
 *     return 0;
 * }
 * \endcode
 *
 * \par Add the test to a test group
 * Tests are run from test groups. Simply define a test group and call each of
 * your tests with \ref run_test().
 * \code
 * static char * my_test_group()
 * {
 *     run_test(fail_test);
 *     return 0;
 * }
 * \endcode
 *
 * \par Main function
 * Now you just call each test group with \ref run_group() and add a pass
 * at the end.
 * \code
 * int main(void) {
 *     run_group(my_test_group);
 *
 *     printf("%d assertions in %d tests\n", _asses_run, _tests_run);
 *     printf("ALL PASSED");
 * }
 * \endcode
 *
 * That's it! Just compile your test program and run it!
 *
 * \section makefile Example makefile
 * Here's an example make file to make your tests:
 * \code
 * P=my_tests                              # test program name without extension
 * OBJECTS=                                         # files being tested go here
 * CFLAGS=-Wall -Wno-unused     # show all errors, except for comented out tests
 * CC=gcc -std=gnu99 -D_GNU_SOURCE             # gcc with extras (asprintf, etc)
 *
 * $(P): $(OBJECTS)
 *
 * .PHONY: check
 * check: $(P)
 * 	./$(P)
 *
 * .PHONY: clean
 * clean:
 * 	$(RM) $(P) $(OBJECTS)
 * \endcode
 *
 */
#ifndef _MYUNIT_H_
#define _MYUNIT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** \brief Assert that \c test is true / non-zero */
#define ass(test) do { \
    _asses_run++; \
    char *message; \
    asprintf(&message, "%s:%d:%s() Assertion '%s' failed", \
            __FILE__, __LINE__, __func__, #test); \
    if (!(test)) return message; \
} while (0)

/** \brief Compare two integers
 *
 * Compares the two integers like an if statement. COMP can be any comparison
 * operator that you sould use in an if statement.
 *
 * \note These macros use a local variable inside a block to evaluate the inputs
 *       exactly once. Therefore if your test is relying on a global with the
 *       same name as this internal variable, you will have problems.
 *
 * \param X The first integer to be compared
 * \param COMP the comparison to be performed
 * \param Y The second integer to be compared
 */
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

/** \brief Compare two strings
 *
 * Uses strcmp to compare the two strings. Then compares that result with 0
 * using COMP. COMP can be any comparison operator you would use in an if
 * statement.
 *
 * \note These macros use a local variable inside a block to evaluate the inputs
 *       exactly once. Therefore if your test is relying on a global with the
 *       same name as this internal variable, you will have problems.
 *
 * \param X The first string to be compared
 * \param COMP the comparison to be performed
 * \param Y The second string to be compared
 */
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

/** \brief Run a test
 *
 * Tests must have the following signature:
 * \code
 * static char * my_test_name(void);
 * \endcode
 * On failure, the assertion macros will return a string with the error message
 * for you. If all the assertions pass the function should return 0 or NULL. Do
 * this by placing a "<tt>return 0;</tt>" as the last statement in the test
 * function.
 *
 * \param test The test function to be run
 */
#define run_test(test) do { char *message = test(); _tests_run++; \
    if (message) return message; \
    else free(message); \
} while (0)

/** \brief Run a group of tests
 *
 * Test groups must have the following signature:
 * \code
 * static char * my_group_name(void);
 * \endcode
 * If any of the tests fail, this macro will return a string with the error
 * message from the failed test for you. If all the tests pass the function
 * should return 0 or NULL. Do this by placing a "<tt>return 0;</tt>" as the
 * last statement in the test function.
 *
 * \param group The test group function to be run
 */
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

long int _tests_run = 0;              /**< number of tests that have been run */
long int _asses_run = 0;       /**< number of asserts that have been asserted */


#endif /* _MYUNIT_H_ */
/** @} */  // end of  module_myunit group
