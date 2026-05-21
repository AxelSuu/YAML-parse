#include "unity.h"
#include "../include/parser.h"

// Runs before each test — reset state, init structs, etc.
void setUp(void) {}

// Runs after each test — free memory, close handles, etc.
void tearDown(void) {}

// --- Tests ---
void test_parser(void) {
    int i;
    YamlMap map = {0};
    i = parse("../configs/test.yaml", &map);
    TEST_ASSERT_EQUAL(1, i);
}

void test_parser_invalid_file(void) {
    int i;
    YamlMap map = {0};
    i = parse("../configs/nonexistent.yaml", &map);
    TEST_ASSERT_EQUAL(0, i);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_parser);
    RUN_TEST(test_parser_invalid_file);
    return UNITY_END();
}