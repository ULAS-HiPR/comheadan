#ifndef CAN_HANDLER_H
#define CAN_HANDLER_H

#include "CAN_Frames.h"

class CAN_Handler {
    public:
        virtual bool send(CAN_Frame* frame) = 0;
        virtual bool receive(CAN_Frame* frame) = 0;
        virtual ~CAN_Handler() = default;
};

#endif