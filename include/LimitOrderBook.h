#pragma once
#include"Common.h"
#include"OrderInfo.h"
#include"OrderBookEntry.h"

class LimitOrderBook
{
public:
    LimitOrderBook(double tick = .5, uint8 tickmultiplier = 10, uint8 pricemultiplier = 100):
                                    m_dblTickSize(tick), m_nTickMultiplier(tickmultiplier), m_nPriceMultiplier(pricemultiplier)
    {}
    void Add(const std::shared_ptr<OrderInfo>& order);
    void Update(std::shared_ptr<OrderInfo>& order);
    void Cancel(std::shared_ptr<OrderInfo> pOrderInfo);

    void Add(uint64 ord_id, enOrdSide side, uint64 price, uint64 quantity);
    void Update(uint64 prev_ord_id, uint64 prevquantity, uint64 new_ord_id, uint64 newquantity);
    void Cancel(uint64 ord_id);

    void PrintOrderBook();
    QueryOrderLevelResult QueryOrderBookLevel(int level, enOrdSide side);
    QueryOrderLevelResult QueryOrderStatus(uint64 ord_id);

private:
    bool CheckTickSize(const std::shared_ptr<OrderInfo>& pOrderInfo);
    std::shared_ptr<OrderInfo> GetOrderInfo(uint64 ord_id);
    void RemoveOrderInfo(const std::shared_ptr<OrderInfo>& order);
    void OrderMatching();

private:

    template<class T>
    void AddOrder(T& orderBook,const std::shared_ptr<OrderInfo>& pOrderInfo)
    {
        auto it = orderBook.find(pOrderInfo->GetPrice());
        if (it == orderBook.end())
        {
            std::shared_ptr <OrderBookEntry> pBookEntry(new OrderBookEntry());
            pBookEntry->AddQuantity(pOrderInfo->GetQuantity());
            pBookEntry->AddOrderQueue(pOrderInfo);
            orderBook.emplace(pOrderInfo->GetPrice(), pBookEntry);
        }
        else
        {
            it->second->AddQuantity(pOrderInfo->GetQuantity());
            it->second->AddOrderQueue(pOrderInfo);
        }
    }

    template<class T>
    void UpdateAmendDownOrder(T& orderBook, const std::shared_ptr<OrderInfo>& pOldOrderInfo, const std::shared_ptr<OrderInfo>& pRplOrderInfo)
    {
        auto it = orderBook.find(pOldOrderInfo->GetPrice());
        if (it != orderBook.end())
        {
            std::shared_ptr <OrderBookEntry> pOrdreBookEntries = it->second;
            if (!pOrdreBookEntries->IsEmptyQueue())
            {
                uint64_t prevqty = pOldOrderInfo->GetQuantity();
                auto queueit = pOrdreBookEntries->find(pOldOrderInfo->GetOrderReceiveTime());
                if (queueit != pOrdreBookEntries->end())
                {
                    pOldOrderInfo->SetOrderStatus(enOrdStatus::ORDER_STAT_CANCELLED);
                    pRplOrderInfo->SetOrderStatus(enOrdStatus::ORDER_STAT_REPLACED);
                    queueit->second = pRplOrderInfo;
                    pOrdreBookEntries->UpdateQuantity(prevqty, pRplOrderInfo->GetQuantity());
                }
            }
        }
    }

    template<class T>
    void UpdateAmendUpOrder(T& orderBook, const std::shared_ptr<OrderInfo>& pOldOrderInfo, const std::shared_ptr<OrderInfo>& pRplOrderInfo)
    {
        auto it = orderBook.find(pOldOrderInfo->GetPrice());
        if (it != orderBook.end())
        {
            std::shared_ptr <OrderBookEntry> pOrdreBookEntries = it->second;
            if (!pOrdreBookEntries->IsEmptyQueue())
            {
                auto queueit = pOrdreBookEntries->find(pOldOrderInfo->GetOrderReceiveTime());
                if (queueit != pOrdreBookEntries->end())
                {
                    uint64_t prevqty = pOldOrderInfo->GetQuantity();
                    pOrdreBookEntries->erase(queueit);
                    pOrdreBookEntries->AddOrderQueue(pRplOrderInfo);
                    pOldOrderInfo->SetOrderStatus(enOrdStatus::ORDER_STAT_CANCELLED);
                    pRplOrderInfo->SetOrderStatus(enOrdStatus::ORDER_STAT_REPLACED);
                    pOrdreBookEntries->UpdateQuantity(prevqty, pRplOrderInfo->GetQuantity());
                }
            }
        }
    }

    template<class T>
    void CancelBookOrder(T& orderBook, const std::shared_ptr<OrderInfo>& pOrgOrderInfo,enOrdStatus ordstatus)
    {
        auto it = orderBook.find(pOrgOrderInfo->GetPrice());
        if (it != orderBook.end())
        {
            it->second->CanceBookOrder(pOrgOrderInfo, ordstatus);
            if (it->second->IsEmptyQueue()) //no more order in the queue remove the price entry from bid side
            {
            	orderBook.erase(pOrgOrderInfo->GetPrice());
            }
        }
        pOrgOrderInfo->SetOrderStatus(enOrdStatus::ORDER_STAT_CANCELLED);
    }

    template<class T>
    void GetBook(T& orderBook, enOrdSide side)
    {
        std::stringstream ss;
        if (orderBook.empty())
            return;
        auto PriceIter = orderBook.begin();

        ss << (side== enOrdSide::SIDE_BID ? " BID SIDE" :  " ASK SIDE")  << std::endl;
        ss << " Level   " << " Price   " << "   totalSize" << std::endl;
        int level = 0;
        for (; PriceIter != orderBook.end(); ++PriceIter)
        {
            int queue = 0;
            std::shared_ptr <OrderBookEntry> pOrdreBookEntries = PriceIter->second;
            if (!pOrdreBookEntries->IsEmptyQueue())
            {
                auto OrderQueue = pOrdreBookEntries->begin();
                ss << " Level   " << level++ << "  " << PriceIter->first / double(m_nPriceMultiplier) << "    " << pOrdreBookEntries->GetCurrentTotalOrderQuantity() << std::endl;
                for (; OrderQueue != pOrdreBookEntries->end(); ++OrderQueue)
                {
                	std::stringstream squeue;
                    OrderQueue->second->GetOrderDetails(squeue);
                    ss << "queue:" << queue++ << "    " << squeue.str() << std::endl;
                }
            }
        }
        std::cout << " CURRENT BOOK:" << ss.str();
    }

    template<class T>
    QueryOrderLevelResult QueryOrderBookLevel(T& orderBook,int querylevel)
    {
        QueryOrderLevelResult queryOrderLevelResult;
        if (orderBook.empty())
            return queryOrderLevelResult;
        auto PriceIter = orderBook.begin();

        int level = 0;
        for (; PriceIter != orderBook.end(); ++PriceIter)
        {
            if (querylevel == level++)
            {
                int queue = 0;
                queryOrderLevelResult.m_dblPrice = PriceIter->first/(double)m_nPriceMultiplier;
                std::shared_ptr <OrderBookEntry> pBidOrdreBookEntries = PriceIter->second;
                if (!pBidOrdreBookEntries->IsEmptyQueue())
                {
                    queryOrderLevelResult.m_NumItem = static_cast<uint32>(pBidOrdreBookEntries->size());
                    queryOrderLevelResult.m_nQuantity = pBidOrdreBookEntries->GetCurrentTotalOrderQuantity();
                }
            }
        }
        return queryOrderLevelResult;
    }

    template<class T>
    QueryOrderLevelResult QueryOrderStatus(T& orderBook,const std::shared_ptr<OrderInfo> pOrderInfo)
    {
        QueryOrderLevelResult orderstatus;
        orderstatus.m_enOrdStatus = pOrderInfo->GetOrderStatus();
        if (orderBook.empty())
            return orderstatus;
        auto PriceIter = orderBook.begin();
        int level = 0;
        for (; PriceIter != orderBook.end(); ++PriceIter)
        {
            int queuepos = 0;
            bool OrderFound = false;
            std::shared_ptr <OrderBookEntry> pBidOrdreBookEntries = PriceIter->second;
            if (!pBidOrdreBookEntries->IsEmptyQueue())
            {
                auto OrderQueue = pBidOrdreBookEntries->begin();
                for (; OrderQueue != pBidOrdreBookEntries->end(); ++OrderQueue)
                {
                    if(OrderQueue->second->GetOrderID() == pOrderInfo->GetOrderID())
                    {
                        orderstatus.m_QueuePosition = queuepos;
                        orderstatus.m_dblPrice = PriceIter->first;
                        orderstatus.m_eOrdSide = OrderQueue->second->GetSide();
                        OrderFound = true;
                        break;
                    }
                    queuepos++;
                }
            }
            if (OrderFound)
                break;
        }
        return orderstatus;
    }

private:
    mapBidOrderBook m_mapBidOrderBook;
    mapAskOrderBook m_mapAskOrderBook;
    OrderStore      m_AllOrderInfo;
    double          m_dblTickSize;
    uint8           m_nTickMultiplier;
    uint8           m_nPriceMultiplier;
};

