cmake_minimum_required(VERSION 3.19)
project(db)

set(CMAKE_CXX_STANDARD 17)

include_directories(Include)

include_directories(Include/db)
aux_source_directory(Source/db DB_DIR)

include_directories(Include/dbms)
aux_source_directory(Source/dbms DBMS_DIR)

include_directories(Include/exceptions)
aux_source_directory(Source/exceptions EXCEPTIONS_DIR)

include_directories(Include/Info)
aux_source_directory(Source/info INFO_DIR)

include_directories(Include/tools)
aux_source_directory(Source/tools TOOL_DIR)


add_executable(db Source/main.cpp ${DB_DIR} ${DBMS_DIR} ${EXCEPTIONS_DIR} ${INFO_DIR} ${TOOL_DIR})