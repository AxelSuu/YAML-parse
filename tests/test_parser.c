#include "unity.h"
#include "../include/parser.h"

void setUp(void) {}
void tearDown(void) {}

// --- Tests ---

void test_parser(void) {
    YamlMap map = {0};
    TEST_ASSERT_EQUAL(1, parse("../configs/test.yaml", &map));
    TEST_ASSERT_EQUAL(2, map.count);
    TEST_ASSERT_EQUAL_STRING("config.stage", map.entries[0].key);
    TEST_ASSERT_EQUAL_STRING("test", map.entries[0].value);
    TEST_ASSERT_EQUAL_STRING("config.script", map.entries[1].key);
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
    TEST_ASSERT_EQUAL_STRING("test", yaml_get(&map, "config.stage"));
    TEST_ASSERT_EQUAL_STRING("echo \"This is a test\"", yaml_get(&map, "config.script"));
    TEST_ASSERT_NULL(yaml_get(&map, "nonexistent"));
    TEST_ASSERT_NULL(yaml_get(&map, "stage"));
    TEST_ASSERT_NULL(yaml_get(&map, "script"));
}

void test_parser_flat_keys(void) {
    YamlMap map = {0};
    TEST_ASSERT_EQUAL(1, parse("../configs/flat.yaml", &map));
    TEST_ASSERT_EQUAL_STRING("bar", yaml_get(&map, "foo"));
    TEST_ASSERT_EQUAL_STRING("42",  yaml_get(&map, "num"));
}

void test_parser_deep_nesting(void) {
    YamlMap map = {0};
    TEST_ASSERT_EQUAL(1, parse("../configs/deep.yaml", &map));
    TEST_ASSERT_EQUAL_STRING("test", yaml_get(&map, "a.b.c"));
}

void test_parser_mixed(void) {
    YamlMap map = {0};
    TEST_ASSERT_EQUAL(1, parse("../configs/mixed.yaml", &map));
    TEST_ASSERT_EQUAL_STRING("hello", yaml_get(&map, "top"));
    TEST_ASSERT_EQUAL_STRING("world", yaml_get(&map, "nest.key"));
}

void test_yaml_get_partial_key_returns_null(void) {
    YamlMap map = {0};
    TEST_ASSERT_EQUAL(1, parse("../configs/test.yaml", &map));
    TEST_ASSERT_NULL(yaml_get(&map, "config"));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_parser);
    RUN_TEST(test_parser_invalid_file);
    RUN_TEST(test_parser_null_filepath);
    RUN_TEST(test_parser_null_out);
    RUN_TEST(test_yaml_get);
    RUN_TEST(test_parser_flat_keys);
    RUN_TEST(test_parser_deep_nesting);
    RUN_TEST(test_parser_mixed);
    RUN_TEST(test_yaml_get_partial_key_returns_null);
    return UNITY_END();
}
