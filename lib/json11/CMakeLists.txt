
# Source Files
file(GLOB json11_cpp_files src/*.cpp)
file(GLOB json11_hpp_files src/*.hpp)

### Project Settings
add_library(json11 STATIC ${json11_cpp_files} ${json11_hpp_files})
target_include_directories(json11 PUBLIC src)

assign_source_group(${json11_cpp_files})
assign_source_group(${json11_hpp_files})
