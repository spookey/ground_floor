#include "test.hpp"

void test_construct_empty() {
    Str8 str = Str8();

    TEST_ASSERT_EQUAL_size_t(8, str.size());
    TEST_ASSERT_EQUAL_size_t(0, str.length());
    TEST_ASSERT_TRUE(str.empty());
    TEST_ASSERT_TRUE(!str);
    TEST_ASSERT_EQUAL_size_t(8 - 0, str.left());

    TEST_ASSERT_EQUAL_CHAR_ARRAY(
        "\0\0\0\0\0\0\0\0\0",
        str.c_str(), 1 + 8
    );
}

void test_construct_preset_string() {
    Str16 str = Str16("string");

    TEST_ASSERT_EQUAL_size_t(16, str.size());
    TEST_ASSERT_EQUAL_size_t(6, str.length());
    TEST_ASSERT_FALSE(str.empty());
    TEST_ASSERT_FALSE(!str);
    TEST_ASSERT_EQUAL_size_t(16 - 6, str.left());

    TEST_ASSERT_EQUAL_CHAR_ARRAY(
        "string\0\0\0\0\0\0\0\0\0\0\0",
        str.c_str(), 1 + 16
    );
}

void test_construct_preset_char() {
    Str8 str = Str8('s');

    TEST_ASSERT_EQUAL_size_t(8, str.size());
    TEST_ASSERT_EQUAL_size_t(1, str.length());
    TEST_ASSERT_FALSE(str.empty());
    TEST_ASSERT_FALSE(!str);
    TEST_ASSERT_EQUAL_size_t(8 - 1, str.left());

    TEST_ASSERT_EQUAL_CHAR_ARRAY(
        "s\0\0\0\0\0\0\0\0\0",
        str.c_str(), 1 + 8
    );
}

void test_char_pointer() {
    Str8 nil = Str8();
    TEST_ASSERT_EQUAL_CHAR('\0', nil.c_str()[0]);
    TEST_ASSERT_EQUAL_CHAR('\0', (*nil)[0]);

    Str8 chr = Str8('s');
    TEST_ASSERT_EQUAL_CHAR('s', chr.c_str()[0]);
    TEST_ASSERT_EQUAL_CHAR('s', (*chr)[0]);
    TEST_ASSERT_EQUAL_CHAR('\0', chr.c_str()[1]);
    TEST_ASSERT_EQUAL_CHAR('\0', (*chr)[1]);

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

void test_set_char() {
    Str8 str = Str8('s');
    TEST_ASSERT_TRUE(str == 's');

    TEST_ASSERT_TRUE(str.set('c'));
    TEST_ASSERT_TRUE(str == 'c');

    TEST_ASSERT_FALSE(str.set("overflowing string"));
    TEST_ASSERT_TRUE(str == "overflow");
    TEST_ASSERT_EQUAL_CHAR_ARRAY(
        "overflow\0",
        str.c_str(), 1 + 8
    );
}

void test_set_char_array() {
    const char txt[] = {'s', 't', 'r', 'i', 'n', 'g', '\0'};
    const char cnt[] = {'c', 'o', 'n', 't', 'e', 'n', 't', '\0'};
    Str8 str = Str8(txt);
    TEST_ASSERT_TRUE(str == "string");

    TEST_ASSERT_TRUE(str.set(cnt));
    TEST_ASSERT_TRUE(str == "content");

    TEST_ASSERT_FALSE(str.set("overflowing string"));
    TEST_ASSERT_TRUE(str == "overflow");
    TEST_ASSERT_EQUAL_CHAR_ARRAY(
        "overflow\0",
        str.c_str(), 1 + 8
    );
}

void test_set_string() {
    Str8 str = Str8("string");
    TEST_ASSERT_TRUE(str == "string");

    TEST_ASSERT_TRUE(str.set("content"));
    TEST_ASSERT_TRUE(str == "content");

    TEST_ASSERT_FALSE(str.set("overflowing string"));
    TEST_ASSERT_TRUE(str == "overflow");
    TEST_ASSERT_EQUAL_CHAR_ARRAY(
        "overflow\0",
        str.c_str(), 1 + 8
    );
}

void test_clear() {
    Str16 str = Str16("string");

    TEST_ASSERT_EQUAL_size_t(16, str.size());
    TEST_ASSERT_EQUAL_size_t(6, str.length());
    TEST_ASSERT_EQUAL_size_t(16 - 6, str.left());
    TEST_ASSERT_FALSE(str.empty());
    TEST_ASSERT_FALSE(!str);

    TEST_ASSERT_TRUE(str.clear());

    TEST_ASSERT_EQUAL_size_t(16, str.size());
    TEST_ASSERT_EQUAL_size_t(0, str.length());
    TEST_ASSERT_EQUAL_size_t(16 - 0, str.left());
    TEST_ASSERT_TRUE(str.empty());
    TEST_ASSERT_TRUE(!str);
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
    TEST_ASSERT_FALSE(str.starts_with("struct"));

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

void test_append_char() {
    Str8 str = Str8('s');

    TEST_ASSERT_TRUE(str == 's');
    TEST_ASSERT_TRUE(str.append('c'));
    TEST_ASSERT_TRUE(str == "sc");

    TEST_ASSERT_EQUAL_CHAR_ARRAY(
        "sc\0\0\0\0\0\0\0",
        str.c_str(), 1 + 8
    );
}

void test_append_char_array() {
    const char txt[] = {'s', 't', 'r', 'i', 'n', 'g', '\0'};
    const char cnt[] = {'c', 'o', 'n', 't', 'e', 'n', 't', '\0'};
    Str16 str = Str16();

    TEST_ASSERT_TRUE(str == "");
    TEST_ASSERT_TRUE(str.append(txt));
    TEST_ASSERT_TRUE(str == "string");
    TEST_ASSERT_TRUE(str.append(cnt));
    TEST_ASSERT_TRUE(str == "stringcontent");

    TEST_ASSERT_EQUAL_CHAR_ARRAY(
        "stringcontent\0\0\0\0",
        str.c_str(), 1 + 16
    );
}

void test_append_string() {
    Str16 str = Str16("string");
    TEST_ASSERT_TRUE(str == "string");

    TEST_ASSERT_TRUE(str.append("content"));
    TEST_ASSERT_TRUE(str == "stringcontent");

    TEST_ASSERT_FALSE(str.append("overflow"));
    TEST_ASSERT_TRUE(str == "stringcontent");

    TEST_ASSERT_EQUAL_CHAR_ARRAY(
        "stringcontent\0\0\0\0",
        str.c_str(), 1 + 16
    );
}

void test_prepend_char() {
    Str8 str = Str8('s');

    TEST_ASSERT_TRUE(str == 's');
    TEST_ASSERT_TRUE(str.prepend('c'));
    TEST_ASSERT_TRUE(str == "cs");

    TEST_ASSERT_EQUAL_CHAR_ARRAY(
        "cs\0\0\0\0\0\0\0",
        str.c_str(), 1 + 8
    );
}

void test_prepend_char_array() {
    const char txt[] = {'s', 't', 'r', 'i', 'n', 'g', '\0'};
    const char cnt[] = {'c', 'o', 'n', 't', 'e', 'n', 't', '\0'};
    Str16 str = Str16();

    TEST_ASSERT_TRUE(str == "");
    TEST_ASSERT_TRUE(str.prepend(txt));
    TEST_ASSERT_TRUE(str == "string");
    TEST_ASSERT_TRUE(str.prepend(cnt));
    TEST_ASSERT_TRUE(str == "contentstring");

    TEST_ASSERT_EQUAL_CHAR_ARRAY(
        "contentstring\0\0\0\0",
        str.c_str(), 1 + 16
    );
}

void test_prepend_string() {
    Str16 str = Str16("string");
    TEST_ASSERT_TRUE(str == "string");

    TEST_ASSERT_TRUE(str.prepend("content"));
    TEST_ASSERT_TRUE(str == "contentstring");

    TEST_ASSERT_FALSE(str.prepend("overflow"));
    TEST_ASSERT_TRUE(str == "contentstring");

    TEST_ASSERT_EQUAL_CHAR_ARRAY(
        "contentstring\0\0\0\0",
        str.c_str(), 1 + 16
    );
}

void test_upper_lower() {
    Str8 chr = Str8('c');
    Str16 str = Str16("string");

    TEST_ASSERT_TRUE(chr == "c");
    TEST_ASSERT_TRUE(str == "string");

    chr.upper();
    str.upper();

    TEST_ASSERT_TRUE(chr == "C");
    TEST_ASSERT_TRUE(str == "STRING");

    chr.lower();
    str.lower();

    TEST_ASSERT_TRUE(chr == "c");
    TEST_ASSERT_TRUE(str == "string");

    TEST_ASSERT_EQUAL_CHAR_ARRAY(
        "c\0\0\0\0\0\0\0\0",
        chr.c_str(), 1 + 8
    );
    TEST_ASSERT_EQUAL_CHAR_ARRAY(
        "string\0\0\0\0\0\0\0\0\0\0\0",
        str.c_str(), 1 + 16
    );
}


void run() {
    RUN_TEST(test_construct_empty);
    RUN_TEST(test_construct_preset_string);
    RUN_TEST(test_construct_preset_char);
    RUN_TEST(test_char_pointer);
    RUN_TEST(test_set_char);
    RUN_TEST(test_set_char_array);
    RUN_TEST(test_set_string);
    RUN_TEST(test_clear);
    RUN_TEST(test_equals_char);
    RUN_TEST(test_equals_char_array);
    RUN_TEST(test_equals_string);
    RUN_TEST(test_starts_with_char);
    RUN_TEST(test_starts_with_char_array);
    RUN_TEST(test_starts_with_string);
    RUN_TEST(test_ends_with_char);
    RUN_TEST(test_ends_with_char_array);
    RUN_TEST(test_ends_with_string);
    RUN_TEST(test_append_char);
    RUN_TEST(test_append_char_array);
    RUN_TEST(test_append_string);
    RUN_TEST(test_prepend_char);
    RUN_TEST(test_prepend_char_array);
    RUN_TEST(test_prepend_string);
    RUN_TEST(test_upper_lower);
}
