#include <unity.h>
#include <I2C_Mock.h>
#include <I2C/I2C_Handler.h>
#include <vector>


void test_read(void) {
    std::vector<int> read_data = {10, 20, 30, 40};
    I2C_Handler* i2c = new I2C_Mock(read_data);
    
    uint8_t buf[4];
    i2c->read(0x50, 0x00, buf, 4);
    TEST_ASSERT_EQUAL_UINT8(10, buf[0]);
    TEST_ASSERT_EQUAL_UINT8(20, buf[1]);
    TEST_ASSERT_EQUAL_UINT8(30, buf[2]);
    TEST_ASSERT_EQUAL_UINT8(40, buf[3]);
    delete i2c;
}

void test_write(void) {
    std::vector<int> read_data; 
    I2C_Handler* i2c = new I2C_Mock(read_data);
    
    uint8_t data_to_write[3] = {1, 2, 3};
    i2c->write(0x50, data_to_write, 3);
    
    I2C_Mock* mock = dynamic_cast<I2C_Mock*>(i2c);
    TEST_ASSERT_NOT_NULL(mock);
    const std::vector<int>& write_buffer = mock->getWriteBuffer();
    TEST_ASSERT_EQUAL_INT(3, write_buffer.size());
    TEST_ASSERT_EQUAL_INT(1, write_buffer[0]);
    TEST_ASSERT_EQUAL_INT(2, write_buffer[1]);
    TEST_ASSERT_EQUAL_INT(3, write_buffer[2]);
    
    delete i2c;
}

int main( int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_write);
    RUN_TEST(test_read);
    UNITY_END();
}