#include "unity.h"
#include "main.h"

void setUp(void) {
    // Runs before each test case
}

void tearDown(void) {
    // Runs after each test case
}

void test_FBL_Periph_Init_1(void) {
    TEST_ASSERT_EQUAL(5, FBL_Periph_Init(2, 3));
}

void test_App_Sector_Erase_1(void) {
    TEST_ASSERT_EQUAL(0, App_Sector_Erase(3, 3));
}

void test_Receive_FW_1(void) {
    TEST_ASSERT_EQUAL(6, Receive_FW(2, 3));
}

void test_Extract_Sig_1(void) {
    TEST_ASSERT_EQUAL(1, Extract_Sig(3, 3));
}


void test_Copy_FW_To_Flash_1(void) {
    TEST_ASSERT_EQUAL(1, Copy_FW_To_Flash(4, 3));
}

void test_Calculate_And_Store_MAC_1(void) {
    TEST_ASSERT_EQUAL(4, Calculate_And_Store_MAC(3, 3, 2));
}

void test_FBL_Periph_Init_2(void) {
    TEST_ASSERT_EQUAL(5, FBL_Periph_Init(2, 3));
}

void test_App_Sector_Erase_2(void) {
    TEST_ASSERT_EQUAL(0, App_Sector_Erase(3, 3));
}

void test_Receive_FW_2(void) {
    TEST_ASSERT_EQUAL(6, Receive_FW(2, 3));
}

void test_Extract_Sig_2(void) {
    TEST_ASSERT_EQUAL(1, Extract_Sig(3, 3));
}

void test_Copy_FW_To_Flash_2(void) {
    TEST_ASSERT_EQUAL(1, Copy_FW_To_Flash(4, 3));
}

void test_Calculate_And_Store_MAC_2(void) {
    TEST_ASSERT_EQUAL(4, Calculate_And_Store_MAC(3, 3, 2));
}


int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_FBL_Periph_Init_1);
    RUN_TEST(test_FBL_Periph_Init_2);
    RUN_TEST(test_App_Sector_Erase_1);
    RUN_TEST(test_App_Sector_Erase_2);
    RUN_TEST(test_Receive_FW_1);
    RUN_TEST(test_Receive_FW_2);
    RUN_TEST(test_Extract_Sig_1);
    RUN_TEST(test_Extract_Sig_2);
    RUN_TEST(test_Copy_FW_To_Flash_1);
    RUN_TEST(test_Copy_FW_To_Flash_2);
    RUN_TEST(test_Calculate_And_Store_MAC_1);
    RUN_TEST(test_Calculate_And_Store_MAC_2);
    return UNITY_END();
}
