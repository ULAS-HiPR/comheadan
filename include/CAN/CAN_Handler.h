#ifndef CAN_HANDLER_H
#define CAN_HANDLER_H

#include "CAN_Frames.h"

class CAN_Handler {
    public:
        virtual bool init() = 0;
        virtual bool send(CAN_Frame* frame) = 0;
        virtual bool receive(CAN_Frame* frame) = 0;
        virtual bool is_bus_off() const { return false; }
        virtual bool recover_from_bus_off() { return false; }
        virtual uint32_t error() const { return 0U; }
        virtual uint8_t tx_error_count() const { return 0U; }
        virtual uint8_t rx_error_count() const { return 0U; }
        virtual ~CAN_Handler() = default;
};

#endif
