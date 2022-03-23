#include "Utility.h"
#include "TestData.h"
#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/base_class.hpp>

template<class Archive>
void serialize(Archive &archive, BaseData &obj) {
    archive(obj.name);
}

template<class Archive>
void serialize(Archive &archive, TestData &obj) {
    archive(cereal::base_class<BaseData>(&obj), obj.floats, obj.doubles);
}

int main() {
    START_BENCHMARKING_SESSION("cereal");

    using Buffer = std::stringstream;
    auto serializeTest1 = [](const TestData &data, Buffer &buffer) {
        PROFILE_SCOPE("Serialize TestData");
        cereal::BinaryOutputArchive binaryOutputArchive(buffer);
        binaryOutputArchive(data);
    };

    auto deserializeTest1 = [](const Buffer &buffer) {
        PROFILE_SCOPE("Deserialize TestData");
        TestData res;
        cereal::BinaryInputArchive binaryInputArchive((std::istream &) buffer);
        binaryInputArchive(res);
        return res;
    };

    uint32_t bufferSize = 0;
    START_BENCHMARKING_ITERATIONS(10) {
        PRINT_BENCHMARKING_ITERATION();
        Buffer buffer;
        serializeTest1(globalTestData, buffer);
        bufferSize = buffer.str().size();
        auto res = deserializeTest1(buffer);
        if(!testSerializedData(res)) {
            printf(RED("Error in cereal!\n"));
            return 0;
        }
    }
    printf("Serialised Data Size: " GREEN("%u") " Bytes\n", bufferSize);
}
