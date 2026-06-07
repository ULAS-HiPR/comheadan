#ifndef DATA_H
#define DATA_H
#include <stdio.h>
#include <cstdint>

//12 bytes
struct accle_data
{
    float x{0};
    float y{0};
    float z{0};
};

//6 bytes
struct gyro_data
{
    int16_t x{0};
    int16_t y{0};
    int16_t z{0};
};

//12 bytes update
struct baro_data
{
    int32_t pressure{101325};
    float temperature{0};
    float altitude{0};
};

//18 bytes
struct imu_data
{
    accle_data acceleration;
    gyro_data gyro;
    int temperature; //check this 
};


//12 bytes
struct prediction_data
{
    float altitude{0};
    float velocity{0};
    float acceleration{0};
};

// 32 bytes
struct core_flight_data
{
    uint32_t time{0};
    baro_data barometer;
    imu_data imu;
};

//25 bytes
struct gps_data
{
    double latitude;
    double longitude;
    float altitude;
    float velocity;
    uint8_t satellites;
};


//39 bytes
struct secondary_flight_data
{
    uint16_t time;
    gps_data gps;
    accle_data acceleration;
   // imu_data imu;
};

// 52 bytes
struct flight_data
{
    uint32_t time{0};
    prediction_data prediction;
    core_flight_data core_data;
    int16_t state;
};

struct flash_internal_data {
    int main_height;
    int drouge_delay;
    int liftoff_thresh;
    int last_log;
};


// 13 btyes
struct canards_raw
{
    float kp{0.0f};
    float kd{0.0f};
    float servo_angle{0.0f};
    bool active{false};
};

enum State {
    CALIBRATING,
    READY,
    POWERED,
    COASTING,
    DROUGE,
    MAIN,
    LANDED,
    NUM_STATES
};

#endif // DATA_H

