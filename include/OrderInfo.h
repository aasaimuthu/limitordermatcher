#pragma once

#include <chrono>
#include "Common.h"

class OrderInfo
{
private:
    uint64     m_nReceivedTime;
    uint64     m_nOrderID;
    uint64     m_nQuantity;
    uint64     m_nPrice;
    enOrdSide  m_eSide;
    enOrdStatus m_eOrdStatus;
    uint64      m_nLevesQuantity;
    uint64      m_nRplOrdID;
public:
    OrderInfo(uint64 id, enOrdSide side, uint64 price, uint64 qty, enOrdStatus eOrdStatus, uint64 rplOrdID = 0)
                                                                                : m_nOrderID(id),
                                                                                  m_eSide(side),
                                                                                  m_nQuantity(qty),
                                                                                  m_nPrice(price),
                                                                                  m_eOrdStatus(eOrdStatus),
                                                                                  m_nLevesQuantity(qty),
                                                                                  m_nRplOrdID(rplOrdID)
    {
        Init();
    }
    OrderInfo() : m_nOrderID(0),
        m_eSide(enOrdSide::SIDE_UNKNOWN),
        m_nQuantity(0), m_nPrice(0),
        m_eOrdStatus(enOrdStatus::ORDER_STAT_UNKNOWN),
        m_nLevesQuantity(0),
        m_nRplOrdID(0)
    {
        Init();
    }

    void Init()
    {
        m_nReceivedTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    }

    uint64 GetOrderReceiveTime() const
    {
        return m_nReceivedTime;
    }

    uint64 GetOrderID() const
    {
        return m_nOrderID;
    }

    void SetOrderID(uint64 ord_id)
    {
        m_nOrderID = ord_id;
    }

    void SetRplOrderID(uint64 ord_id)
    {
        m_nOrderID = ord_id;
    }

    uint64 GetRplOrderID() const
    {
        return m_nRplOrdID;
    }

    enOrdSide GetSide() const
    {
        return m_eSide;
    }

    uint64 GetQuantity() const
    {
        return m_nQuantity;
    }

    void SetQuantity(uint64 qty)
    {
        if (qty < m_nQuantity) {
        m_nQuantity = qty;
        }
    }

    void SetLeavesQty(int64 LevesQuantity)
    {
        m_nLevesQuantity = LevesQuantity;
        if (m_nLevesQuantity == 0)
        {
            m_eOrdStatus = enOrdStatus::ORDER_STAT_FILLED;
        }
        else if(m_nLevesQuantity >0 )
        {
            m_eOrdStatus = enOrdStatus::ORDER_STAT_PART_FILLED;
        }
    }

    uint64 GetLeavesQuantity() const
    {
        return m_nLevesQuantity;
    }
    uint64 GetPrice() const
    {
        return m_nPrice;
    }
    void SetOrderStatus(enOrdStatus ordstat)
    {
        m_eOrdStatus = ordstat;
    }
    enOrdStatus GetOrderStatus() const
    {
        return m_eOrdStatus;
    }
    void GetOrderDetails(std::stringstream& ss)
    {         
        ss << "order_receivetime( " << m_nReceivedTime <<" )";
        ss << " order_id( " << m_nOrderID << " )";
        ss << " order_orgqty(" << m_nQuantity << " )";
        ss << " order_leavesqty(" << m_nLevesQuantity << " )";
        ss << " ord_status(" << GetOrderStatusStr(m_eOrdStatus) << " )";
        if(enOrdStatus::ORDER_STAT_REPLACED == m_eOrdStatus)
            ss << " ord_replaced for ord_id(" << m_nRplOrdID << " )";
    }

};
