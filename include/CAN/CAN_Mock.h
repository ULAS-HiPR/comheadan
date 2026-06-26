#ifndef CAN_MOCK_H
#define CAN_MOCK_H

#include "CAN_Handler.h"
#include "CAN_Frames.h"
#include <cstdio>

class CAN_MOCK : public CAN_Handler {
public:
    CAN_MOCK() = default;

    bool init() override {
        printf("[CAN_MOCK] init()\n");
        return true;
    }

    bool send(CAN_Frame* frame) override {
        printf("[CAN_MOCK] SEND -> ID: 0x%03lX | DLC: %d | DATA:",
               frame->id,
               frame->dlc);

        for (int i = 0; i < frame->dlc; i++) {
            printf(" %02X", frame->data[i]);
        }

        printf("\n");
        return true;
    }

    bool receive(CAN_Frame* frame) override {
        printf("[CAN_MOCK] RECEIVE called (no data)\n");
        return false;
    }
};

#endif