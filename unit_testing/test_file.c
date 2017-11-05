//
//  test_file.c
//  unit_testing
//
//  Created by HENRY BERGIN on 11/2/17.
//  Copyright © 2017 Henry Bergin. All rights reserved.
//

#include <stdio.h>
#include "test_file.h"
#include "btest.h"

TEST(testExternal, testify)
{
    EXPECT_TRUE(1);
    EXPECT_EQ_INT(23, 23);
    EXPECT_EQ_INT(23, 22);
    EXPECT_FALSE(1);
    EXPECT_FALSE(0);
    EXPECT_EQ_STR("hello", "hello", sizeof("hello"));
    EXPECT_EQ_STR("hello", "world", sizeof("hello"));
}

TEST(testExternal, test2)
{
    EXPECT_TRUE(1);
    EXPECT_TRUE(1);
    EXPECT_TRUE(1);
    EXPECT_TRUE(0);
    EXPECT_TRUE(1);
    EXPECT_TRUE(0);
    EXPECT_TRUE(0);
}

void sample_test_file_init()
{
    ADD_TEST(testExternal, testify);
    ADD_TEST(testExternal, test2);
}
