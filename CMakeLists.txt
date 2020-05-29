cmake_minimum_required(VERSION 3.5.1)
project(compiler)

set(CMAKE_CXX_STANDARD 14)
add_definitions(-std=c++11)

find_package(BISON)
find_package(FLEX)

BISON_TARGET(MyParser src/parser.y ${CMAKE_CURRENT_BINARY_DIR}/compiler.cpp)
FLEX_TARGET(MyScanner src/c_lexer.l ${CMAKE_CURRENT_BINARY_DIR}/tokens.cpp)
ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

include_directories(
    include
    3rdParty
    ${CMAKE_CURRENT_BINARY_DIR}
)


add_executable(
    ${PROJECT_NAME}
    ${BISON_MyParser_OUTPUTS} 
    ${FLEX_MyScanner_OUTPUTS}
    src/main.cpp
    src/irCodeGen.cpp
    src/parserTreeNode.cpp
    src/saveTree.cpp
)

target_link_libraries(
    ${PROJECT_NAME}
)