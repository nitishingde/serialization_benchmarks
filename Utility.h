#ifndef SERIALIZATION_BENCHMARKS_UTILITY_H
#define SERIALIZATION_BENCHMARKS_UTILITY_H


#include <chrono>
#include <string>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_CLEAR_LINE    "\33[2K"

#define RED(x) ANSI_COLOR_RED x ANSI_COLOR_RESET
#define GREEN(x) ANSI_COLOR_GREEN x ANSI_COLOR_RESET
#define YELLOW(x) ANSI_COLOR_YELLOW x ANSI_COLOR_RESET
#define BLUE(x) ANSI_COLOR_BLUE x ANSI_COLOR_RESET
#define MAGENTA(x) ANSI_COLOR_MAGENTA x ANSI_COLOR_RESET
#define CYAN(x) ANSI_COLOR_CYAN x ANSI_COLOR_RESET

namespace benchmark {
    /**
     * RAII
     */
    class BenchMarker {
    public:
        explicit BenchMarker(const char *pName);
        ~BenchMarker();
    };

    /**
     * RAII
     */
    class Timer {
    private:
        std::string mName;
        std::chrono::high_resolution_clock::time_point mStartPoint;
    public:
        explicit Timer(const char *pName);
        ~Timer();
    };
}

#define PROFILING 1
#if PROFILING
#define TOKEN_PASTE_(x, y) x##y
#define CONCAT_(x, y) TOKEN_PASTE_(x, y)

#define START_BENCHMARKING_SESSION(name) benchmark::BenchMarker CONCAT_(benchMarker, __LINE__)(name)
#define START_BENCHMARKING_ITERATIONS(iterations) \
    for(uint32_t svp_iteration = 0, svp_iterations = iterations; svp_iteration < svp_iterations; ++svp_iteration)

#define PRINT_BENCHMARKING_ITERATION()                      \
    printf(ANSI_CLEAR_LINE "Iteration : %u/%u\r", svp_iteration+1, svp_iterations); \
    fflush(stdout)

#define PROFILE_SCOPE(name) benchmark::Timer CONCAT_(timer, __LINE__)(name)
#define PROFILE_FUNC() PROFILE_SCOPE(__PRETTY_FUNCTION__)
#else
#define START_BENCHMARKING_SESSION(name)
    #define START_BENCHMARKING_ITERATIONS(iterations)
    #define PRINT_BENCHMARKING_ITERATION()
    #define PROFILE_SCOPE(name)
    #define PROFILE_FUNC()
#endif


#endif //SERIALIZATION_BENCHMARKS_UTILITY_H
