#pragma once
#ifdef LINUX

#include "CAN/CAN_Handler.h"

/*
 * CAN_Linux — SocketCAN implementation of CAN_Handler.
 *
 * Opens a raw AF_CAN socket on the named interface (default: "vcan0").
 * Receive is non-blocking: returns false immediately if no frame is waiting.
 * All boards bind to the same interface and see all frames, identical to
 * real CAN bus behaviour where every node receives every message.
 */
class CAN_Linux : public CAN_Handler {
public:
    explicit CAN_Linux(const char * ifname = "vcan0");
    ~CAN_Linux();

    /* Open the socket and bind to the interface. Returns true on success. */
    bool init();

    /* Pack frame into struct can_frame and write to socket. */
    bool send(CAN_Frame * frame) override;

    /* Non-blocking read. Returns false if no frame available. */
    bool receive(CAN_Frame * frame) override;

private:
    const char * _ifname;
    int          _sock;
};

#endif /* LINUX */
