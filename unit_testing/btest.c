//
//  btest.c
//  unit_testing
//
//  Created by HENRY BERGIN on 10/31/17.
//  Copyright © 2017 Henry Bergin. All rights reserved.
//

// Modeled after gtest. Boost test and google test both seem to use
// static intitialization of objects to register the test function
// at runtime. This isn't possible with C so you need to register
// the tests manually. Maybe it would be possible to peek in the object
// files to find the function addresses, but that could get messy.

// Test report should print the line that failed.

// An external function table map file could be cool (addresses to functions that could be read anywhere).

// Note: test names can't have underscores since that is a delimiter

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "btest.h"

//----- linked list -----

typedef struct list_node
{
    void *data;
    struct list_node *next;
}list_node;


// Note: to dereference void pointer: uint8_t value = *(uint8_t *)(void_ptr);

void ll_add_node(list_node **head, void *data, uint8_t data_size)
{
    
    list_node *new_node = malloc(sizeof(list_node));

    new_node->data = malloc(data_size);
    memcpy(new_node->data, data, data_size);
    
    if( (*head) == NULL )
    {
        (*head) = new_node;
        (*head)->next = NULL;
    }
    else
    {
        list_node *current_node = (*head);
        while(current_node->next != NULL)
        {
            current_node = current_node->next;
        }
        
        new_node->next = NULL;
        current_node->next = new_node;
    }
}

void ll_teardown(list_node *head)
{
    // walk down the list and delete the nodes
    list_node *current_node = head;
    list_node *trail_node = head; // walks behind the current node
    
    while(current_node != NULL)
    {
        trail_node = current_node;
        current_node = current_node->next;
        free(trail_node->data); // delete the data that was alloctated
        free(trail_node);
        trail_node->data = NULL;
        trail_node = NULL;
    }
}

void *ll_get_data(list_node *head, uint16_t pos)
{ // todo: make it so that it stores the last search position and can jump if get is >= last pos
    uint8_t hit_end = 0;
    list_node *current_node = head;
    for (uint16_t i = 0; (i < pos) && (!hit_end); i++) {
        
        if(current_node->next == NULL && (i<pos))
        {
            current_node = NULL;
            hit_end = 1;
        }
        else
        {
            current_node = current_node->next;
        }
    }
    
    if(current_node == NULL)
    {
        return NULL;
    }
    else
    {
        return current_node->data;
    }
}

void *ll_next(list_node *current_node)
{
    if((current_node)->next != NULL)
    {
        void *data = (current_node)->data;
        return data;
    }
    else
    {
        return NULL;
    }
}

void *ll_data(list_node *node)
{
    return node->data;
}

void ll_iterate(list_node **current_node)
{
    (*current_node) = (*current_node)->next;
}

//------------------------


typedef struct test_report_node
{
    char *failed_line_string;
}R_node;

typedef struct test_node
{
    char name[100];
    void (*test_ptr)(void); // function pointer
    uint16_t failedCount; // 0 -> pass    >0 -> number failed
}T_node;

list_node *test_head = NULL;
list_node *report_head = NULL;


void update_test_status(const char *test_name, uint8_t passfail)
{
    list_node *current_node = test_head;
    T_node *data_node = (T_node *)ll_data(current_node);
    while(data_node != NULL && strcmp(data_node->name,test_name))
    {
        data_node = (T_node *)ll_data(current_node);
        ll_iterate(&current_node);
    }
    
    if(!passfail) data_node->failedCount++; // increment the number of failed tests
}

void expect_true(uint8_t val, const char *test_name)
{
    if(val)
    {
        printf("%s: EXPECT TRUE PASSED\n", test_name);
        update_test_status(test_name, 1);
    }
    else
    {
        printf("-> %s: EXPECT TRUE FAILED\n", test_name);
        update_test_status(test_name, 0);
    }
}

void add_test_report(R_node node)
{
    ll_add_node(&report_head, &node, sizeof(node));
}

void add_test_node(T_node node)
{
    ll_add_node(&test_head, &node, sizeof(node));
}

void btest_add_test(void *testf, const char *name)
{ // should probably be a list of function pointers that is static. Then the start test
  // function would walk down the list calling each test
    
    T_node node;
    strcpy(node.name, name);
    node.test_ptr = testf;
    node.failedCount = 0; // start out as all tests failing
    
    ll_add_node(&test_head, &node, sizeof(node));
    
    printf("adding test: %s\n",node.name);
}

void btest_run_all_tests()
{
    list_node *current_node = test_head;
    while(current_node != NULL)
    {
        printf("==============================\n\n");
        printf("====running %s====\n", ((T_node *)ll_data(current_node))->name);
        (*(((T_node *)ll_data(current_node))->test_ptr))(); // run the test
        ll_iterate(&current_node);
    }
    printf("==============================\n\n");
}

void btest_teardown()
{ // walk down the list and delete the test
    ll_teardown(test_head);
}

void btest_report()
{
    list_node *current_node = test_head;
    T_node *data_node;
    uint16_t failed_tests_count = 0;
    
    while(current_node != NULL)
    {
        data_node = (T_node *)ll_data(current_node);
        if(data_node->failedCount > 0) failed_tests_count += data_node->failedCount;
        ll_iterate(&current_node);
    }
    
    printf("failed tests: %d\n", failed_tests_count); // this could be more specific eventually
                                                      // i.e. which tests failed as a whole, and which part
}

void btest_start_testing()

{
    
    printf("++++++++ BTEST +++++++++\n");

    printf("starting testing...\n\n");
    btest_run_all_tests();
    
    printf("======== testing report ========\n");
    btest_report();
    printf("================================\n\n");
    
    printf("tearing down testing...\n");
    btest_teardown();
    printf("testing completed...\n");
    
    printf("++++++++++++++++++++++++\n\n\n");
    
}

