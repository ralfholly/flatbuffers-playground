set(TARGET_EXE CurrentDateTimeTest)
set(FBS_FILE current_date_time.fbs)

find_package(Flatbuffers REQUIRED)

include_directories(${CMAKE_BINARY_DIR})
add_executable(${TARGET_EXE}
    CurrentDateTimeTest.cpp
    )

target_link_libraries(${TARGET_EXE} flatbuffers::flatbuffers)

# add_dependencies(${TARGET_EXE} fbs_target)
# 
# build_flatbuffers(
#     current_date_time.fbs
#     ${CMAKE_CURRENT_SOURCE_DIR}
#     fbs_target
#     ""
#     ""
#     ""
#     ""
#     )
