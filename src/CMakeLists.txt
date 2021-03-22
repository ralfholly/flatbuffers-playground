set(TARGET_EXE CurrentDateTimeTest)
set(PROTO_FILE current_date_time.proto)

find_package(Protobuf REQUIRED)

include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(${TARGET_EXE}
    CurrentDateTimeTest.cpp
    ${PROTO_SRCS}
    ${PROTO_HDRS}
    )

target_link_libraries(${TARGET_EXE} protobuf::protobuf)

protobuf_generate(
    LANGUAGE cpp
    TARGET ${TARGET_EXE}
    PROTOS ${PROTO_FILE}
    )
