#ifndef DATA_H
#define DATA_H
#include <stdio.h>
#include <cstdint>
#include <cstring>

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
    accle_data acceleration{};
    gyro_data gyro{};
    int temperature{0}; //check this 
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
    baro_data barometer{};
    imu_data imu{};
};

//25 bytes
struct gps_data
{
    double latitude{0.0};
    double longitude{0.0};
    float altitude{0.0f};
    float velocity{0.0f};
    uint8_t satellites{0U};
};

struct __attribute__((packed)) gps_packet_t
{
    int32_t latitude;
    int32_t longitude;
    int16_t altitude;
    int16_t velocity;
    uint8_t satellites;
    uint8_t gps_valid;
};

static inline std::size_t pack_gps(const gps_data& in, uint8_t* out_buffer, bool gps_valid)
{
    gps_packet_t pkt;

    if (gps_valid) {
        pkt.latitude  = static_cast<int32_t>(in.latitude * 1e7);
        pkt.longitude = static_cast<int32_t>(in.longitude * 1e7);
        pkt.altitude  = static_cast<int16_t>(in.altitude * 10.0f);
        pkt.velocity  = static_cast<int16_t>(in.velocity * 10.0f);
        pkt.satellites = in.satellites;
    } else {
        // safe fallback values
        pkt.latitude   = 0;
        pkt.longitude  = 0;
        pkt.altitude   = 0;
        pkt.velocity   = 0;
        pkt.satellites = 0;
    }

    pkt.gps_valid = gps_valid ? 1 : 0;

    std::memcpy(out_buffer, &pkt, sizeof(pkt));
    return sizeof(pkt);
}

/// 13 btyes
struct canards_raw
{
    float kp{0.0f};
    float kd{0.0f};
    float servo_angle{0.0f};
    bool active{false};
};

struct secondary_flight_data
{
    gps_data gps{};
    canards_raw canards{};
};

// 52 bytes
struct flight_data
{
    uint32_t time{0};
    prediction_data prediction{};
    core_flight_data core_data{};
    int16_t state{0};
};

struct flash_internal_data {
    int main_height{200};
    int drouge_delay{0};
    int liftoff_thresh{20};
    int last_log{0};
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

