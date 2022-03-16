#include "Utility.h"
#include <vector>
#include <numeric>

namespace benchmark {
    struct ProfileResult {
        std::string name;
        double elapsedTime;
    };

    class Profiler {
    private:
        Profiler() = default;
        std::vector<std::pair<std::string, std::vector<double>>> mExecutions;
        std::string mSession;

    private:
        void flush() const;
    public:
        ~Profiler();
        static Profiler *getInstance();
        void startSession(const char *pSession);
        void endSession();
        bool isSessionActive() const;
        void log(const ProfileResult &profileResult);
    };
}

static benchmark::Profiler *pInstance = nullptr;

benchmark::Profiler::~Profiler() {
    endSession();
}

benchmark::Profiler* benchmark::Profiler::getInstance() {
    if(pInstance == nullptr) {
        pInstance = new Profiler();
    }
    return pInstance;
}

void benchmark::Profiler::flush() const {
    printf("|----------------------------------|------------------------------------------------------------------|------------|--------------|--------------|--------------|\n");
    printf(
            "| "
            MAGENTA("Session") "                          | "
            MAGENTA("Scope") "                                                            | "
            MAGENTA("Iterations") " | "
            MAGENTA("Avg") "          | "
            MAGENTA("Min") "          | "
            MAGENTA("Max") "          |\n"
    );
    printf("|----------------------------------|------------------------------------------------------------------|------------|--------------|--------------|--------------|\n");

    for(const auto &it: mExecutions) {
        auto &executionTime = it.second;
        printf(
                "| " CYAN("%-32.32s") " | " YELLOW("%-64.64s") " | %-10zu | " BLUE("%.9f") "s | " GREEN("%.9f") "s | " RED("%.9f") "s |\n",
                mSession.c_str(),
                it.first.c_str(),
                executionTime.size(),
                std::accumulate(executionTime.begin(), executionTime.end(), 0.0)/executionTime.size(),
                *std::min_element(executionTime.begin(), executionTime.end()),
                *std::max_element(executionTime.begin(), executionTime.end())
        );
    }

    printf("|----------------------------------|------------------------------------------------------------------|------------|--------------|--------------|--------------|\n\n");
}

void benchmark::Profiler::startSession(const char *pSession) {
    if(!mSession.empty() and mSession != pSession) endSession();
    mSession = pSession;
}

void benchmark::Profiler::endSession() {
    flush();
    mSession.clear();
    mExecutions.clear();
}

bool benchmark::Profiler::isSessionActive() const {
    return !mSession.empty();
}

void benchmark::Profiler::log(const ProfileResult &profileResult) {
    if(!isSessionActive()) return;
    for(auto &execution: mExecutions) {
        if(execution.first == profileResult.name) {
            execution.second.push_back(profileResult.elapsedTime);
            return;
        }
    }
    // couldn't find it
    mExecutions.emplace_back(std::make_pair(profileResult.name, std::vector<double>{profileResult.elapsedTime}));
}

benchmark::BenchMarker::BenchMarker(const char *pName) {
    Profiler::getInstance()->startSession(pName);
}

benchmark::BenchMarker::~BenchMarker() {
    Profiler::getInstance()->endSession();
}

benchmark::Timer::Timer(const char *pName)
        : mName(pName)
        , mStartPoint(std::chrono::high_resolution_clock::now()) {
}

benchmark::Timer::~Timer() {
    auto endPoint = std::chrono::high_resolution_clock::now();
    Profiler::getInstance()->log(ProfileResult {
            .name = mName,
            .elapsedTime = double(std::chrono::duration_cast<std::chrono::nanoseconds>(endPoint - mStartPoint).count()) / 1.e9
    });
}
