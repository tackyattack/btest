//
//  btest.h
//  unit_testing
//
//  Created by HENRY BERGIN on 10/31/17.
//  Copyright © 2017 Henry Bergin. All rights reserved.
//

// Simple testing library for C

#ifndef btest_h
#define btest_h

#define STRINGIFY(x) #x
#define CONCAT(x, y) STRINGIFY(x ## y)

#define ADD_TEST(test_group, test_name) btest_add_test(test_group ## _ ## test_name, \
CONCAT(test_group, _  ## test_name))
#define TEST(test_group, test_name) void test_group ## _ ## test_name()

#define EXPECT_TRUE(x) expect_true(x, __func__)

void btest_start_testing();
void btest_add_test(void *testf, const char *name);
void expect_true(uint8_t val, const char *test_name);

#endif /* btest_h */
