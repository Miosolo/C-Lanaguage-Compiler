cmake_minimum_required(VERSION 3.0.0)
project(expr-parser VERSION 0.1.0)

aux_source_directory(. DIR_SRCS)

add_executable(expr.parser ${DIR_SRCS})
