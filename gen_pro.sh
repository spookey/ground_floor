#!/usr/bin/env sh

SELF=$(cd "$(dirname "$0")" || exit 1; pwd)
[ "$SELF" != "$(pwd)" ] && { echo "Launch from base directory!"; exit 1; }

NAME="$1"
[ -z "$NAME" ] && { echo "Please supply a name."; exit 1; }
case $NAME in  -*) echo "Just the name, no parameters!"; exit 1 ;; esac

BASE="example/$NAME"
SRC_DIR="$BASE/src"
TST_DIR="$BASE/test"
LIB_DIR="$BASE/lib/ground_floor"

[ ! -d "$SRC_DIR" ] && mkdir -vp "$SRC_DIR"
[ ! -d "$TST_DIR" ] && mkdir -vp "$TST_DIR"
[ ! -d "$LIB_DIR" ] && mkdir -vp "$LIB_DIR"


[ ! -e "$SRC_DIR/main.hpp" ] && cat <<'MAIN_HPP' > "$SRC_DIR/main.hpp"
#ifndef _main_hpp_
#define _main_hpp_

#include <Arduino.h>

void setup();
void loop();

#endif
MAIN_HPP

[ ! -e "$SRC_DIR/main.cpp" ] && cat <<'MAIN_CPP' > "$SRC_DIR/main.cpp"
#include "main.hpp"

void setup() {};

void loop() {};
MAIN_CPP


[ ! -e "$TST_DIR/test.hpp" ] && cat <<'TEST_HPP' > "$TST_DIR/test.hpp"
#ifndef _test_hpp_
#define _test_hpp_

#include <Arduino.h>
#include <unity.h>

void run();

void setup() {
    UNITY_BEGIN();
    run();
    UNITY_END();
}

void loop() {}

#endif
TEST_HPP

[ ! -e "$TST_DIR/test.cpp" ] && cat <<'TEST_CPP' > "$TST_DIR/test.cpp"
#include "test.hpp"

void run() {
    TEST_ASSERT_TRUE(true);
}
TEST_CPP


cd "$BASE" || exit 2
[ ! -e ".gitignore" ] && [ -e "../../.gitignore" ] && \
    ln -s "../../.gitignore" ".gitignore"

[ ! -e "platformio.ini" ] && [ -e "../../platformio_common.ini" ] && \
    ln -s "../../platformio_common.ini" "platformio.ini"
cd "$SELF" || exit 2

cd "$LIB_DIR" || exit 3
[ ! -e "src" ] && [ -d "../../../../src" ] && \
    ln -s "../../../../src" "src"
cd "$SELF" || exit 3

exit 0
