#ifndef CAN_FRAME_H
#define CAN_FRAME_H

#include <cstdint>
#include <cstring>


// Priority 0 — CRITICAL
#define CAN_ID_IMU_ACCEL      0x000
#define CAN_ID_IMU_GYRO       0x010
#define CAN_ID_BARO           0x020
#define CAN_ID_FLIGHT_STATE   0x030
#define CAN_ID_KALMANN        0x040
#define CAN_ID_PYRO_ARM       0x050
#define CAN_ID_PYRO_FIRE      0x060



// Priority 2 — MEDIUM
#define CAN_ID_PYRO_STATUS    0x200
#define CAN_ID_PYRO_ACK       0x210
#define CAN_ID_CONFIG_CMD     0x220

// Priority 3 — NORMAL
#define CAN_ID_POWER_MAIN     0x300
#define CAN_ID_POWER_SERVO    0x310
#define CAN_ID_TX_STATUS      0x340
#define CAN_ID_SYNC           0x350
#define CAN_ID_GPS            0x360

// Priority 4 — LOW
#define CAN_ID_HEARTBEAT_BASE 0x420
#define CAN_ID_HEARTBEAT      CAN_ID_HEARTBEAT_BASE
#define CAN_ID_MUON_CPM       0x430
#define CAN_ID_CANARDS        0x440 //also used for airbrakes

#define CAN_ID_HEARTBEAT_NODE(node_id) \
    (CAN_ID_HEARTBEAT_BASE | ((node_id) & 0x0F))
#define CAN_ID_IS_HEARTBEAT(id) \
    (((id) & 0x7F0U) == CAN_ID_HEARTBEAT_BASE)


// Magic Word
#define PYRO_MAGIC            0xDEAD

// Heartbeats
#define NODE_CROI             0x01  // flight computer
#define NODE_PLEASC           0x02  // pyro board
#define NODE_LAMH             0x03  // servo board
#define NODE_TEACHTAIRE       0x04  // telemetry board
#define NODE_MUON             0x05  // muon detector
#define NODE_FOINSE           0x06  // power board

#define CAN_HEARTBEAT_ERR_BUS_OFF      0x01
#define CAN_HEARTBEAT_ERR_CAN_ERROR    0x02
#define CAN_HEARTBEAT_ERR_TX_DROP      0x04
#define CAN_HEARTBEAT_ERR_NODE_TIMEOUT 0x08

// Flight state enum (I have no idea if this is correct)
enum class FlightState : uint8_t {
    IDLE        = 0,
    ARMED       = 1,
    POWERED     = 2, 
    COASTING    = 3, 
    APOGEE      = 4,
    DESCENT     = 5,
    LANDED      = 6,
    FAULT       = 10
};


struct CAN_Frame {
    uint32_t id; 
    uint8_t  dlc;
    uint8_t  data[8];
};



// __attribute__((packed)) is so that compiler doesnt add padding
struct __attribute__((packed)) IMU_ACCEL_Payload {
    int16_t  ax;            
    int16_t  ay;            
    int16_t  az;           
    uint16_t timestamp_ms;
};


struct __attribute__((packed)) IMU_GYRO_Payload {
    int16_t  gx;
    int16_t  gy;
    int16_t  gz;
    uint16_t timestamp_ms;
};

// Bitmask flags to be determiined
struct __attribute__((packed)) BARO_Payload {
    uint32_t pressure;
    int16_t  temp;
    uint16_t timestamp_ms;
};


struct __attribute__((packed)) FLIGHT_STATE_Payload {
    uint8_t  state;          // FlightState enum
    uint8_t flags;           // Random flags
    uint16_t timestamp_ms;
};

struct __attribute__((packed)) KALMANN_Payload {
    int16_t accleration;
    int16_t  altitude_m;
    int16_t  vspeed_cms;
    uint16_t timestamp_ms;
    
};

struct __attribute__((packed)) PYRO_ARM_Payload {
    uint8_t  channel_mask;
    uint16_t magic;
    uint8_t  reserved;
};


struct __attribute__((packed)) PYRO_FIRE_Payload {
    uint8_t  channel;
    uint16_t magic;
    uint8_t  reserved;
    uint32_t timestamp_ms;
};

enum class PyroResult : uint8_t { FIRED = 0, FAULT = 1 };

struct __attribute__((packed)) PYRO_ACK_Payload {
    uint8_t channel;
    uint8_t result;
    uint8_t fault_code;
    uint8_t reserved;
};

// All of these are bitmasks
struct __attribute__((packed)) PYRO_STATUS_Payload {
    uint8_t armed;
    uint8_t cont_check;
    uint8_t faults1;
    uint8_t faults2;
};

// flags TBD
struct __attribute__((packed)) POWER_MAIN_Payload {
    uint16_t vbat_mv;
    uint16_t ibat_ma;
    uint8_t  soc_pct; 
    uint8_t  flags;
    uint16_t reserved;
};


struct __attribute__((packed)) POWER_SERVO_Payload {
    uint16_t vservo_mv;
    uint16_t iservo_ma;
    int8_t   temp_c;
    uint8_t  flags;
    uint16_t reserved;
};


struct __attribute__((packed)) SYNC_Payload {
    uint32_t timestamp_ms;  // bytes 0-3   (1 ms / LSB)
};


struct __attribute__((packed)) HEARTBEAT_Payload {
    uint8_t node_id;
    uint8_t state;
    uint8_t err;
    uint8_t uptime_s;
};


struct __attribute__((packed)) CONFIG_CMD_Payload {
    uint8_t  cmd_id;
    uint16_t param1;
    uint16_t param2;
    uint16_t param3;
    uint8_t  reserved;
};


// no idea if rssi and snr are the correct terms
struct __attribute__((packed)) TX_STATUS_Payload {
    int8_t  rssi;
    int8_t  snr;
    uint8_t tx_queue;
    uint8_t flags;
};


struct __attribute__((packed)) MUON_CPM_Payload {
    uint16_t cpm;
    uint32_t total_counts;
    uint16_t reserved;
};

struct __attribute__((packed)) GPS_Payload {
    int32_t latitude;     // store as int32 but only 24-bit range
    int32_t longitude;   
    uint8_t satellites;
    uint8_t flags;
};

struct __attribute__((packed)) CanardsPayload
{
    int16_t kp;           // scaled: kp * 1000
    int16_t kd;           // scaled: kd * 1000
    int16_t servo_angle;  // scaled: degrees * 100
    uint8_t active;
};

//F unctions to help pack the frames
template<typename T>
inline CAN_Frame pack_frame(uint32_t id, const T& payload) {
    static_assert(sizeof(T) <= 8, "CAN payload too big");
    CAN_Frame frame{};
    frame.id  = id;
    frame.dlc = 8;
    memset(frame.data, 0, sizeof(frame.data));
    memcpy(frame.data, &payload, sizeof(T));
    return frame;
}

template<typename T>
inline void unpack_frame(const CAN_Frame& frame, T& payload) {
    static_assert(sizeof(T) <= 8, "CAN payload too big");
    memset(&payload, 0, sizeof(T));
    const uint8_t bytes_to_copy = frame.dlc < sizeof(T)
                                ? frame.dlc
                                : static_cast<uint8_t>(sizeof(T));
    memcpy(&payload, frame.data, bytes_to_copy);
}

template<typename T>
inline bool try_unpack_frame(const CAN_Frame& frame, T& payload) {
    static_assert(sizeof(T) <= 8, "CAN payload too big");
    if (frame.dlc < sizeof(T) || frame.dlc > 8U) {
        memset(&payload, 0, sizeof(T));
        return false;
    }
    memcpy(&payload, frame.data, sizeof(T));
    return true;
}

//gps specifc cause 24 bit lat/lon and 8 bit sat and flags
inline CAN_Frame pack_gps(uint32_t id, int32_t lat, int32_t lon, uint8_t sat, uint8_t flags)
{
    CAN_Frame frame{};
    frame.id = id;
    frame.dlc = 8;

    frame.data[0] = (lat >> 16) & 0xFF;
    frame.data[1] = (lat >> 8) & 0xFF;
    frame.data[2] = (lat) & 0xFF;

    frame.data[3] = (lon >> 16) & 0xFF;
    frame.data[4] = (lon >> 8) & 0xFF;
    frame.data[5] = (lon) & 0xFF;

    frame.data[6] = sat;
    frame.data[7] = flags;

    return frame;
}

inline bool unpack_gps(const CAN_Frame& frame,
                        GPS_Payload& payload)
{
    payload.latitude = (int32_t)((frame.data[0] << 16) |
                    (frame.data[1] << 8) |
                    (frame.data[2]));

    // sign extend 24-bit
    if (payload.latitude & 0x800000) payload.latitude |= 0xFF000000;

    payload.longitude = (int32_t)((frame.data[3] << 16) |
                    (frame.data[4] << 8) |
                    (frame.data[5]));

    if (payload.longitude & 0x800000) payload.longitude |= 0xFF000000;

    payload.satellites   = frame.data[6];
    payload.flags = frame.data[7];

    return true;
}

inline int32_t gps_encode(double deg)
{
    return (int32_t)(deg * 1e6);   // 0.1m-ish resolution
}

inline double gps_decode(int32_t raw)
{
    return (double)raw / 1e6;
}

#endif