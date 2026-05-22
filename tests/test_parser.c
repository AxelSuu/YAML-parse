#include "unity.h"
#include "../include/parser.h"

void setUp(void) {}
void tearDown(void) {}

// --- Tests ---

void test_parser(void) {
    YamlMap map = {0};
    TEST_ASSERT_EQUAL(1, parse("../configs/test.yaml", &map));
    TEST_ASSERT_EQUAL(2, map.count);
    TEST_ASSERT_EQUAL_STRING("stage", map.entries[0].key);
    TEST_ASSERT_EQUAL_STRING("test", map.entries[0].value);
    TEST_ASSERT_EQUAL_STRING("script", map.entries[1].key);
    TEST_ASSERT_EQUAL_STRING("echo \"This is a test\"", map.entries[1].value);
}

void test_parser_invalid_file(void) {
    YamlMap map = {0};
    TEST_ASSERT_EQUAL(0, parse("../configs/nonexistent.yaml", &map));
}

void test_parser_null_filepath(void) {
    YamlMap map = {0};
    TEST_ASSERT_EQUAL(0, parse(NULL, &map));
}

void test_parser_null_out(void) {
    TEST_ASSERT_EQUAL(0, parse("../configs/test.yaml", NULL));
}

void test_yaml_get(void) {
    YamlMap map = {0};
    TEST_ASSERT_EQUAL(1, parse("../configs/test.yaml", &map));
    TEST_ASSERT_EQUAL_STRING("test", yaml_get(&map, "stage"));
    TEST_ASSERT_EQUAL_STRING("echo \"This is a test\"", yaml_get(&map, "script"));
    TEST_ASSERT_NULL(yaml_get(&map, "nonexistent"));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_parser);
    RUN_TEST(test_parser_invalid_file);
    RUN_TEST(test_parser_null_filepath);
    RUN_TEST(test_parser_null_out);
    RUN_TEST(test_yaml_get);
    return UNITY_END();
}
