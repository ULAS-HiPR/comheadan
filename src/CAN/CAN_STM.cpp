#ifdef CAN_COMPATIBLE
#include <CAN/CAN_STM.h>

bool CAN_STM::init() {
    return HAL_CAN_Start(_hcan) == HAL_OK;
}

bool CAN_STM::send(CAN_Frame* frame) {
    CAN_TxHeaderTypeDef tx_header;
    tx_header.StdId = frame->id; 
    tx_header.IDE   = CAN_ID_STD;
    tx_header.RTR   = CAN_RTR_DATA;
    tx_header.DLC   = frame->dlc;

    uint32_t tx_mailbox;
    return HAL_CAN_AddTxMessage(
        _hcan,
        &tx_header,
        frame->data,
        &tx_mailbox
    ) == HAL_OK;
}

bool CAN_STM::receive(CAN_Frame* frame) {
    if (HAL_CAN_GetRxFifoFillLevel(_hcan, CAN_RX_FIFO0) == 0) {
        return false;
    }

    CAN_RxHeaderTypeDef rx_header;

    // Check if the frame is actually valid
    HAL_StatusTypeDef status = HAL_CAN_GetRxMessage(
        _hcan,
        CAN_RX_FIFO0,
        &rx_header,
        frame->data
    );

    if (status != HAL_OK) return false;

    frame->id  = rx_header.StdId;
    frame->dlc = rx_header.DLC;
    return true;
}
#endif