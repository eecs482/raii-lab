#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include <bit>

#include "../cpu.h"
#include "../mutex.h"

#include "lock_guard.hpp"

/**
 * INSTRUCTIONS
 *
 * 1) Run “make file_size_counter” to build
 * 2) Run “./file_size_counter disk.in0 disk.in1 disk.in2 disk.in3 disk.in4” to get a sense for the program
 * 3) Examine file_size_counter.cpp
 * 4) Can you give an input to this program that fails to print a line for every given file path? Why did it fail?
 * 5) Fix the source code without RAII
 * 6) Implement lock_guard.hpp and change the fix to use that
 */

// Assume our filesystem requires a lock to be thread-safe
// This is for the sake of example and is not true on Linux
mutex file_system_lock;
std::vector<std::filesystem::path> file_paths;

void print_file_size(std::filesystem::path const& file_path)
{
    // RAII solution: Use your lock guard
    // The destructor of the lock guard (which calls unlock) is garunteed to be called once this scope is exited
    // This includes even if an exception is thrown
    lock_guard guard{file_system_lock};

    std::ifstream file_stream;
    file_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit); // Throw exceptions on error

    file_stream.open(file_path);
    file_stream.seekg(0, std::ios::end);
    std::cout << file_path << " is " << file_stream.tellg() << " bytes" << std::endl;
}

void worker(void* argument)
{
    std::filesystem::path const& file_path = file_paths[std::bit_cast<std::size_t>(argument)];
    try
    {
        print_file_size(file_path);
    }
    catch (std::exception const& e)
    {
        // non-RAII solution would require us to unlock the mutex here:
        // file_system_lock.unlock();
        // This solution should ring alarm bells in your head!
        // You have logic in a completely different place of code
        // You also have two unlocks per lock to keep track of
        std::cerr << "Skipping file with io error: " << file_path << ": " << e.what() << std::endl;
    }
}

void boot(void*)
{
    for (std::size_t i = 0; i < file_paths.size(); ++i)
    {
        // "reinterpret_cast" can also be used instead of "std::bit_cast"
        // The latter is prefered since it checks that the size of the to and from type are the same at compile time
        thread{worker, std::bit_cast<void*>(i)};
    }
}

int main(int argc, char* argv[])
{
    file_paths = {argv + 1, argv + argc};
    cpu::boot(boot, nullptr, 0);
}
