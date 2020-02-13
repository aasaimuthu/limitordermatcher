#include "LimitOrderBook.h"
using namespace std;

shared_ptr<OrderInfo> LimitOrderBook::GetOrderInfo(uint64 ord_id)
{
    const auto it = m_AllOrderInfo.find(ord_id);
    if (it != m_AllOrderInfo.end())
    {
        return it->second;
    }
    static std::shared_ptr<OrderInfo> empty;
    return empty;
}

void LimitOrderBook::RemoveOrderInfo(const std::shared_ptr<OrderInfo>& pOrderInfo)
{
    m_AllOrderInfo.erase(pOrderInfo->GetOrderID());
}

bool  LimitOrderBook::CheckTickSize(const std::shared_ptr<OrderInfo>& pOrderInfo)
{
    static uint8 ntickmov = static_cast<int>(m_dblTickSize * m_nTickMultiplier);
    if (pOrderInfo->GetPrice() % ntickmov == 0)
        return true;
    cout << " invalide tick price movement orderprice(" << pOrderInfo->GetPrice() << ") allowed tick move(" << m_dblTickSize <<")" << endl;
    return false;
}

void LimitOrderBook::Add(uint64 ord_id,enOrdSide side, uint64 price, uint64 quantity)
{
    if (price == 0 || quantity == 0)
    {
        cout << "invalide ord qty or price price( "<< price << " ) quantity(" << quantity <<" )" <<  endl;
        return;
    }
    if (side != enOrdSide::SIDE_BID && side != enOrdSide::SIDE_ASK)
    {
        cout << "invalide ord side" << endl;
        return;
    }
    shared_ptr<OrderInfo> pOrder(new OrderInfo(ord_id, side, price, quantity, enOrdStatus::ORDER_STAT_NEW));
    if(pOrder)
    {
        auto pair = m_AllOrderInfo.emplace(pOrder->GetOrderID(), pOrder);
        if (pair.second)
            Add(pOrder);
        else
        	cout << "failed to emplace the object into map." << endl;
    }
    else
    {
        cout << "failed to create the orderinfo object." << endl;
    }
}

void LimitOrderBook::Add(const std::shared_ptr<OrderInfo>& pOrderInfo)
{
    if(pOrderInfo == nullptr)
    {
        cout << "new order is NULL "<< endl;
        return;
    }
    if (GetOrderInfo(pOrderInfo->GetOrderID()) != nullptr)
    {
        pOrderInfo->SetOrderStatus(enOrdStatus::ORDER_STAT_REJECTED);
        cout << "Duplicate ordre id, rejecting ord_id( " << pOrderInfo->GetOrderID() << " )"<< endl;
        return;
    }
    if (!CheckTickSize(pOrderInfo))
    {
        pOrderInfo->SetOrderStatus(enOrdStatus::ORDER_STAT_REJECTED);
        cout << "Duplicate OrderInfo, rejecting ord_id( " << pOrderInfo->GetOrderID() << " )" << endl;
        return;
    }

    pOrderInfo->GetSide() == enOrdSide::SIDE_BID ? AddOrder(m_mapBidOrderBook, pOrderInfo) : AddOrder(m_mapAskOrderBook, pOrderInfo);

    OrderMatching();
}

void LimitOrderBook::Update(uint64 prev_ord_id, uint64 prevquantity, uint64 new_ord_id, uint64 newquantity)
{
    const std::shared_ptr<OrderInfo> pOldOrderInfo = GetOrderInfo(prev_ord_id);
    if (pOldOrderInfo == nullptr)
    {
        cout << "Existing replace order not found, order_id(" << prev_ord_id << ") new_ord_id( " << new_ord_id << " )"<< endl;
        return;
    }
    std::shared_ptr<OrderInfo> pRplOrderInfo(new OrderInfo(new_ord_id, pOldOrderInfo->GetSide(), pOldOrderInfo->GetPrice(), newquantity, enOrdStatus::ORDER_STAT_PENDING_REPLACE, prev_ord_id));
    Update(pRplOrderInfo);
}

void LimitOrderBook::Update(std::shared_ptr<OrderInfo>& pRplOrderInfo)
{
    if(pRplOrderInfo == nullptr)
    {
        cout << "replace order is NULL "<< endl;
        return;
    }
    if (pRplOrderInfo->GetPrice() == 0 || pRplOrderInfo->GetQuantity() == 0)
    {
        //cancel the existing OrderInfo
        pRplOrderInfo->SetOrderStatus(enOrdStatus::ORDER_STAT_REJECTED);
        cout << "Price or order quantity are zero., order_id(" << pRplOrderInfo->GetOrderID() << ") price( " << pRplOrderInfo->GetPrice() << ") qty( " <<  pRplOrderInfo->GetQuantity() << " )" << endl;
        return;
    }

    const std::shared_ptr<OrderInfo> pOldOrderInfo = GetOrderInfo(pRplOrderInfo->GetRplOrderID());
    if (pOldOrderInfo == nullptr)
    {
        pRplOrderInfo->SetOrderStatus(enOrdStatus::ORDER_STAT_REJECTED);
        cout << "Existing replace order not found, order_id(" << pRplOrderInfo->GetOrderID() << ")" << endl;
        return;
    }
    if (pOldOrderInfo->GetOrderStatus() == enOrdStatus::ORDER_STAT_REPLACED)
    {
        pRplOrderInfo->SetOrderStatus(enOrdStatus::ORDER_STAT_REJECTED);
        cout << "existing order already replaced , order_id(" << pRplOrderInfo->GetOrderID() << ")" << endl;
        return;
    }
    auto pair = m_AllOrderInfo.emplace(pRplOrderInfo->GetOrderID(), pRplOrderInfo);
    if (!pair.second)
    {
        pRplOrderInfo->SetOrderStatus(enOrdStatus::ORDER_STAT_REJECTED);
        cout << "failed to insert the replace order, order_id(" << pRplOrderInfo->GetOrderID() << ")" << endl;
        return;
    }
    if (pOldOrderInfo->GetQuantity() != pRplOrderInfo->GetQuantity())
    {
        if (pOldOrderInfo->GetQuantity() < pRplOrderInfo->GetQuantity())
        {
            pRplOrderInfo->GetSide() == enOrdSide::SIDE_BID ? UpdateAmendUpOrder(m_mapBidOrderBook, pOldOrderInfo, pRplOrderInfo) : UpdateAmendUpOrder(m_mapAskOrderBook, pOldOrderInfo, pRplOrderInfo);
        }
        else
        {
            pRplOrderInfo->GetSide() == enOrdSide::SIDE_BID ? UpdateAmendDownOrder(m_mapBidOrderBook, pOldOrderInfo,pRplOrderInfo) : UpdateAmendDownOrder(m_mapAskOrderBook, pOldOrderInfo, pRplOrderInfo);
        }
    }
    else
    {
        pRplOrderInfo->SetOrderStatus(enOrdStatus::ORDER_STAT_REJECTED);
        cout << "Existing order and replace order quantities are same(" << pRplOrderInfo->GetOrderID() << ")" << endl;
    }
    OrderMatching();
}

void LimitOrderBook::Cancel(uint64 ord_id)
{
    const std::shared_ptr<OrderInfo> pOrderInfo = GetOrderInfo(ord_id);
    if (pOrderInfo == nullptr)
    {
        cout << "OrderInfo not found, could be already cancelled, orderid(" << ord_id << " )" << endl;
        return;
    }
    enOrdStatus ordstatus = pOrderInfo->GetOrderStatus();
    pOrderInfo->GetSide() == enOrdSide::SIDE_BID ? CancelBookOrder(m_mapBidOrderBook, pOrderInfo, ordstatus) : CancelBookOrder(m_mapAskOrderBook, pOrderInfo, ordstatus);
    OrderMatching();
}

void LimitOrderBook::Cancel(std::shared_ptr<OrderInfo> pOrderInfo)
{
    if(pOrderInfo == nullptr)
    {
        cout << "cancel order is NULL "<< endl;
        return;
    }
    Cancel(pOrderInfo->GetOrderID());
}

void LimitOrderBook::OrderMatching()
{
    auto bidPriceIter = m_mapBidOrderBook.begin();
    auto askPriceIter = m_mapAskOrderBook.begin();

    while (bidPriceIter != m_mapBidOrderBook.end() && askPriceIter != m_mapAskOrderBook.end())
    {
        if (bidPriceIter->first >= askPriceIter->first)
        {
            std::shared_ptr <OrderBookEntry> pBidOrdreBookEntries = bidPriceIter->second;
            std::shared_ptr <OrderBookEntry> pAskOrdreBookEntries = askPriceIter->second;

            mapOrderListQueue::iterator bidOrderQueue = pBidOrdreBookEntries->begin();
            mapOrderListQueue::iterator askOrderQueue = pAskOrdreBookEntries->begin();
            while (bidOrderQueue != pBidOrdreBookEntries->end() && askOrderQueue != pAskOrdreBookEntries->end())
            {
                if (bidOrderQueue->second->GetLeavesQuantity() == askOrderQueue->second->GetLeavesQuantity())
                {
                    int64 deltaQty = bidOrderQueue->second->GetLeavesQuantity() - askOrderQueue->second->GetLeavesQuantity();
                    bidOrderQueue->second->SetLeavesQty(deltaQty);
                    askOrderQueue->second->SetLeavesQty(deltaQty);

                    if (bidOrderQueue->second->GetLeavesQuantity() == 0)
                        pBidOrdreBookEntries->erase(bidOrderQueue); //remove from queue since fill filled
                    if (askOrderQueue->second->GetLeavesQuantity() == 0)
                        pAskOrdreBookEntries->erase(askOrderQueue);//remove from queue since fill filled
                }
                else if (bidOrderQueue->second->GetLeavesQuantity() > askOrderQueue->second->GetLeavesQuantity())
                {
                    int64 deltaQty = bidOrderQueue->second->GetLeavesQuantity() - askOrderQueue->second->GetLeavesQuantity();
                    int64 filledQty = askOrderQueue->second->GetLeavesQuantity();
                    pBidOrdreBookEntries->ReduceOrderFilledQuantity(filledQty);
                    pAskOrdreBookEntries->ReduceOrderFilledQuantity(filledQty);

                    bidOrderQueue->second->SetLeavesQty(deltaQty);
                    askOrderQueue->second->SetLeavesQty(0);

                    if (bidOrderQueue->second->GetLeavesQuantity() == 0)
                        pBidOrdreBookEntries->erase(bidOrderQueue); //remove from queue if no leaves qty

                    if (askOrderQueue->second->GetLeavesQuantity() == 0)
                        pAskOrdreBookEntries->erase(askOrderQueue); //remove from queue if no leaves qty
                }
                else if (bidOrderQueue->second->GetLeavesQuantity() < askOrderQueue->second->GetLeavesQuantity())
                {
                    int64 deltaQty = askOrderQueue->second->GetLeavesQuantity() - bidOrderQueue->second->GetLeavesQuantity();
                    int64 filledQty = bidOrderQueue->second->GetLeavesQuantity();
                    pBidOrdreBookEntries->ReduceOrderFilledQuantity(filledQty);
                    pAskOrdreBookEntries->ReduceOrderFilledQuantity(filledQty);
                    bidOrderQueue->second->SetLeavesQty(0);
                    askOrderQueue->second->SetLeavesQty(deltaQty);

                    if (bidOrderQueue->second->GetLeavesQuantity() == 0)
                        pBidOrdreBookEntries->erase(bidOrderQueue); //remove from queue if no leaves qty

                    if (askOrderQueue->second->GetLeavesQuantity() == 0)
                        pAskOrdreBookEntries->erase(askOrderQueue); //remove from queue if no leaves qty
                }
                bidOrderQueue = pBidOrdreBookEntries->begin();
                askOrderQueue = pAskOrdreBookEntries->begin();
            }
            if (pBidOrdreBookEntries->IsEmptyQueue())
            {
                m_mapBidOrderBook.erase(bidPriceIter->first);
            }
            if (pAskOrdreBookEntries->IsEmptyQueue())
            {
                m_mapAskOrderBook.erase(askPriceIter->first);
            }
        }
        else
        {
            break;
        }
        bidPriceIter = m_mapBidOrderBook.begin();
        askPriceIter = m_mapAskOrderBook.begin();
    }
}

void LimitOrderBook::PrintOrderBook()
{
    GetBook(m_mapBidOrderBook, enOrdSide::SIDE_BID);
    GetBook(m_mapAskOrderBook, enOrdSide::SIDE_ASK);
}

QueryOrderLevelResult LimitOrderBook::QueryOrderBookLevel(int querylevel, enOrdSide side)
{
    return side == enOrdSide::SIDE_BID ? QueryOrderBookLevel(m_mapBidOrderBook,querylevel) : QueryOrderBookLevel(m_mapAskOrderBook,querylevel);
}


QueryOrderLevelResult LimitOrderBook::QueryOrderStatus(uint64 ord_id)
{
    QueryOrderLevelResult orderstatus;
    const std::shared_ptr<OrderInfo> pOrderInfo = GetOrderInfo(ord_id);
    if (pOrderInfo == nullptr)
    {
        cout << " order not found to query the order status ord_id( " << ord_id << " )" << endl;
        return orderstatus;
    }
    return pOrderInfo->GetSide() == enOrdSide::SIDE_BID ? QueryOrderStatus(m_mapBidOrderBook,pOrderInfo) : QueryOrderStatus(m_mapAskOrderBook,pOrderInfo);
}

