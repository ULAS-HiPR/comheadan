#ifndef I2C_MOCK_H
#define I2C_MOCK_H
#include <cstddef>
#include <vector>
#include <I2C/I2C_Handler.h>

class I2C_Mock : public I2C_Handler {
    public:
        I2C_Mock(std::vector<int> read_data) : read_buffer(read_data) {};
        bool write(int addr, uint8_t* data, size_t len) override ;
        bool read(int addr, uint8_t data, uint8_t* buf, size_t len) override;
        bool is_ready(int addr, uint32_t trials = 2, uint32_t timeout_ms = 10) override;
        const std::vector<int>& getWriteBuffer() const ;
    private:
        std::vector<int> write_buffer;
        std::vector<int> read_buffer;     
};

#endif // I2C_MOCK_H
