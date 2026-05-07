#include <I2C_Mock.h>
#include <cstddef>

bool I2C_Mock::write(int addr, uint8_t* data, size_t len) {
    for (int i = 0; i < len; ++i) {
        write_buffer.push_back(static_cast<int>(data[i]));
    }
    return true;
}

bool I2C_Mock::read(int addr, uint8_t data, uint8_t* buf, size_t len) {
    for (int i = 0; i < len; ++i) {
        if (i < read_buffer.size()) {
            buf[i] = static_cast<uint8_t>(read_buffer[i]);
        } else {
            buf[i] = 0; 
        }

    }
    return true;
}

bool I2C_Mock::is_ready(int addr, uint32_t, uint32_t) {
    return true;
}

const std::vector<int>& I2C_Mock::getWriteBuffer() const {
    return write_buffer;
}
