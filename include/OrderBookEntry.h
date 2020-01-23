#pragma once

#include"Common.h"
#include"OrderInfo.h"

class OrderBookEntry;

typedef std::map<uint64_t, std::shared_ptr<OrderInfo> > mapOrderListQueue;
typedef std::map<uint64_t, std::shared_ptr <OrderBookEntry>, std::greater<uint64_t>> mapBidOrderBook;
typedef std::map<uint64_t, std::shared_ptr <OrderBookEntry>> mapAskOrderBook;
typedef std::unordered_map<uint64_t, std::shared_ptr<OrderInfo>> OrderStore;

class OrderBookEntry
{
public:
    OrderBookEntry() :m_TotalBookSize(0) {}
    void AddOrderQueue(std::shared_ptr<OrderInfo> pOrder)
    {
        m_OrderListQueue.emplace(pOrder->GetOrderReceiveTime(), pOrder);
    }

    void AddQuantity(uint64_t qty)
    {
        m_TotalBookSize += qty;
    }

    void UpdateQuantity(int64_t preqty, int64_t newqty)
    {
        m_TotalBookSize += newqty- preqty;
    }

    void ReduceOrderFilledQuantity(int64_t qty)
    {
        m_TotalBookSize -=qty;
    }

    uint64_t GetCurrentTotalOrderQuantity()const
    {
        return m_TotalBookSize;;
    }

    void CanceBookOrder(std::shared_ptr<OrderInfo> pOrder, enOrdStatus ordStatus)
    {
        if (IsEmptyQueue())
            return;
        m_TotalBookSize -= pOrder->GetQuantity();
        pOrder->SetOrderStatus(ordStatus);
        auto it = m_OrderListQueue.find(pOrder->GetOrderReceiveTime());
        if( it != m_OrderListQueue.end())
        {
            m_OrderListQueue.erase(it);
         }
    }

    bool IsEmptyQueue()
    {
        return m_OrderListQueue.empty();
    }
    mapOrderListQueue::iterator begin() { return m_OrderListQueue.begin(); }
    mapOrderListQueue::const_iterator begin() const { return m_OrderListQueue.begin(); }
    mapOrderListQueue::iterator end() { return m_OrderListQueue.end(); }
    mapOrderListQueue::const_iterator end() const { return m_OrderListQueue.end(); }
    mapOrderListQueue::iterator find(const uint64_t entrykey) { return m_OrderListQueue.find(entrykey); }
    void erase(mapOrderListQueue::iterator it) { m_OrderListQueue.erase(it); }
    size_t size() const { return m_OrderListQueue.size(); }

private:
    uint64_t m_TotalBookSize;
    mapOrderListQueue m_OrderListQueue;
};

