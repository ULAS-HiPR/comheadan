#ifndef CAN_FRAME_H
#define CAN_FRAME_H

#include <cstdint>
#include <cstring>


// Priority 0 — CRITICAL
#define CAN_ID_IMU_ACCEL      0x000
#define CAN_ID_IMU_GYRO       0x010
#define CAN_ID_BARO           0x020
#define CAN_ID_FLIGHT_STATE   0x030
#define CAN_ID_PYRO_ARM       0x040
#define CAN_ID_PYRO_FIRE      0x050
#define CAN_ID_KALMANN        0x060


// Priority 2 — MEDIUM
#define CAN_ID_PYRO_STATUS    0x200
#define CAN_ID_PYRO_ACK       0x210
#define CAN_ID_CONFIG_CMD     0x220

// Priority 3 — NORMAL
#define CAN_ID_POWER_MAIN     0x300
#define CAN_ID_POWER_SERVO    0x310
#define CAN_ID_TX_STATUS      0x340
#define CAN_ID_SYNC           0x350

// Priority 4 — LOW
#define CAN_ID_HEARTBEAT      0x420
#define CAN_ID_MUON_CPM       0x430


// Magic Word
#define PYRO_MAGIC            0xDEAD

// Heartbeats
#define NODE_CROI             0x01  // flight computer
#define NODE_PLEASC           0x02  // pyro board
#define NODE_LAMH             0x03  // servo board
#define NODE_TEACHTAIRE       0x04  // telemetry board
#define NODE_MUON             0x05  // muon detector

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


//F unctions to help pack the frames
template<typename T>
inline CAN_Frame pack_frame(uint32_t id, const T& payload) {
    static_assert(sizeof(T) <= 8, "Payload too big"); // compile time error checking
    CAN_Frame frame{};
    frame.id  = id;
    frame.dlc = 8;  // force full frame

    memset(frame.data, 0, 8);
    memcpy(frame.data, &payload, sizeof(T));

    return frame;
}

template<typename T>
inline void unpack_frame(const CAN_Frame& frame, T& payload) {
    memcpy(&payload, frame.data, sizeof(T));
}

#endif