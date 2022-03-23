#include "Utility.h"
#include "TestData.h"
#include <bitsery/bitsery.h>
#include <bitsery/adapter/buffer.h>
#include <bitsery/traits/array.h>
#include <bitsery/traits/string.h>
#include <bitsery/traits/vector.h>
#include <bitsery/ext/inheritance.h>
#include <bitsery/ext/value_range.h>


template<typename S>
void serialize(S& s, BaseData& o) {
    s.text1b(o.name, 16);
}

template<typename S>
void serialize(S& s, TestData& o) {
    s.container4b(o.floats);
//    s.container8b(o.doubles, 1e6);
    s.enableBitPacking([&o](typename S::BPEnabledType& sbp) {
        sbp.container(o.doubles, 1e6, [](typename S::BPEnabledType &sbp, uint64_t val) {
//            constexpr bitsery::ext::ValueRange<double> range{0., 1000., .1};
            constexpr bitsery::ext::ValueRange<uint64_t> range{0, 1023};
            sbp.ext(val, range);
        });
    });
    s.ext(o, bitsery::ext::BaseClass<BaseData>{});
}

int main() {
    START_BENCHMARKING_SESSION("bitsery");
    globalTestData.init();

    using Buffer = std::string;//std::vector<uint8_t>;
    using OutputAdapter = bitsery::OutputBufferAdapter<Buffer>;
    using InputAdapter = bitsery::InputBufferAdapter<Buffer>;

    auto serializeTest1 = [](const TestData &data) {
        PROFILE_SCOPE("Serialize TestData");
        Buffer buffer;
        auto writtenSize = bitsery::quickSerialization<OutputAdapter>(buffer, data);
        return std::make_tuple(buffer, writtenSize);
    };

    auto deserializeTest1 = [](const Buffer &buffer, size_t writtenSize) {
        PROFILE_SCOPE("Deserialize TestData");
        TestData res;
        auto state = bitsery::quickDeserialization<InputAdapter>({buffer.begin(), writtenSize}, res);
        assert(state.first == bitsery::ReaderError::NoError && state.second);

        return res;
    };

    uint32_t bufferSize = 0;
    START_BENCHMARKING_ITERATIONS(10) {
        PRINT_BENCHMARKING_ITERATION();
        auto [buffer, writtenSize] = serializeTest1(globalTestData);
        bufferSize = writtenSize;
        auto res = deserializeTest1(buffer, writtenSize);

        if(!testSerializedData(res)) {
            printf(RED("Error in bitsery!\n"));
            return 0;
        }
    }
    printf("Serialised Data Size: " GREEN("%u") " Bytes\n", bufferSize);
}
