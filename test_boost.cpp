#include "Utility.h"
#include "TestData.h"
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <sstream>

template<class Archive>
void serialize(Archive &ar, BaseData &o, const unsigned int version) {
    ar & o.name;
}

template<class Archive>
void serialize(Archive &ar, TestData &o, const unsigned int version) {
    ar & boost::serialization::base_object<BaseData>(o);
    ar & o.floats;
    ar & o.doubles;
}

int main() {
    START_BENCHMARKING_SESSION("boost");

    using Buffer = std::stringstream;
    auto serializeTest1 = [](const TestData &data) {
        PROFILE_SCOPE("Serialize TestData");
        Buffer buffer;
        boost::archive::binary_oarchive bo(buffer);
        bo << data;

        return buffer;
    };

    auto deserializeTest1 = [](const Buffer &buffer) {
        PROFILE_SCOPE("Deserialize TestData");
        TestData res;
        boost::archive::binary_iarchive bi((std::istream &) buffer);
        bi >> res;

        return res;
    };

    uint32_t bufferSize = 0;
    START_BENCHMARKING_ITERATIONS(10) {
        PRINT_BENCHMARKING_ITERATION();
        auto buffer = serializeTest1(globalTestData);
        bufferSize = buffer.str().size();
        auto res = deserializeTest1(buffer);
        if(!testSerializedData(res)) {
            printf(RED("Error in boost!\n"));
            return 0;
        }
    }
    printf("Serialised Data Size: " GREEN("%u") " Bytes\n", bufferSize);
}
