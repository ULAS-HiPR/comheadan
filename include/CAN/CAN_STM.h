#ifdef STM
#ifdef CAN_COMPATIBLE
#ifndef CAN_STM_H
#define CAN_STM_H

#include "CAN_Handler.h"
#ifdef F4
#include "stm32f4xx_hal.h"
#elif F0
#include "stm32f0xx_hal.h"
#endif

class CAN_STM : public CAN_Handler {
    public:
        explicit CAN_STM(CAN_HandleTypeDef* hcan) : _hcan(hcan) {}
        bool init();
        bool send(CAN_Frame* frame) override;
        bool receive(CAN_Frame* frame) override;
        bool configure_accept_all_filter(uint32_t filter_bank = 0);
        bool configure_filter(uint32_t std_id, uint32_t std_id_mask,
                              uint32_t filter_bank = 0);
        bool is_started() const;
        bool is_bus_off() const override;
        bool recover_from_bus_off() override;
        uint32_t error() const override;
        uint8_t tx_error_count() const override;
        uint8_t rx_error_count() const override;

    private:
        CAN_HandleTypeDef* _hcan;
};
#endif

#endif
#endif
