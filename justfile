default: test

# Compile and run both C11 and C++ test suites
test: test-c test-cpp

# Compile and run the test suite in C11 mode
test-c:
    gcc -std=c11 -Wall -Wextra -O2 test.c -o test_c
    ./test_c

# Compile and run the test suite in C++ mode
test-cpp:
    g++ -std=c++17 -Wall -Wextra -O2 -x c++ test.c -o test_cpp
    ./test_cpp

# Remove compiled test binaries
clean:
    rm -f test_c test_cpp
