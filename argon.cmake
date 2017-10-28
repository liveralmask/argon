find_package(BISON)
find_package(FLEX)
BISON_TARGET(PARSER parser.y parser.c)
FLEX_TARGET(LEXER lexer.l lexer.c)
ADD_FLEX_BISON_DEPENDENCY(LEXER PARSER)
include_directories(${CMAKE_CURRENT_LIST_DIR})
set(ARGON_SRCS
  ${BISON_PARSER_OUTPUTS}
  ${FLEX_LEXER_OUTPUTS}
  ${CMAKE_CURRENT_LIST_DIR}/argon_parser.c
  ${CMAKE_CURRENT_LIST_DIR}/argon_runner.c
)
