cmake_minimum_required(VERSION 3.0.0)
project(gram-parser VERSION 0.1.0)

add_executable(gram.checker gram.lex.c rdcheck.c)
add_executable(gram.parser gram.lex.c rdparse.c)
