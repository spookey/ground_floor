#include "test.hpp"

void test_construct_empty() {
    Str16 str = Str16();

    TEST_ASSERT_EQUAL_size_t(16, str.size());
    TEST_ASSERT_EQUAL_size_t(0, str.length());
    TEST_ASSERT_TRUE(str.empty());
    TEST_ASSERT_TRUE(!str);
    TEST_ASSERT_EQUAL_size_t(16 - 0, str.left());
}

void test_construct_preset_string() {
    Str16 str = Str16("string");

    TEST_ASSERT_EQUAL_size_t(16, str.size());
    TEST_ASSERT_EQUAL_size_t(6, str.length());
    TEST_ASSERT_FALSE(str.empty());
    TEST_ASSERT_FALSE(!str);
    TEST_ASSERT_EQUAL_size_t(16 - 6, str.left());
}

void test_construct_preset_char() {
    Str8 str = Str8('s');

    TEST_ASSERT_EQUAL_size_t(8, str.size());
    TEST_ASSERT_EQUAL_size_t(1, str.length());
    TEST_ASSERT_FALSE(str.empty());
    TEST_ASSERT_FALSE(!str);
    TEST_ASSERT_EQUAL_size_t(8 - 1, str.left());
}

void test_char_pointer() {
    Str16 str = Str16("string");
    TEST_ASSERT_EQUAL_CHAR('s', str.c_str()[0]);
    TEST_ASSERT_EQUAL_CHAR('t', str.c_str()[1]);
    TEST_ASSERT_EQUAL_CHAR('r', str.c_str()[2]);
    TEST_ASSERT_EQUAL_CHAR('i', str.c_str()[3]);
    TEST_ASSERT_EQUAL_CHAR('n', str.c_str()[4]);
    TEST_ASSERT_EQUAL_CHAR('g', str.c_str()[5]);
    TEST_ASSERT_EQUAL_CHAR('\0', str.c_str()[6]);

    TEST_ASSERT_EQUAL_CHAR('s', (*str)[0]);
    TEST_ASSERT_EQUAL_CHAR('t', (*str)[1]);
    TEST_ASSERT_EQUAL_CHAR('r', (*str)[2]);
    TEST_ASSERT_EQUAL_CHAR('i', (*str)[3]);
    TEST_ASSERT_EQUAL_CHAR('n', (*str)[4]);
    TEST_ASSERT_EQUAL_CHAR('g', (*str)[5]);
    TEST_ASSERT_EQUAL_CHAR('\0', (*str)[6]);
}

void test_equals_char() {
    Str8 str = Str8('s');

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
    Str16 str = Str16("string");
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
    Str8 one = Str8("one");
    Str16 two = Str16("two");

    TEST_ASSERT_TRUE(one.equals(one));
    TEST_ASSERT_TRUE(one == one);
    TEST_ASSERT_TRUE(one.equals(Str8("one")));
    TEST_ASSERT_TRUE(one.equals(Str16("one")));

    TEST_ASSERT_FALSE(one.equals(two));
    TEST_ASSERT_TRUE(one != two);
    TEST_ASSERT_FALSE(one.equals(Str8("two")));
    TEST_ASSERT_FALSE(one.equals(Str16("two")));

    TEST_ASSERT_TRUE(two.equals(two));
    TEST_ASSERT_TRUE(two == two);
    TEST_ASSERT_TRUE(two.equals(Str8("two")));
    TEST_ASSERT_TRUE(two.equals(Str16("two")));

    TEST_ASSERT_FALSE(two.equals(one));
    TEST_ASSERT_TRUE(two != one);
    TEST_ASSERT_FALSE(two.equals(Str8("one")));
    TEST_ASSERT_FALSE(two.equals(Str16("one")));
}

void test_starts_with_char() {
    Str8 str = Str8('s');
    Str8 nil = Str8();

    TEST_ASSERT_TRUE(str.starts_with('s'));
    TEST_ASSERT_FALSE(str.starts_with('c'));

    TEST_ASSERT_FALSE(nil.starts_with('s'));
    TEST_ASSERT_FALSE(nil.starts_with('c'));
}

void test_starts_with_char_array() {
    Str16 str = Str16("string");
    Str8 nil = Str8();

    TEST_ASSERT_TRUE(str.starts_with('s'));
    TEST_ASSERT_TRUE(str.starts_with("string"));
    TEST_ASSERT_FALSE(str.starts_with("string23"));
    TEST_ASSERT_FALSE(str.starts_with("23string"));

    TEST_ASSERT_TRUE(str.starts_with("s"));
    TEST_ASSERT_TRUE(str.starts_with("st"));
    TEST_ASSERT_TRUE(str.starts_with("str"));
    TEST_ASSERT_FALSE(str.starts_with("stru"));
    TEST_ASSERT_FALSE(str.starts_with("struc"));
    TEST_ASSERT_FALSE(str.starts_with("strut"));

    TEST_ASSERT_TRUE(nil.starts_with(""));
    TEST_ASSERT_FALSE(nil.starts_with("string"));
}

void test_starts_with_string() {
    Str16 str = Str16("string");
    Str8 nil = Str8();

    TEST_ASSERT_TRUE(str.starts_with('s'));
    TEST_ASSERT_TRUE(str.starts_with("s"));

    TEST_ASSERT_TRUE(str.starts_with(Str8("string")));
    TEST_ASSERT_FALSE(str.starts_with(Str8("string42")));
    TEST_ASSERT_FALSE(str.starts_with(Str8("42string")));
    TEST_ASSERT_FALSE(str.starts_with(Str8("struct")));

    TEST_ASSERT_FALSE(nil.starts_with(Str8("string")));
    TEST_ASSERT_TRUE(nil.starts_with(nil));
}

void test_ends_with_char() {
    Str8 str = Str8('s');
    Str8 nil = Str8();

    TEST_ASSERT_TRUE(str.ends_with('s'));
    TEST_ASSERT_FALSE(str.ends_with('c'));

    TEST_ASSERT_FALSE(nil.ends_with('s'));
    TEST_ASSERT_FALSE(nil.ends_with('c'));
}

void test_ends_with_char_array() {
    Str16 str = Str16("string");
    Str8 nil = Str8();

    TEST_ASSERT_TRUE(str.ends_with('g'));
    TEST_ASSERT_TRUE(str.ends_with("string"));
    TEST_ASSERT_FALSE(str.ends_with("string23"));
    TEST_ASSERT_FALSE(str.ends_with("23string"));

    TEST_ASSERT_TRUE(str.ends_with("g"));
    TEST_ASSERT_TRUE(str.ends_with("ng"));
    TEST_ASSERT_TRUE(str.ends_with("ing"));
    TEST_ASSERT_FALSE(str.ends_with("ting"));
    TEST_ASSERT_FALSE(str.ends_with("sting"));
    TEST_ASSERT_FALSE(str.ends_with("esting"));
    TEST_ASSERT_FALSE(str.ends_with("testing"));

    TEST_ASSERT_TRUE(nil.ends_with(""));
    TEST_ASSERT_FALSE(nil.ends_with("string"));
}

void test_ends_with_string() {
    Str16 str = Str16("string");
    Str8 nil = Str8();

    TEST_ASSERT_TRUE(str.ends_with('g'));
    TEST_ASSERT_TRUE(str.ends_with("g"));

    TEST_ASSERT_TRUE(str.ends_with(Str8("string")));
    TEST_ASSERT_FALSE(str.ends_with(Str8("string42")));
    TEST_ASSERT_FALSE(str.ends_with(Str8("42string")));
    TEST_ASSERT_FALSE(str.ends_with(Str8("struct")));

    TEST_ASSERT_FALSE(nil.ends_with(Str8("string")));
    TEST_ASSERT_TRUE(nil.ends_with(nil));
}

void test_set_char() {
    Str8 str = Str8('s');
    TEST_ASSERT_TRUE(str == 's');

    Str8 chr = str.set('c');
    TEST_ASSERT_TRUE(chr == 'c');
    TEST_ASSERT_TRUE(str == 'c');
}

void test_set_char_array() {
    const char txt[] = {'s', 't', 'r', 'i', 'n', 'g', '\0'};
    const char cnt[] = {'c', 'o', 'n', 't', 'e', 'n', 't', '\0'};
    Str16 str = Str16(txt);

    TEST_ASSERT_TRUE(str == "string");
    Str16 tmp = str.set(cnt);
    TEST_ASSERT_TRUE(tmp == "content");
    TEST_ASSERT_TRUE(str == "content");
}

void test_set_string() {
    Str16 str = Str16("string");
    TEST_ASSERT_TRUE(str == "string");

    Str16 tmp = str.set("content");
    TEST_ASSERT_TRUE(tmp == "content");
    TEST_ASSERT_TRUE(str == "content");
}

void test_clear() {
    Str16 str = Str16("string");

    TEST_ASSERT_EQUAL_size_t(6, str.length());
    TEST_ASSERT_FALSE(str.empty());

    Str16 nil = str.clear();

    TEST_ASSERT_EQUAL_size_t(16, nil.size());
    TEST_ASSERT_EQUAL_size_t(0, nil.length());
    TEST_ASSERT_TRUE(nil.empty());
    TEST_ASSERT_TRUE(!nil);
    TEST_ASSERT_EQUAL_size_t(16 - 0, nil.left());

    TEST_ASSERT_EQUAL_size_t(0, str.length());
    TEST_ASSERT_TRUE(str.empty());
}


void run() {
    RUN_TEST(test_construct_empty);
    RUN_TEST(test_construct_preset_string);
    RUN_TEST(test_construct_preset_char);
    RUN_TEST(test_char_pointer);
    RUN_TEST(test_equals_char);
    RUN_TEST(test_equals_char_array);
    RUN_TEST(test_equals_string);
    RUN_TEST(test_starts_with_char);
    RUN_TEST(test_starts_with_char_array);
    RUN_TEST(test_starts_with_string);
    RUN_TEST(test_ends_with_char);
    RUN_TEST(test_ends_with_char_array);
    RUN_TEST(test_ends_with_string);
    RUN_TEST(test_set_char);
    RUN_TEST(test_set_char_array);
    RUN_TEST(test_set_string);
    RUN_TEST(test_clear);
}
