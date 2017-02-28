/* file minunit_example.c */
#include "myunit.h"


static char * test_foo()
{
    int foo = 7;
    ass(foo == 7);
    return 0;
}

static char * test_bar()
{
    int bar = 4;
    ass_int(bar++, ==, 4);  // bar is now 5
    ass(bar == 5);
    return 0;
}

static char * test_baz()
{
    char * baz = "hello";
    ass_str(baz, ==, "hello");
    ass_str(baz, !=, "hello, dick");
    ass_str(baz, <, "zzzzzzzzzzz");
    ass_str(baz, >, "aaaaaaaaaaa");
    return 0;
}

static char * test_double()
{
    ass_double(3.549, ==, 3.549);
    return 0;
}

static char * test_fail()
{
    ass(0);
    return 0;
}

static char * passing_tests()
{
    run_test(test_foo);
    run_test(test_bar);
    run_test(test_baz);
    run_test(test_double);
    return 0;
}

static char * failing_tests()
{
    run_test(test_fail);
    return 0;
}

int main(int argc, char **argv)
{
    run_group(passing_tests);
    run_group(failing_tests);

    printf("%d assertions in %d tests\n", _asses_run, _tests_run);
    printf("ALL TESTS PASSED\n");
}
