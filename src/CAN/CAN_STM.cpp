#ifdef CAN_COMPATIBLE
#include <CAN/CAN_STM.h>

bool CAN_STM::init() {
    if (_hcan == nullptr || _hcan->Instance == nullptr) {
        return false;
    }

    if (is_started()) {
        return true;
    }

    if (!configure_accept_all_filter()) {
        return false;
    }

    HAL_CAN_ResetError(_hcan);
    return HAL_CAN_Start(_hcan) == HAL_OK;
}

bool CAN_STM::send(CAN_Frame* frame) {
    if (_hcan == nullptr || _hcan->Instance == nullptr || frame == nullptr ||
        !is_started() || is_bus_off()) {
        return false;
    }
    if (frame->id > 0x7FFU || frame->dlc > 8U) {
        return false;
    }
    if (HAL_CAN_GetTxMailboxesFreeLevel(_hcan) == 0U) {
        return false;
    }

    CAN_TxHeaderTypeDef tx_header{};
    tx_header.StdId = frame->id;
    tx_header.IDE = CAN_ID_STD;
    tx_header.RTR = CAN_RTR_DATA;
    tx_header.DLC = frame->dlc;
    tx_header.TransmitGlobalTime = DISABLE;

    uint32_t tx_mailbox = 0U;
    return HAL_CAN_AddTxMessage(
        _hcan,
        &tx_header,
        frame->data,
        &tx_mailbox
    ) == HAL_OK;
}

bool CAN_STM::receive(CAN_Frame* frame) {
    if (_hcan == nullptr || _hcan->Instance == nullptr || frame == nullptr ||
        !is_started()) {
        return false;
    }

    if (__HAL_CAN_GET_FLAG(_hcan, CAN_FLAG_FOV0) != RESET) {
        __HAL_CAN_CLEAR_FLAG(_hcan, CAN_FLAG_FOV0);
    }

    if (HAL_CAN_GetRxFifoFillLevel(_hcan, CAN_RX_FIFO0) == 0) {
        return false;
    }

    CAN_RxHeaderTypeDef rx_header{};
    for (uint8_t& byte : frame->data) {
        byte = 0U;
    }

    HAL_StatusTypeDef status = HAL_CAN_GetRxMessage(
        _hcan,
        CAN_RX_FIFO0,
        &rx_header,
        frame->data
    );

    if (status != HAL_OK) return false;

    if (rx_header.IDE != CAN_ID_STD || rx_header.RTR != CAN_RTR_DATA ||
        rx_header.DLC > 8U) {
        return false;
    }

    frame->id  = rx_header.StdId;
    frame->dlc = rx_header.DLC;
    return true;
}

bool CAN_STM::configure_accept_all_filter(uint32_t filter_bank) {
    return configure_filter(0U, 0U, filter_bank);
}

bool CAN_STM::configure_filter(uint32_t std_id, uint32_t std_id_mask,
                               uint32_t filter_bank) {
    if (_hcan == nullptr || _hcan->Instance == nullptr ||
        std_id > 0x7FFU || std_id_mask > 0x7FFU) {
        return false;
    }

    CAN_FilterTypeDef filter{};
    filter.FilterIdHigh = static_cast<uint16_t>(std_id << 5U);
    filter.FilterIdLow = 0;
    filter.FilterMaskIdHigh = static_cast<uint16_t>(std_id_mask << 5U);
    filter.FilterMaskIdLow = 0;
    filter.FilterFIFOAssignment = CAN_RX_FIFO0;
    filter.FilterBank = filter_bank;
    filter.FilterMode = CAN_FILTERMODE_IDMASK;
    filter.FilterScale = CAN_FILTERSCALE_32BIT;
    filter.FilterActivation = ENABLE;
    filter.SlaveStartFilterBank = 14;

    return HAL_CAN_ConfigFilter(_hcan, &filter) == HAL_OK;
}

bool CAN_STM::is_started() const {
    return _hcan != nullptr && _hcan->Instance != nullptr &&
           HAL_CAN_GetState(_hcan) == HAL_CAN_STATE_LISTENING;
}

bool CAN_STM::is_bus_off() const {
    return _hcan != nullptr && _hcan->Instance != nullptr &&
           (_hcan->Instance->ESR & CAN_ESR_BOFF) != 0U;
}

bool CAN_STM::recover_from_bus_off() {
    if (_hcan == nullptr || _hcan->Instance == nullptr) {
        return false;
    }

    if (!is_bus_off()) {
        return is_started();
    }

    (void)HAL_CAN_Stop(_hcan);
    HAL_CAN_ResetError(_hcan);
    return HAL_CAN_Start(_hcan) == HAL_OK;
}

uint32_t CAN_STM::error() const {
    if (_hcan == nullptr || _hcan->Instance == nullptr) {
        return HAL_CAN_ERROR_PARAM;
    }
    return HAL_CAN_GetError(_hcan);
}

uint8_t CAN_STM::tx_error_count() const {
    if (_hcan == nullptr || _hcan->Instance == nullptr) {
        return 0U;
    }
    return static_cast<uint8_t>((_hcan->Instance->ESR & CAN_ESR_TEC_Msk) >>
                                CAN_ESR_TEC_Pos);
}

uint8_t CAN_STM::rx_error_count() const {
    if (_hcan == nullptr || _hcan->Instance == nullptr) {
        return 0U;
    }
    return static_cast<uint8_t>((_hcan->Instance->ESR & CAN_ESR_REC_Msk) >>
                                CAN_ESR_REC_Pos);
}
#endif