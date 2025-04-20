#include "unity.h"
#include "main.h"

void setUp(void) {
    // Runs before each test case
}

void tearDown(void) {
    // Runs after each test case
}
int Initialize(int a, int b);
int FBL_Check(int a, int b);
int CAL_Check(int a, int b);
int APP_Check(int a, int b);
int Set_Target_FW(int a, int b);
int ProgRequest_Check(int a, int b, int c);

void test_Initialize_1(void) {
    TEST_ASSERT_EQUAL(5, Initialize(2, 3));
}

void test_FBL_Check_1(void) {
    TEST_ASSERT_EQUAL(0, FBL_Check(3, 3));
}

void test_CAL_Check_1(void) {
    TEST_ASSERT_EQUAL(6, CAL_Check(2, 3));
}

void test_APP_Check_1(void) {
    TEST_ASSERT_EQUAL(1, APP_Check(3, 3));
}


void test_Set_Target_FW_1(void) {
    TEST_ASSERT_EQUAL(1, Set_Target_FW(4, 3));
}

void test_ProgRequest_Check_1(void) {
    TEST_ASSERT_EQUAL(4, ProgRequest_Check(3, 3, 2));
}

void test_Initialize_2(void) {
    TEST_ASSERT_EQUAL(5, Initialize(2, 3));
}

void test_FBL_Check_2(void) {
    TEST_ASSERT_EQUAL(0, FBL_Check(3, 3));
}

void test_CAL_Check_2(void) {
    TEST_ASSERT_EQUAL(6, CAL_Check(2, 3));
}

void test_APP_Check_2(void) {
    TEST_ASSERT_EQUAL(1, APP_Check(3, 3));
}


void test_Set_Target_FW_2(void) {
    TEST_ASSERT_EQUAL(1, Set_Target_FW(4, 3));
}

void test_ProgRequest_Check_2(void) {
    TEST_ASSERT_EQUAL(4, ProgRequest_Check(3, 3, 2));
}


int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_Initialize_1);
    RUN_TEST(test_FBL_Check_1);
    RUN_TEST(test_CAL_Check_1);
    RUN_TEST(test_APP_Check_1);
    RUN_TEST(test_Set_Target_FW_1);
    RUN_TEST(test_ProgRequest_Check_1);
    RUN_TEST(test_Initialize_2);
    RUN_TEST(test_FBL_Check_2);
    RUN_TEST(test_CAL_Check_2);
    RUN_TEST(test_APP_Check_2);
    RUN_TEST(test_Set_Target_FW_2);
    RUN_TEST(test_ProgRequest_Check_2);
    return UNITY_END();
}
