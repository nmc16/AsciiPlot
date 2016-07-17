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
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "asciip_lists.h"

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
/* TODO need to add checks for error reporting the correct codes using mocks */
TEST_GROUP(ListTestGroup)
{
   void setup()
   {
      /* Do nothing */
   }

   void teardown()
   {
      /* Do nothing */
   }
};

TEST(ListTestGroup, TestListInit)
{
   Asciip_List *res;
   Asciip_Point *point;

   /* Check NULL result does not get initialized */
   res = asciip_list_init(NULL, NULL, NULL);
   CHECK_TEXT((!res), "Asciip list was initialized with NULL pointers");

   /* Check that NULL error/init_point works */
   res = asciip_list_init(NULL, &res, NULL);
   CHECK_TEXT((res), "Asciip list was not initialized with non-NULL result");
   CHECK_TEXT((!res->head), "Asciip list HEAD was not NULL when registered with NULL init point");
   CHECK_TEXT((!res->tail), "Asciip list TAIL was not NULL when registered with NULL init pointer");
   UNSIGNED_LONGS_EQUAL(0, res->size);
   asciip_list_destroy(res, NULL);
   res = NULL;

   /* Check call with actual point */
   point = asciip_point_init(0, 0, &point, NULL);
   res = asciip_list_init(point, &res, NULL);
   CHECK_TEXT((res), "List not initialized with point and result");
   CHECK_TEXT((res->head), "Asciip list HEAD was NULL when registered with init point");
   CHECK_TEXT((res->tail), "Asciip list TAIL was NULL when registered with init pointer");
   POINTERS_EQUAL(res->tail, res->head);
   UNSIGNED_LONGS_EQUAL(1, res->size);
   POINTERS_EQUAL(point, res->head->data);
};
