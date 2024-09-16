cmake_minimum_required(VERSION 3.30)
project(metal_demo)
set(CMAKE_CXX_STANDARD 17)

include_directories(include 
                    vendor/metal-cpp 
                    vendor/metal-cpp-extensions 

                    vendor/glfw/include

                    vendor/imgui/
                    vendor/imgui/backends

                    vendor/stb/

                    vendor/spdlog/include

                    vendor/tinygltf/
                    )
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/spdlog)

add_compile_definitions(IMGUI_IMPL_METAL_CPP)
file(GLOB all_sources CONFIGURE_DEPENDS
    "src/*.cpp"
    "src/*.mm"
    "src/*.m"
    "src/vendor_impls/*.cpp"
    "src/vendor_impls/*.mm"
    "src/vendor_impls/*.m"
    # ImGui
    "vendor/imgui/*.cpp"
    "vendor/imgui/backends/imgui_impl_glfw.cpp"
    "vendor/imgui/backends/imgui_impl_metal.mm"
)
add_executable(${PROJECT_NAME} ${all_sources})
target_link_libraries(${PROJECT_NAME}
                                    "-framework Metal"
                                    "-framework MetalKit"
                                    "-framework AppKit"
                                    "-framework Foundation"
                                    "-framework QuartzCore")

target_link_libraries(${PROJECT_NAME} glfw)
add_custom_target(copy_shaders ALL
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${PROJECT_SOURCE_DIR}/shaders
            ${PROJECT_BINARY_DIR}/shaders
            COMMENT "Copying shaders to build directory"
)
add_custom_target(copy_assets ALL
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${PROJECT_SOURCE_DIR}/data
            ${PROJECT_BINARY_DIR}/data
            COMMENT "Copying assets to build directory"
)

## Testing 

include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB all_tests CONFIGURE_DEPENDS
    "test/*.cpp"
)
add_executable(metal_demo_test ${all_tests})
target_link_libraries(metal_demo_test GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(metal_demo_test)