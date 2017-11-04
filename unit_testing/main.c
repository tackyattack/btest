//
//  main.c
//  unit_testing
//
//  Created by HENRY BERGIN on 10/31/17.
//  Copyright © 2017 Henry Bergin. All rights reserved.
//

#include <stdio.h>
#include "btest.h"
#include "test_file.h"

int main(int argc, const char * argv[])
{
    sample_test_file_init();
    btest_start_testing();
    
    return 0;
}
