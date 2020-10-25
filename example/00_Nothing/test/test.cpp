#include "test.hpp"

void run() {
    Nothing none = Nothing();

    TEST_ASSERT_TRUE(none.is(true));
    TEST_ASSERT_FALSE(none.is(false));
}
