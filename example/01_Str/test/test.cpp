#include "test.hpp"

void test_construct_empty() {
    Str<SLEN> str = Str<SLEN>();

    TEST_ASSERT_EQUAL_size_t(SLEN, str.size());
    TEST_ASSERT_EQUAL_size_t(0, str.length());
    TEST_ASSERT_TRUE(str.empty());
    TEST_ASSERT_EQUAL_size_t(SLEN - 0, str.left());
}

void test_construct_preset_string() {
    Str<SLEN> str = Str<SLEN>("string");

    TEST_ASSERT_EQUAL_size_t(SLEN, str.size());
    TEST_ASSERT_EQUAL_size_t(6, str.length());
    TEST_ASSERT_FALSE(str.empty());
    TEST_ASSERT_EQUAL_size_t(SLEN - 6, str.left());
}

void test_construct_preset_char() {
    Str<TLEN> str = Str<TLEN>('s');

    TEST_ASSERT_EQUAL_size_t(TLEN, str.size());
    TEST_ASSERT_EQUAL_size_t(1, str.length());
    TEST_ASSERT_FALSE(str.empty());
    TEST_ASSERT_EQUAL_size_t(TLEN - 1, str.left());
}

void test_char_pointer() {
    Str<SLEN> str = Str<SLEN>("string");

    TEST_ASSERT_EQUAL_CHAR('s', str.c_str()[0]);
    TEST_ASSERT_EQUAL_CHAR('t', str.c_str()[1]);
    TEST_ASSERT_EQUAL_CHAR('r', str.c_str()[2]);
    TEST_ASSERT_EQUAL_CHAR('i', str.c_str()[3]);
    TEST_ASSERT_EQUAL_CHAR('n', str.c_str()[4]);
    TEST_ASSERT_EQUAL_CHAR('g', str.c_str()[5]);
    TEST_ASSERT_EQUAL_CHAR('\0', str.c_str()[6]);

    TEST_ASSERT_EQUAL_CHAR('s', str.c()[0]);
    TEST_ASSERT_EQUAL_CHAR('t', str.c()[1]);
    TEST_ASSERT_EQUAL_CHAR('r', str.c()[2]);
    TEST_ASSERT_EQUAL_CHAR('i', str.c()[3]);
    TEST_ASSERT_EQUAL_CHAR('n', str.c()[4]);
    TEST_ASSERT_EQUAL_CHAR('g', str.c()[5]);
    TEST_ASSERT_EQUAL_CHAR('\0', str.c()[6]);
}

void test_equals_char() {
    Str<TLEN> str = Str<TLEN>('s');

    TEST_ASSERT_TRUE(str.equals("s"));
    TEST_ASSERT_TRUE(str.equals('s'));
    TEST_ASSERT_TRUE(str == "s");
    TEST_ASSERT_TRUE(str == 's');

    TEST_ASSERT_FALSE(str.equals("x"));
    TEST_ASSERT_FALSE(str.equals('x'));
    TEST_ASSERT_TRUE(str != "x");
    TEST_ASSERT_TRUE(str != 'x');
}

void test_equals_char_array() {
    Str<SLEN> str = Str<SLEN>("string");
    const char txt[] = {'s', 't', 'r', 'i', 'n', 'g', '\0'};
    const char wrg[] = {'w', 'r', 'o', 'n', 'g', '\0'};

    TEST_ASSERT_TRUE(str.equals("string"));
    TEST_ASSERT_TRUE(str.equals(txt));
    TEST_ASSERT_TRUE(str == "string");
    TEST_ASSERT_TRUE(str.equals(txt));
    TEST_ASSERT_TRUE(str == txt);

    TEST_ASSERT_FALSE(str.equals("wrong"));
    TEST_ASSERT_TRUE(str != "wrong");
    TEST_ASSERT_FALSE(str.equals(wrg));
    TEST_ASSERT_TRUE(str != wrg);
}

void test_equals_string() {
    Str<TLEN> one = Str<TLEN>("one");
    Str<SLEN> two = Str<SLEN>("two");

    TEST_ASSERT_TRUE(one.equals(one));
    TEST_ASSERT_TRUE(one == one);
    TEST_ASSERT_TRUE(one.equals(Str<TLEN>("one")));
    TEST_ASSERT_TRUE(one.equals(Str<SLEN>("one")));

    TEST_ASSERT_FALSE(one.equals(two));
    TEST_ASSERT_TRUE(one != two);
    TEST_ASSERT_FALSE(one.equals(Str<TLEN>("two")));
    TEST_ASSERT_FALSE(one.equals(Str<SLEN>("two")));

    TEST_ASSERT_TRUE(two.equals(two));
    TEST_ASSERT_TRUE(two == two);
    TEST_ASSERT_TRUE(two.equals(Str<TLEN>("two")));
    TEST_ASSERT_TRUE(two.equals(Str<SLEN>("two")));

    TEST_ASSERT_FALSE(two.equals(one));
    TEST_ASSERT_TRUE(two != one);
    TEST_ASSERT_FALSE(two.equals(Str<TLEN>("one")));
    TEST_ASSERT_FALSE(two.equals(Str<SLEN>("one")));
}


void run() {
    RUN_TEST(test_construct_empty);
    RUN_TEST(test_construct_preset_string);
    RUN_TEST(test_construct_preset_char);
    RUN_TEST(test_char_pointer);
    RUN_TEST(test_equals_char);
    RUN_TEST(test_equals_char_array);
    RUN_TEST(test_equals_string);
}
