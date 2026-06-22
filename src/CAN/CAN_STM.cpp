#ifdef CAN_COMPATIBLE
#include <CAN/CAN_STM.h>

bool CAN_STM::init() {

    CAN_FilterTypeDef filter = {0};

    filter.FilterActivation = ENABLE;
    filter.FilterBank = 0;
    filter.FilterMode = CAN_FILTERMODE_IDMASK;
    filter.FilterScale = CAN_FILTERSCALE_32BIT;
    
    filter.FilterIdHigh = 0x0000;
    filter.FilterIdLow = 0x0000;
    filter.FilterMaskIdHigh = 0x0000;
    filter.FilterMaskIdLow = 0x0000;
    
    filter.FilterFIFOAssignment = CAN_RX_FIFO0;
    
    HAL_CAN_ConfigFilter(_hcan, &filter);

    return HAL_CAN_Start(_hcan) == HAL_OK;
}

bool CAN_STM::send(CAN_Frame* frame) {
    CAN_TxHeaderTypeDef tx_header;
    tx_header.StdId = frame->id; 
    tx_header.IDE   = CAN_ID_STD;
    tx_header.RTR   = CAN_RTR_DATA;
    tx_header.DLC   = frame->dlc;

    uint32_t tx_mailbox;
    HAL_StatusTypeDef ret = HAL_CAN_AddTxMessage(
        _hcan,
        &tx_header,
        frame->data,
        &tx_mailbox
    );

    uint32_t tsr = _hcan->Instance->TSR;
    uint32_t esr = _hcan->Instance->ESR;    
    if (ret != HAL_OK)
    {
        uint32_t err = HAL_CAN_GetError(_hcan);
        return false;
    }

    return true;

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