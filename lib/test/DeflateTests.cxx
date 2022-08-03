#include <gtest/gtest.h>

#include "Deflator.h"

std::string lorum = { // NOLINT
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Duis ex odio, finibus sed lectus et, " \
        "pharetra eleifend sem. Donec quam ipsum, volutpat eu porttitor quis, tempor consectetur nisl. Cras bibendum " \
        "justo vitae velit aliquam, et eleifend justo cursus. Vivamus non scelerisque felis. Ut efficitur diam ut mi " \
        "euismod convallis. Suspendisse ac venenatis neque. Fusce rhoncus porta nisl non rutrum. Etiam pulvinar " \
        "libero eu leo porttitor, vitae pellentesque lectus sollicitudin. Donec ullamcorper a erat vel maximus. " \
        "Aliquam dignissim turpis ut pellentesque lacinia. Ut neque nunc, eleifend nec sagittis eget, volutpat at ex. " \
        "Morbi luctus ipsum sit amet facilisis pharetra. Pellentesque orci nunc, fringilla ut arcu in, tincidunt " \
        "convallis nisi. Vestibulum sem ligula, ultricies eu iaculis interdum, tempor at turpis."
};

std::vector<uint8_t> lorumBytes = { lorum.begin(), lorum.end() }; // NOLINT

/*
 * Just make sure we can, using raw AVRO, actually round-trip an AppMessage
 */
TEST (xflate, deflate) { // NOLINT
    auto compressed = deflate(lorumBytes, 2);

    std::cout << lorum.size() << std::endl;
    std::cout << compressed.size() << std::endl;

    std::string gibberish { compressed.begin(), compressed.end() };
    std::cout << R"(")" << gibberish << R"(")" << std::endl;
}

TEST (xflate, inflate) { // NOLINT
    auto compressed = deflate (lorumBytes, 2);
    auto decompressed = inflate(compressed);

    std::cout << lorumBytes[0] << " " << lorumBytes[1] << std::endl;
    std::cout << compressed[0] << " " << compressed[1] << std::endl;
    std::cout << decompressed[0] << " " << decompressed[1] << std::endl;
    for (const auto & i: decompressed) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    std::string msg = { decompressed.begin(), decompressed.end() };

    std::cout << msg << std::endl;

}