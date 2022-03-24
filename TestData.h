#ifndef SERIALIZATION_BENCHMARKS_TESTDATA_H
#define SERIALIZATION_BENCHMARKS_TESTDATA_H


#include <vector>

struct BaseData {
    std::string name;
    BaseData(std::string name): name(name) {}
};

struct TestData: public BaseData {
    float floats[size_t(1e6)];
    std::vector<double> doubles;
    explicit TestData(std::string name = "", size_t size = 0, bool init = false): BaseData(name) {
        doubles.resize(size, 0);
        if(init) this->init();
    }

    void init() {
        for(size_t i = 0; i < std::size(floats); ++i) {
            floats[i] = i%1000;
            if(i < std::size(doubles)) doubles[i] = i%1000;
        }
    }
};

TestData globalTestData("qwerty", 1e6, true);

bool testSerializedData(TestData &testData) {
    bool errorFreeInheritance = true;
    if(testData.name != "qwerty") {
        printf(RED("Testing Serialised Data: Inheritance Fail!\n"));
        errorFreeInheritance = false;
    }

    bool errorFreeFloats = true, errorFreeDouble = true;
    for(size_t i = 0; i < std::size(testData.floats); ++i) {
        if(errorFreeFloats and testData.floats[i] != (i%1000)) {
            printf(RED("Testing Serialised Data: Static array Fail!\n"));
            errorFreeFloats = false;
        }
        if(errorFreeDouble and ((testData.doubles.size() != std::size(testData.floats) or testData.doubles[i] != (i%1000)))) {
            printf(RED("Testing Serialised Data: std::vector Fail!\n"));
            errorFreeDouble = false;
        }
        if(!errorFreeFloats and !errorFreeDouble) break;
    }

    return errorFreeInheritance and errorFreeFloats and errorFreeDouble;
}


#endif //SERIALIZATION_BENCHMARKS_TESTDATA_H
