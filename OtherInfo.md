# CMake and Testing Environment in C (CLion)

## What is CMake and Why Use It?
CMake is an **open-source build automation tool** that helps:
- Generate platform-specific build files (Makefiles, IDE projects, etc.)
- Manage complex dependencies and configurations
- Support multiple build configurations (Debug/Release)
- Simplify cross-platform compilation

It uses `CMakeLists.txt` configuration files to define:
- Project structure
- External dependencies
- Compilation options
- Executable/library targets

## Basic CMakeLists.txt Structure

```cmake
cmake_minimum_required(VERSION 3.20)
project(MyProject C)

set(CMAKE_C_STANDARD 11)

add_executable(main_program
    src/main.c
    src/utils.c
    include/utils.h
)
```

## Testing Environment in C
CLion supports several testing frameworks through CMake integration. Common choices:

1. **Check** (https://libcheck.github.io/check/)
2. **Unity** (https://www.throwtheswitch.org/unity)
3. **Google Test** (primarily for C++ but can wrap C code)

### CMake Configuration for Testing (Check Example)

```cmake
# Add to CMakeLists.txt
find_package(Check REQUIRED)
include_directories(${CHECK_INCLUDE_DIRS})

add_executable(test_program
    tests/test_utils.c
    src/utils.c
)

target_link_libraries(test_program ${CHECK_LIBRARIES})

enable_testing()
add_test(NAME test_utils COMMAND test_program)
```

### Sample Check Test Implementation

```c name=test_utils.c
#include <check.h>
#include "utils.h"

START_TEST(test_sum) {
    ck_assert_int_eq(sum(2, 3), 5);
}
END_TEST

Suite *utils_suite(void) {
    Suite *s = suite_create("Utils");
    TCase *tc_core = tcase_create("Core Functions");

    tcase_add_test(tc_core, test_sum);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void) {
    SRunner *sr = srunner_create(utils_suite());
    srunner_run_all(sr, CK_NORMAL);

    int failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
```

## CLion Integration
1. **Typical Project Structure**:

```
project/
├── CMakeLists.txt
├── src/
│   └── *.c
├── include/
│   └── *.h
└── tests/
    └── test_*.c
```

2. **Key CLion Features**:
   - Automatic CMake project recognition
   - Built-in test runner
   - Integrated debugger for tests
   - Code coverage analysis
   - Testing framework support

3. **Running Tests**:
   - Open the test file.
   - Click the ▶️ icon next to test cases.
   - View results in the "Run" tool window.

## Build Folder (cmake-build-debug) Contents and Purpose
The `cmake-build-debug` directory is automatically generated by CMake and the associated build tools (e.g., Ninja or Make) during the build process. It contains various files and directories essential for configuring, building, and testing your project. Below are some of its key components:

1. **.cmake (Directory)**  
   Contains CMake-specific configuration files that help manage build settings across multiple runs.

2. **.ninja_deps (File)**  
   A file generated by Ninja containing dependency information for source files, ensuring that only modified components are recompiled.

3. **.ninja_log (File)**  
   A log file created by Ninja that records build actions, which is useful for debugging and analyzing build performance.

4. **API (File)**  
   Could be a binary or data file produced during the build process. Its exact role depends on the specifics of the project.

5. **API.cbp (File)**  
   A Code::Blocks project file generated by CMake when configured to support the Code::Blocks IDE.

6. **API.exe (File)**  
   The executable resulting from compiling the project.

7. **CMakeCache.txt (File)**  
   A cache file that stores all configuration variables and options set during the CMake configuration process.

8. **CMakeFiles (Directory)**  
   Contains internal CMake files and subdirectories required for the build process, including dependency tracking and internal build rules.

9. **Makefile (File)**  
   A Makefile generated by CMake, used by the `make` tool to compile the project.

10. **Testing (Directory)**  
    May include files related to tests, especially when tests are configured via CTest.

11. **build.ninja (File)**  
    A build file for Ninja that outlines the instructions, rules, and dependencies needed to compile the project.

12. **cmake_install.cmake (File)**  
    Contains installation instructions generated by CMake, used when running the install command (e.g., `make install` or its Ninja equivalent).

## Benefits of CLion+CMake Workflow
- ✅ Centralized configuration
- ✅ Cross-platform compatibility
- ✅ Automated dependency management
- ✅ Advanced debugging support
- ✅ Integrated static analysis

## Useful Resources
- Official CMake Documentation: https://cmake.org/documentation/
- CLion Testing Guide: https://www.jetbrains.com/help/clion/unit-testing.html
- CMake Examples Repository: https://github.com/ttroy50/cmake-examples
