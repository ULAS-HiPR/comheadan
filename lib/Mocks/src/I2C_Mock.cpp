#include <I2C_Mock.h>
#include <cstddef>

void I2C_Mock::write(int addr, uint8_t* data, std::size_t len) {
    for (uint i = 0; i < len; ++i) {
        write_buffer.push_back(static_cast<int>(data[i]));
    }
}

void I2C_Mock::read(int addr, uint8_t data, uint8_t* buf, std::size_t len) {
    for (uint i = 0; i < len; ++i) {
        if (i < read_buffer.size()) {
            buf[i] = static_cast<uint8_t>(read_buffer[i]);
        } else {
            buf[i] = 0; 
        }

    }
}

const std::vector<int>& I2C_Mock::getWriteBuffer() const {
    return write_buffer;
}