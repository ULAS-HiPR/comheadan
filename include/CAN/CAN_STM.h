#ifdef F4
#ifndef CAN_STM_H
#define CAN_STM_H

#include "CAN_Handler.h"
#include "stm32f4xx_hal.h"

class CAN_STM : public CAN_Handler {
    public:
        explicit CAN_STM(CAN_HandleTypeDef* hcan) : _hcan(hcan) {}
        bool init();
        bool send(CAN_Frame* frame) override;
        bool receive(CAN_Frame* frame) override;

    private:
        CAN_HandleTypeDef* _hcan;
};

#endif
#endif