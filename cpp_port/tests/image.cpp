#include "graphboard/image.hpp"

#include <cassert>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <system_error>
#include <vector>

using namespace graphboard;

int main() {
    Image img(3, 2);
    img.set(0, 0, 255, 0, 0);
    img.set(2, 1, 0, 0, 255);
    img.set(-1, 0, 9, 9, 9);   // out of range: must be ignored, not crash
    img.set(3, 0, 9, 9, 9);

    const auto path = std::filesystem::temp_directory_path() / "gb_image_test.png";
    assert(writePng(path, img));

    // Read the file back in its own scope so the stream is closed before remove.
    std::vector<std::uint8_t> bytes;
    {
        std::ifstream in(path, std::ios::binary);
        bytes.assign(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
    }
    assert(bytes.size() > 8);

    // PNG signature.
    const std::uint8_t sig[8] = {0x89, 'P', 'N', 'G', 0x0d, 0x0a, 0x1a, 0x0a};
    for (int i = 0; i < 8; ++i) assert(bytes[static_cast<std::size_t>(i)] == sig[i]);

    // Ends with an IEND chunk.
    const std::uint8_t iend[4] = {'I', 'E', 'N', 'D'};
    bool found = false;
    for (std::size_t i = 0; i + 4 <= bytes.size(); ++i) {
        if (bytes[i] == iend[0] && bytes[i + 1] == iend[1] &&
            bytes[i + 2] == iend[2] && bytes[i + 3] == iend[3]) {
            found = true;
            break;
        }
    }
    assert(found);

    // A zero-size image is rejected rather than writing a corrupt file.
    assert(!writePng(path, Image()));

    std::error_code ec;
    std::filesystem::remove(path, ec);
    return 0;
}
