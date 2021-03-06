/************************************************************************
 *
 * File        : test_runner.cpp
 *
 * Description : Runs all registered test cases.
 *
 * Author(s)   : N. McCallum
 *
 * Version     : 0.1
 *
 ************************************************************************/

/************************************************************************
 * Standard Header Includes
 ************************************************************************/
#include <stdlib.h>

/************************************************************************
 * Other Header Includes
 ************************************************************************/
#include "CppUTest/CommandLineTestRunner.h"

/************************************************************************
 * Macro Definitions
 ************************************************************************/

/************************************************************************
 * Type and Struct Definitions
 ************************************************************************/

/************************************************************************
 * Constant and Enumeration Definitions
 ************************************************************************/

/************************************************************************
 * Functions
 ************************************************************************/

/************************************************************************
 * Name        : main
 *
 * Description : Parses user arguments and runs test with arguments
 *               passed.
 *
 * Returns     : EXIT_FAILURE - One or more tests failed.
 *               EXIT_SUCCESS - All tests succeeded or no tests were run.
 *
 ************************************************************************/
int main(int    argc,
         char **argv)
{
   return CommandLineTestRunner::RunAllTests(argc, argv);
}
