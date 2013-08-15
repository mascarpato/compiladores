CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT(compilador)
FIND_PACKAGE(FLEX REQUIRED)
FIND_PACKAGE(BISON REQUIRED)
FLEX_TARGET(scanner scanner.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.c)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${compilador_SOURCE_DIR}/include/)
INCLUDE_DIRECTORIES(${compilador_SOURCE_DIR}/src/)
ADD_EXECUTABLE(main
  ${FLEX_scanner_OUTPUTS}
  src/main.c
  src/comp_dict.c
  src/comp_list.c
  src/comp_tree.c
  src/comp_graph.c
)
TARGET_LINK_LIBRARIES(main ${FLEX_LIBRARIES})

# Marcelo -- Tests List Functions
ADD_EXECUTABLE(test_list
  tests/test_list.c
  src/comp_list.c)
# Marcelo -- Tests Dictionary Functions
ADD_EXECUTABLE(test_dic
	tests/test_dict.c
	src/comp_dict.c)  
# Mauricio -- Tests Tree Functions
ADD_EXECUTABLE(test_tree
  tests/test_tree.c
  src/comp_tree.c)
# Diego -- Tests Graph
ADD_EXECUTABLE(test_graph
  tests/test_graph.c
  src/comp_graph.c)
# Marcelo -- Enables Debug information
ADD_DEFINITIONS(-g)

