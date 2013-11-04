CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT(compilador)
FIND_PACKAGE(FLEX REQUIRED)
FIND_PACKAGE(BISON 2.5 REQUIRED)
FLEX_TARGET(scanner scanner.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.c)
BISON_TARGET(parser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.c VERBOSE parser.output)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${compilador_SOURCE_DIR}/include/)
INCLUDE_DIRECTORIES(${compilador_SOURCE_DIR}/src/)
ADD_EXECUTABLE(main
  src/comp_symbols.c
  src/comp_dict.c
  src/iks_ast.c
  src/comp_tree.c
  src/comp_list.c
  src/comp_graph.c
  src/tac.c
  src/code.c
  ${FLEX_scanner_OUTPUTS}
  ${BISON_parser_OUTPUTS}
  src/main.c
  src/gv.c
  src/semantic.c
)
TARGET_LINK_LIBRARIES(main ${FLEX_LIBRARIES})

# Marcelo -- Tests List Functions
#ADD_EXECUTABLE(test_list
#  tests/test_list.c
#  src/comp_list.c)
# Marcelo -- Tests Dictionary Functions
#ADD_EXECUTABLE(test_dic
#	tests/test_dict.c
#	src/comp_dict.c)  
# Mauricio -- Tests Tree Functions
#ADD_EXECUTABLE(test_tree
#  tests/test_tree.c
#  src/comp_tree.c)
# Diego -- Tests Graph
#ADD_EXECUTABLE(test_graph
#  tests/test_graph.c
#  src/comp_graph.c)

# Marcelo -- Enables Debug information
ADD_DEFINITIONS(-g)

#ADD_EXECUTABLE(auto_test_e1
#  ${FLEX_scanner_OUTPUTS}
#  ${BISON_parser_OUTPUTS}
#  tests/main_aval_e1.c
#  src/comp_dict.c
#  src/comp_list.c
#  src/comp_tree.c
#  src/comp_graph.c
#)
#TARGET_LINK_LIBRARIES(auto_test_e1 ${FLEX_LIBRARIES})
