
# Source Files
file(GLOB logger_cpp_files src/*.cpp)
file(GLOB logger_hpp_files src/*.hpp)
file(GLOB logger_test_cpp_files test/*.cpp)

### Project Settings
add_library(logger STATIC ${logger_cpp_files} ${logger_hpp_files})
add_executable(logger-test ${logger_cpp_files} ${logger_hpp_files} ${logger_test_cpp_files})

target_include_directories(logger-test PUBLIC src)

assign_source_group(${logger_cpp_files})
assign_source_group(${logger_hpp_files})
assign_source_group(${logger_test_cpp_files})
