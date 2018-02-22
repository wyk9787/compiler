SRC=src/compiler.cpp src/expression.cpp src/parser_driver.cpp src/interpreter.cpp
LEX_OUTPUT=src/scanner.yy.cpp
BISON_OUTPUT=src/parser.yy.cpp
BISON_AUX=src/location.hh src/parser.yy.hpp src/position.hh src/stack.hh src/parser.h
CC=clang++
CC_FLAGS=-std=c++14 -O3 -Wall -Wno-deprecated-register
INCLUDE=-Iinclude/ -Isrc/
BUILD=./build

.PHONY : all compiler clean test

all: compiler

test:
	./test/test.sh

compiler : ${LEX_OUTPUT} ${BISON_OUTPUT} ${SRC}
	${CC} ${INCLUDE} -o ${BUILD}/$@ ${CC_FLAGS} $^

src/scanner.yy.cpp : src/scanner.l
	flex -o $@ $<

src/parser.yy.cpp : src/parser.yy
	bison -o $@ $<

clean :
	rm -rf compiler
	rm -rf ${BISON_OUTPUT} ${BISON_AUX}
	rm -rf ${LEX_OUTPUT}
