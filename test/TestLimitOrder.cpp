#include "TestLimitOrder.h"

using namespace std;

void TestLimitOrder::TestBidPriceTick()
{
	LimitOrderBook matcher;
	std::shared_ptr<OrderInfo> bidOrder1(new OrderInfo(10000, enOrdSide::SIDE_BID, 1001,  100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder1);
	BOOST_REQUIRE(bidOrder1->GetOrderStatus() == enOrdStatus::ORDER_STAT_REJECTED);

	std::shared_ptr<OrderInfo> bidOrder2(new OrderInfo(10000, enOrdSide::SIDE_BID, 1005,  100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder2);
	BOOST_REQUIRE(bidOrder2->GetOrderStatus() == enOrdStatus::ORDER_STAT_NEW);
}

void TestLimitOrder::TestAskPriceTick()
{
	LimitOrderBook matcher;
	std::shared_ptr<OrderInfo> askOrder1(new OrderInfo(10000, enOrdSide::SIDE_BID, 1001,  100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(askOrder1);
	BOOST_REQUIRE(askOrder1->GetOrderStatus() == enOrdStatus::ORDER_STAT_REJECTED);

	std::shared_ptr<OrderInfo> askOrder2(new OrderInfo(10000, enOrdSide::SIDE_BID, 1005,  100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(askOrder2);
	BOOST_REQUIRE(askOrder2->GetOrderStatus() == enOrdStatus::ORDER_STAT_NEW);
}

void TestLimitOrder::TestAddDuplicateOrderId()
{
	LimitOrderBook matcher;
	std::shared_ptr<OrderInfo> bidOrder1(new OrderInfo(10000, enOrdSide::SIDE_BID, 1005,  100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder1);
	BOOST_REQUIRE(bidOrder1->GetOrderStatus() == enOrdStatus::ORDER_STAT_NEW);

	std::shared_ptr<OrderInfo> bidOrder2(new OrderInfo(10000, enOrdSide::SIDE_BID, 1005,  100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder2);
	BOOST_REQUIRE(bidOrder2->GetOrderStatus() == enOrdStatus::ORDER_STAT_REJECTED);

}
void TestLimitOrder::TestAddBidAndAskOrderBook()
{
	LimitOrderBook matcher;

	std::shared_ptr<OrderInfo> bidOrder1(new OrderInfo(10000, enOrdSide::SIDE_BID, 1005,  100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder1);

	std::shared_ptr<OrderInfo> bidOrder2(new OrderInfo(10001, enOrdSide::SIDE_BID, 1010, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder2);

	std::shared_ptr<OrderInfo> bidOrder3(new OrderInfo(10002, enOrdSide::SIDE_BID, 1015, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder3);

	std::shared_ptr<OrderInfo> askOrder1(new OrderInfo(20000, enOrdSide::SIDE_ASK, 1020, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(askOrder1);

	std::shared_ptr<OrderInfo> askOrder2(new OrderInfo(20001, enOrdSide::SIDE_ASK, 1025, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(askOrder2);

	std::shared_ptr<OrderInfo> askOrder3(new OrderInfo(20002, enOrdSide::SIDE_ASK, 1030, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(askOrder3);

	matcher.PrintOrderBook();
	QueryOrderLevelResult result = matcher.QueryOrderBookLevel(0,enOrdSide::SIDE_BID);
	BOOST_REQUIRE(result.m_nQuantity == 100 && result.m_dblPrice == 10.15 && result.m_NumItem == 1);

	result = matcher.QueryOrderBookLevel(2,enOrdSide::SIDE_BID);
	BOOST_REQUIRE(result.m_nQuantity == 100 && result.m_dblPrice == 10.05 && result.m_NumItem == 1);

	result = matcher.QueryOrderBookLevel(0, enOrdSide::SIDE_ASK);
	BOOST_REQUIRE(result.m_nQuantity == 100 && result.m_dblPrice == 10.20 && result.m_NumItem == 1);

	result = matcher.QueryOrderBookLevel(2,enOrdSide::SIDE_ASK);
	BOOST_REQUIRE(result.m_nQuantity == 100 && result.m_dblPrice == 10.3 && result.m_NumItem == 1 );
}

void TestLimitOrder::TestAddBidSideWithOrderInQueue()
{
	LimitOrderBook matcher;
	std::shared_ptr<OrderInfo> bidOrder1(new OrderInfo(10000, enOrdSide::SIDE_BID, 1005, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder1);

	std::shared_ptr<OrderInfo> bidOrder2(new OrderInfo(10001, enOrdSide::SIDE_BID, 1010, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder2);

	std::shared_ptr<OrderInfo> bidOrder3(new OrderInfo(10002, enOrdSide::SIDE_BID, 1015, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder3);

	QueryOrderLevelResult result = matcher.QueryOrderBookLevel(0,enOrdSide::SIDE_BID);
	BOOST_REQUIRE(result.m_nQuantity == 100);
	BOOST_REQUIRE(result.m_dblPrice == 10.15);
	BOOST_REQUIRE(result.m_NumItem == 1);

	result = matcher.QueryOrderBookLevel(2,enOrdSide::SIDE_BID);
	BOOST_REQUIRE(result.m_nQuantity == 100);
	BOOST_REQUIRE(result.m_dblPrice == 10.05);
	BOOST_REQUIRE(result.m_NumItem == 1);

	std::shared_ptr<OrderInfo> bidOrder4(new OrderInfo(10003, enOrdSide::SIDE_BID, 1005, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder4);

	std::shared_ptr<OrderInfo> bidOrder5(new OrderInfo(10004, enOrdSide::SIDE_BID, 1010, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder5);

	std::shared_ptr<OrderInfo> bidOrder6(new OrderInfo(10005, enOrdSide::SIDE_BID, 1015, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder6);


	std::shared_ptr<OrderInfo> bidOrder7(new OrderInfo(10006, enOrdSide::SIDE_BID, 1005, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder7);

	std::shared_ptr<OrderInfo> bidOrder8(new OrderInfo(10007, enOrdSide::SIDE_BID, 1015, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder8);

	matcher.PrintOrderBook();

	result = matcher.QueryOrderBookLevel(0,enOrdSide::SIDE_BID);
	BOOST_REQUIRE(result.m_nQuantity == 300);
	BOOST_REQUIRE(result.m_dblPrice == 10.15);
	BOOST_REQUIRE(result.m_NumItem == 3);

	result = matcher.QueryOrderBookLevel(1,enOrdSide::SIDE_BID);
	BOOST_REQUIRE(result.m_nQuantity == 200);
	BOOST_REQUIRE(result.m_dblPrice == 10.10);
	BOOST_REQUIRE(result.m_NumItem == 2);

	result = matcher.QueryOrderBookLevel(2,enOrdSide::SIDE_BID);
	BOOST_REQUIRE(result.m_nQuantity == 300);
	BOOST_REQUIRE(result.m_dblPrice == 10.05);
	BOOST_REQUIRE(result.m_NumItem == 3);
}

void TestLimitOrder::TestAddAskSideWithOrderInQueue()
{
	LimitOrderBook matcher;
	std::shared_ptr<OrderInfo> AskOrder1(new OrderInfo(10000, enOrdSide::SIDE_ASK, 1005, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(AskOrder1);

	std::shared_ptr<OrderInfo> AskOrder2(new OrderInfo(10001, enOrdSide::SIDE_ASK, 1010, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(AskOrder2);

	std::shared_ptr<OrderInfo> AskOrder3(new OrderInfo(10002, enOrdSide::SIDE_ASK, 1015, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(AskOrder3);
	matcher.PrintOrderBook();

	QueryOrderLevelResult result = matcher.QueryOrderBookLevel(0,enOrdSide::SIDE_ASK);
	BOOST_REQUIRE(result.m_nQuantity == 100);
	BOOST_REQUIRE(result.m_dblPrice == 10.05);
	BOOST_REQUIRE(result.m_NumItem == 1);

	result = matcher.QueryOrderBookLevel(2,enOrdSide::SIDE_ASK);
	BOOST_REQUIRE(result.m_nQuantity == 100);
	BOOST_REQUIRE(result.m_dblPrice == 10.15);
	BOOST_REQUIRE(result.m_NumItem == 1);

	std::shared_ptr<OrderInfo> AskOrder4(new OrderInfo(10003, enOrdSide::SIDE_ASK, 1005, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(AskOrder4);

	std::shared_ptr<OrderInfo> AskOrder5(new OrderInfo(10004, enOrdSide::SIDE_ASK, 1010, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(AskOrder5);

	std::shared_ptr<OrderInfo> AskOrder6(new OrderInfo(10005, enOrdSide::SIDE_ASK, 1015, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(AskOrder6);

	std::shared_ptr<OrderInfo> AskOrder7(new OrderInfo(10006, enOrdSide::SIDE_ASK, 1005, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(AskOrder7);

	std::shared_ptr<OrderInfo> AskOrder8(new OrderInfo(10007, enOrdSide::SIDE_ASK, 1015, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(AskOrder8);

	matcher.PrintOrderBook();
	result = matcher.QueryOrderBookLevel(0,enOrdSide::SIDE_ASK);
	BOOST_REQUIRE(result.m_nQuantity == 300);
	BOOST_REQUIRE(result.m_dblPrice == 10.05);
	BOOST_REQUIRE(result.m_NumItem == 3);

	result = matcher.QueryOrderBookLevel(1,enOrdSide::SIDE_ASK);
	BOOST_REQUIRE(result.m_nQuantity == 200);
	BOOST_REQUIRE(result.m_dblPrice == 10.10);
	BOOST_REQUIRE(result.m_NumItem == 2);

	result = matcher.QueryOrderBookLevel(2,enOrdSide::SIDE_ASK);
	BOOST_REQUIRE(result.m_nQuantity == 300);
	BOOST_REQUIRE(result.m_dblPrice == 10.15);
	BOOST_REQUIRE(result.m_NumItem == 3);
}

void TestLimitOrder::TestCancelBidSideOrders()
{
	LimitOrderBook matcher;
	std::shared_ptr<OrderInfo> bidOrder1(new OrderInfo(10000, enOrdSide::SIDE_BID, 1005, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder1);

	std::shared_ptr<OrderInfo> bidOrder2(new OrderInfo(10001, enOrdSide::SIDE_BID, 1010, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder2);

	std::shared_ptr<OrderInfo> bidOrder3(new OrderInfo(10002, enOrdSide::SIDE_BID, 1015, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder3);

	std::shared_ptr<OrderInfo> bidOrder4(new OrderInfo(10003, enOrdSide::SIDE_BID, 1005, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder4);

	std::shared_ptr<OrderInfo> bidOrder5(new OrderInfo(10004, enOrdSide::SIDE_BID, 1010, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder5);

	std::shared_ptr<OrderInfo> bidOrder6(new OrderInfo(10005, enOrdSide::SIDE_BID, 1015, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder6);
	matcher.PrintOrderBook();

	QueryOrderLevelResult result = matcher.QueryOrderBookLevel(0,enOrdSide::SIDE_BID);
	BOOST_REQUIRE(result.m_nQuantity == 200);
	BOOST_REQUIRE(result.m_dblPrice == 10.15);
	BOOST_REQUIRE(result.m_NumItem == 2);

	result = matcher.QueryOrderBookLevel(2,enOrdSide::SIDE_BID);
	BOOST_REQUIRE(result.m_nQuantity == 200);
	BOOST_REQUIRE(result.m_dblPrice == 10.05);
	BOOST_REQUIRE(result.m_NumItem == 2);

	std::shared_ptr<OrderInfo> bidCancelOrder_10000(new OrderInfo(10000, enOrdSide::SIDE_BID, 0, 0, enOrdStatus::ORDER_STAT_PENDING_CANCEL));
	matcher.Cancel(bidCancelOrder_10000);
	std::shared_ptr<OrderInfo> bidCancelOrder_10003(new OrderInfo(10003, enOrdSide::SIDE_BID, 0, 0, enOrdStatus::ORDER_STAT_PENDING_CANCEL));
	matcher.Cancel(bidCancelOrder_10003);

	result = matcher.QueryOrderBookLevel(2,enOrdSide::SIDE_BID);
	BOOST_REQUIRE(result.m_nQuantity == 0);
	BOOST_REQUIRE(result.m_dblPrice == 0);
	BOOST_REQUIRE(result.m_NumItem == 0);
	matcher.PrintOrderBook();

	result = matcher.QueryOrderStatus(10000);
	BOOST_REQUIRE(result.m_enOrdStatus == enOrdStatus::ORDER_STAT_CANCELLED);
	BOOST_REQUIRE(result.m_QueuePosition == -1);

	result = matcher.QueryOrderStatus(10003);
	BOOST_REQUIRE(result.m_enOrdStatus == enOrdStatus::ORDER_STAT_CANCELLED);
	BOOST_REQUIRE(result.m_QueuePosition == -1);
}

void TestLimitOrder::TestCancelAskSideOrders()
{
	LimitOrderBook matcher;
	std::shared_ptr<OrderInfo> askOrder1(new OrderInfo(10000, enOrdSide::SIDE_ASK, 1005, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(askOrder1);

	std::shared_ptr<OrderInfo> askOrder2(new OrderInfo(10001, enOrdSide::SIDE_ASK, 1010, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(askOrder2);

	std::shared_ptr<OrderInfo> askOrder3(new OrderInfo(10002, enOrdSide::SIDE_ASK, 1015, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(askOrder3);

	std::shared_ptr<OrderInfo> askOrder4(new OrderInfo(10003, enOrdSide::SIDE_ASK, 1005, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(askOrder4);

	std::shared_ptr<OrderInfo> askOrder5(new OrderInfo(10004, enOrdSide::SIDE_ASK, 1010, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(askOrder5);

	std::shared_ptr<OrderInfo> askOrder6(new OrderInfo(10005, enOrdSide::SIDE_ASK, 1015, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(askOrder6);
	matcher.PrintOrderBook();

	QueryOrderLevelResult result = matcher.QueryOrderBookLevel(0,enOrdSide::SIDE_ASK);
	BOOST_REQUIRE(result.m_nQuantity == 200);
	BOOST_REQUIRE(result.m_dblPrice == 10.05);
	BOOST_REQUIRE(result.m_NumItem == 2);

	result = matcher.QueryOrderBookLevel(2,enOrdSide::SIDE_ASK);
	BOOST_REQUIRE(result.m_nQuantity == 200);
	BOOST_REQUIRE(result.m_dblPrice == 10.15);
	BOOST_REQUIRE(result.m_NumItem == 2);

	std::shared_ptr<OrderInfo> askCancelOrder_10000(new OrderInfo(10000, enOrdSide::SIDE_ASK, 0, 0, enOrdStatus::ORDER_STAT_PENDING_CANCEL));
	matcher.Cancel(askCancelOrder_10000);

	std::shared_ptr<OrderInfo> askCancelOrder_10003(new OrderInfo(10003, enOrdSide::SIDE_ASK, 0, 0, enOrdStatus::ORDER_STAT_PENDING_CANCEL));
	matcher.Cancel(askCancelOrder_10003);
	matcher.PrintOrderBook();

	result = matcher.QueryOrderBookLevel(2,enOrdSide::SIDE_ASK);
	BOOST_REQUIRE(result.m_nQuantity == 0);
	BOOST_REQUIRE(result.m_dblPrice == 0);
	BOOST_REQUIRE(result.m_NumItem == 0);

	matcher.PrintOrderBook();
	result = matcher.QueryOrderStatus(10000);
	BOOST_REQUIRE(result.m_enOrdStatus == enOrdStatus::ORDER_STAT_CANCELLED);
	BOOST_REQUIRE(result.m_QueuePosition == -1);

	result = matcher.QueryOrderStatus(10003);
	BOOST_REQUIRE(result.m_enOrdStatus == enOrdStatus::ORDER_STAT_CANCELLED);
	BOOST_REQUIRE(result.m_QueuePosition == -1);
}

void TestLimitOrder::TestUpdateBidSideOrder()
{
	LimitOrderBook matcher;
	std::shared_ptr<OrderInfo> bidOrder1(new OrderInfo(10000, enOrdSide::SIDE_BID, 1005, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder1);

	std::shared_ptr<OrderInfo> bidOrder2(new OrderInfo(10001, enOrdSide::SIDE_BID, 1010, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder2);

	std::shared_ptr<OrderInfo> bidOrder3(new OrderInfo(10002, enOrdSide::SIDE_BID, 1015, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder3);

	std::shared_ptr<OrderInfo> bidOrder4(new OrderInfo(10003, enOrdSide::SIDE_BID, 1005, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder4);

	std::shared_ptr<OrderInfo> bidOrder5(new OrderInfo(10004, enOrdSide::SIDE_BID, 1010, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder5);

	std::shared_ptr<OrderInfo> bidOrder6(new OrderInfo(10005, enOrdSide::SIDE_BID, 1015, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder6);

	matcher.PrintOrderBook();

	QueryOrderLevelResult result = matcher.QueryOrderBookLevel(0,enOrdSide::SIDE_BID);
	BOOST_REQUIRE(result.m_nQuantity == 200);
	BOOST_REQUIRE(result.m_dblPrice == 10.15);
	BOOST_REQUIRE(result.m_NumItem == 2);

	result = matcher.QueryOrderBookLevel(2,enOrdSide::SIDE_BID);
	BOOST_REQUIRE(result.m_nQuantity == 200);
	BOOST_REQUIRE(result.m_dblPrice == 10.05);
	BOOST_REQUIRE(result.m_NumItem == 2);

	result = matcher.QueryOrderStatus(10001);
	BOOST_REQUIRE(result.m_enOrdStatus == enOrdStatus::ORDER_STAT_NEW);
	BOOST_REQUIRE(result.m_QueuePosition == 0);

	result = matcher.QueryOrderStatus(10002);
	BOOST_REQUIRE(result.m_enOrdStatus == enOrdStatus::ORDER_STAT_NEW);
	BOOST_REQUIRE(result.m_QueuePosition == 0);

	std::shared_ptr<OrderInfo> bidUpdateOrder_3_10006(new OrderInfo(10006, enOrdSide::SIDE_BID, 1015, 50, enOrdStatus::ORDER_STAT_PENDING_REPLACE, 10002));
	matcher.Update(bidUpdateOrder_3_10006);

	matcher.PrintOrderBook();
	std::shared_ptr<OrderInfo> bidUpdateOrder_2_10007(new OrderInfo(10007, enOrdSide::SIDE_BID, 1010, 150, enOrdStatus::ORDER_STAT_PENDING_REPLACE, 10001));
	matcher.Update(bidUpdateOrder_2_10007);

	matcher.PrintOrderBook();

	result = matcher.QueryOrderBookLevel(0,enOrdSide::SIDE_BID);
	BOOST_REQUIRE(result.m_nQuantity == 150);
	BOOST_REQUIRE(result.m_dblPrice == 10.15);
	BOOST_REQUIRE(result.m_NumItem == 2);

	result = matcher.QueryOrderBookLevel(1,enOrdSide::SIDE_BID);
	BOOST_REQUIRE(result.m_nQuantity == 250);
	BOOST_REQUIRE(result.m_dblPrice == 10.10);
	BOOST_REQUIRE(result.m_NumItem == 2);

	result = matcher.QueryOrderBookLevel(2,enOrdSide::SIDE_BID);
	BOOST_REQUIRE(result.m_nQuantity == 200);
	BOOST_REQUIRE(result.m_dblPrice == 10.05);
	BOOST_REQUIRE(result.m_NumItem == 2);


	result = matcher.QueryOrderStatus(10002);
	BOOST_REQUIRE(result.m_enOrdStatus == enOrdStatus::ORDER_STAT_CANCELLED);
	BOOST_REQUIRE(result.m_QueuePosition == -1);

	result = matcher.QueryOrderStatus(10006);
	BOOST_REQUIRE(result.m_enOrdStatus == enOrdStatus::ORDER_STAT_REPLACED);
	BOOST_REQUIRE(result.m_QueuePosition == 0);
	result = matcher.QueryOrderStatus(10001);
	BOOST_REQUIRE(result.m_enOrdStatus == enOrdStatus::ORDER_STAT_CANCELLED);
	BOOST_REQUIRE(result.m_QueuePosition == -1);
	result = matcher.QueryOrderStatus(10007);
	BOOST_REQUIRE(result.m_enOrdStatus == enOrdStatus::ORDER_STAT_REPLACED);
	BOOST_REQUIRE(result.m_QueuePosition == 1);
}

void TestLimitOrder::TestUpdateAskSideOrder()
{
	LimitOrderBook matcher;
	std::shared_ptr<OrderInfo> AskOrder1(new OrderInfo(10000, enOrdSide::SIDE_ASK, 1005, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(AskOrder1);

	std::shared_ptr<OrderInfo> AskOrder2(new OrderInfo(10001, enOrdSide::SIDE_ASK, 1010, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(AskOrder2);

	std::shared_ptr<OrderInfo> AskOrder3(new OrderInfo(10002, enOrdSide::SIDE_ASK, 1015, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(AskOrder3);

	std::shared_ptr<OrderInfo> AskOrder4(new OrderInfo(10003, enOrdSide::SIDE_ASK, 1005, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(AskOrder4);

	std::shared_ptr<OrderInfo> AskOrder5(new OrderInfo(10004, enOrdSide::SIDE_ASK, 1010, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(AskOrder5);

	std::shared_ptr<OrderInfo> AskOrder6(new OrderInfo(10005, enOrdSide::SIDE_ASK, 1015, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(AskOrder6);

	matcher.PrintOrderBook();

	QueryOrderLevelResult result = matcher.QueryOrderBookLevel(0,enOrdSide::SIDE_ASK);
	BOOST_REQUIRE(result.m_nQuantity == 200);
	BOOST_REQUIRE(result.m_dblPrice == 10.05);
	BOOST_REQUIRE(result.m_NumItem == 2);

	result = matcher.QueryOrderBookLevel(2,enOrdSide::SIDE_ASK);
	BOOST_REQUIRE(result.m_nQuantity == 200);
	BOOST_REQUIRE(result.m_dblPrice == 10.15);
	BOOST_REQUIRE(result.m_NumItem == 2);

	result = matcher.QueryOrderStatus(10001);
	BOOST_REQUIRE(result.m_enOrdStatus == enOrdStatus::ORDER_STAT_NEW);
	BOOST_REQUIRE(result.m_QueuePosition == 0);

	result = matcher.QueryOrderStatus(10002);
	BOOST_REQUIRE(result.m_enOrdStatus == enOrdStatus::ORDER_STAT_NEW);
	BOOST_REQUIRE(result.m_QueuePosition == 0);

	std::shared_ptr<OrderInfo> AskUpdateOrder_3_10006(new OrderInfo(10006, enOrdSide::SIDE_ASK, 1015, 50, enOrdStatus::ORDER_STAT_PENDING_REPLACE, 10002));
	matcher.Update(AskUpdateOrder_3_10006);

	matcher.PrintOrderBook();
	std::shared_ptr<OrderInfo> AskUpdateOrder_2_10007(new OrderInfo(10007, enOrdSide::SIDE_ASK, 1010, 150, enOrdStatus::ORDER_STAT_PENDING_REPLACE, 10001));
	matcher.Update(AskUpdateOrder_2_10007);

	matcher.PrintOrderBook();

	result = matcher.QueryOrderBookLevel(0,enOrdSide::SIDE_ASK);
	BOOST_REQUIRE(result.m_nQuantity == 200);
	BOOST_REQUIRE(result.m_dblPrice == 10.05);
	BOOST_REQUIRE(result.m_NumItem == 2);

	result = matcher.QueryOrderBookLevel(1,enOrdSide::SIDE_ASK);
	BOOST_REQUIRE(result.m_nQuantity == 250);
	BOOST_REQUIRE(result.m_dblPrice == 10.10);
	BOOST_REQUIRE(result.m_NumItem == 2);

	result = matcher.QueryOrderBookLevel(2,enOrdSide::SIDE_ASK);
	BOOST_REQUIRE(result.m_nQuantity == 150);
	BOOST_REQUIRE(result.m_dblPrice == 10.15);
	BOOST_REQUIRE(result.m_NumItem == 2);

	result = matcher.QueryOrderStatus(10002);
	BOOST_REQUIRE(result.m_enOrdStatus == enOrdStatus::ORDER_STAT_CANCELLED);
	BOOST_REQUIRE(result.m_QueuePosition == -1);

	result = matcher.QueryOrderStatus(10006);
	BOOST_REQUIRE(result.m_enOrdStatus == enOrdStatus::ORDER_STAT_REPLACED);
	BOOST_REQUIRE(result.m_QueuePosition == 0);

	result = matcher.QueryOrderStatus(10001);
	BOOST_REQUIRE(result.m_enOrdStatus == enOrdStatus::ORDER_STAT_CANCELLED);
	BOOST_REQUIRE(result.m_QueuePosition == -1);
	result = matcher.QueryOrderStatus(10007);
	BOOST_REQUIRE(result.m_enOrdStatus == enOrdStatus::ORDER_STAT_REPLACED);
	BOOST_REQUIRE(result.m_QueuePosition == 1);
}

void TestLimitOrder::TestCrossBidAndAskQuantityEqual_FullFill()
{
/*
Bid: 	      Ask:
10.15 		10.20
10.10		10.25
10.05		10.30
1. market snap snapshot
2. place BID order with Size 10.15
*/
	LimitOrderBook matcher;
	std::shared_ptr<OrderInfo> bidOrder1(new OrderInfo(10000, enOrdSide::SIDE_BID, 1005, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder1);

	std::shared_ptr<OrderInfo> bidOrder2(new OrderInfo(10001, enOrdSide::SIDE_BID, 1010, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder2);

	std::shared_ptr<OrderInfo> bidOrder3(new OrderInfo(10002, enOrdSide::SIDE_BID, 1015, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder3);

	std::shared_ptr<OrderInfo> askOrder1(new OrderInfo(20000, enOrdSide::SIDE_ASK, 1020, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(askOrder1);

	std::shared_ptr<OrderInfo> askOrder2(new OrderInfo(20001, enOrdSide::SIDE_ASK, 1025, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(askOrder2);

	std::shared_ptr<OrderInfo> askOrder3(new OrderInfo(20002, enOrdSide::SIDE_ASK, 1030, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(askOrder3);

	matcher.PrintOrderBook();

	std::shared_ptr<OrderInfo> bidOrder4_eqlask(new OrderInfo(10003, enOrdSide::SIDE_BID, 1020, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder4_eqlask);
	matcher.PrintOrderBook();

	QueryOrderLevelResult result = matcher.QueryOrderBookLevel(2,enOrdSide::SIDE_ASK);
	BOOST_REQUIRE(result.m_nQuantity == 0);
	BOOST_REQUIRE(result.m_dblPrice == 0);
	BOOST_REQUIRE(result.m_NumItem == 0);

	result = matcher.QueryOrderBookLevel(1,enOrdSide::SIDE_ASK);
	BOOST_REQUIRE(result.m_nQuantity == 100);
	BOOST_REQUIRE(result.m_dblPrice == 10.3);
	BOOST_REQUIRE(result.m_NumItem == 1);

	result = matcher.QueryOrderBookLevel(0,enOrdSide::SIDE_ASK);
	BOOST_REQUIRE(result.m_nQuantity == 100);
	BOOST_REQUIRE(result.m_dblPrice == 10.25);
	BOOST_REQUIRE(result.m_NumItem == 1);

	result = matcher.QueryOrderStatus(10003);
	BOOST_REQUIRE(result.m_enOrdStatus == enOrdStatus::ORDER_STAT_FILLED);
	BOOST_REQUIRE(result.m_QueuePosition == -1);

	result = matcher.QueryOrderStatus(20000);
	BOOST_REQUIRE(result.m_enOrdStatus == enOrdStatus::ORDER_STAT_FILLED);
	BOOST_REQUIRE(result.m_QueuePosition == -1);
}

void TestLimitOrder::TestCrossBidPriceQuantityGreaterThanAsk_PartFill()
{
	LimitOrderBook matcher;
	std::shared_ptr<OrderInfo> bidOrder1(new OrderInfo(10000, enOrdSide::SIDE_BID, 1005, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder1);

	std::shared_ptr<OrderInfo> bidOrder2(new OrderInfo(10001, enOrdSide::SIDE_BID, 1010, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder2);

	std::shared_ptr<OrderInfo> bidOrder3(new OrderInfo(10002, enOrdSide::SIDE_BID, 1015, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder3);

	std::shared_ptr<OrderInfo> askOrder1(new OrderInfo(20000, enOrdSide::SIDE_ASK, 1020, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(askOrder1);

	std::shared_ptr<OrderInfo> askOrder2(new OrderInfo(20001, enOrdSide::SIDE_ASK, 1025, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(askOrder2);

	std::shared_ptr<OrderInfo> askOrder3(new OrderInfo(20002, enOrdSide::SIDE_ASK, 1030, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(askOrder3);

	matcher.PrintOrderBook();

	QueryOrderLevelResult result = matcher.QueryOrderBookLevel(0,enOrdSide::SIDE_BID);
	BOOST_REQUIRE(result.m_nQuantity == 100);
	BOOST_REQUIRE(result.m_dblPrice == 10.15);
	BOOST_REQUIRE(result.m_NumItem == 1);

	result = matcher.QueryOrderBookLevel(0,enOrdSide::SIDE_ASK);
	BOOST_REQUIRE(result.m_nQuantity == 100);
	BOOST_REQUIRE(result.m_dblPrice == 10.20);
	BOOST_REQUIRE(result.m_NumItem == 1);

	std::shared_ptr<OrderInfo> bidOrder4_Greaterthanask(new OrderInfo(10003, enOrdSide::SIDE_BID, 1020, 150, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder4_Greaterthanask);
	matcher.PrintOrderBook();

	result = matcher.QueryOrderBookLevel(0,enOrdSide::SIDE_BID);
	BOOST_REQUIRE(result.m_nQuantity == 50 );
	BOOST_REQUIRE(result.m_dblPrice == 10.20);
	BOOST_REQUIRE(result.m_NumItem == 1);

	result = matcher.QueryOrderBookLevel(0,enOrdSide::SIDE_ASK);
	BOOST_REQUIRE(result.m_nQuantity == 100);
	BOOST_REQUIRE(result.m_dblPrice == 10.25);
	BOOST_REQUIRE(result.m_NumItem == 1);
	result = matcher.QueryOrderStatus(10003);
	BOOST_REQUIRE(result.m_enOrdStatus == enOrdStatus::ORDER_STAT_PART_FILLED);
	BOOST_REQUIRE(result.m_QueuePosition == 0);
	result = matcher.QueryOrderStatus(20000);
	BOOST_REQUIRE(result.m_enOrdStatus == enOrdStatus::ORDER_STAT_FILLED);
	BOOST_REQUIRE(result.m_QueuePosition == -1);

}

void TestLimitOrder::TestCrossBidQuantityLesserThanAsk_PartFill()
{
	LimitOrderBook matcher;
	std::shared_ptr<OrderInfo> bidOrder1(new OrderInfo(10000, enOrdSide::SIDE_BID, 1005, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder1);

	std::shared_ptr<OrderInfo> bidOrder2(new OrderInfo(10001, enOrdSide::SIDE_BID, 1010, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder2);

	std::shared_ptr<OrderInfo> bidOrder3(new OrderInfo(10002, enOrdSide::SIDE_BID, 1015, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder3);

	std::shared_ptr<OrderInfo> askOrder1(new OrderInfo(20000, enOrdSide::SIDE_ASK, 1020, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(askOrder1);

	std::shared_ptr<OrderInfo> askOrder2(new OrderInfo(20001, enOrdSide::SIDE_ASK, 1025, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(askOrder2);

	std::shared_ptr<OrderInfo> askOrder3(new OrderInfo(20002, enOrdSide::SIDE_ASK, 1030, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(askOrder3);

	matcher.PrintOrderBook();

	QueryOrderLevelResult result = matcher.QueryOrderBookLevel(0,enOrdSide::SIDE_BID);
	BOOST_REQUIRE(result.m_nQuantity == 100 && result.m_dblPrice == 10.15 && result.m_NumItem == 1);

	result = matcher.QueryOrderBookLevel(0,enOrdSide::SIDE_ASK);
	BOOST_REQUIRE(result.m_nQuantity == 100 && result.m_dblPrice == 10.20 && result.m_NumItem == 1);

	std::shared_ptr<OrderInfo> bidOrder4_Greaterthanask(new OrderInfo(10003, enOrdSide::SIDE_BID, 1020, 50, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder4_Greaterthanask);
	matcher.PrintOrderBook();

	result = matcher.QueryOrderBookLevel(0,enOrdSide::SIDE_BID);
	BOOST_REQUIRE(result.m_nQuantity == 100 && result.m_dblPrice== 10.15  &&  result.m_NumItem == 1 );

	result = matcher.QueryOrderBookLevel(0,enOrdSide::SIDE_ASK);
	BOOST_REQUIRE(result.m_nQuantity == 50 && result.m_dblPrice== 10.2  &&  result.m_NumItem == 1 );
	result = matcher.QueryOrderStatus(10003);
	BOOST_REQUIRE(result.m_enOrdStatus == enOrdStatus::ORDER_STAT_FILLED && result.m_QueuePosition == -1 );

	result = matcher.QueryOrderStatus(20000);
	BOOST_REQUIRE(result.m_enOrdStatus == enOrdStatus::ORDER_STAT_PART_FILLED && result.m_QueuePosition == 0 );
}
//
void TestLimitOrder::TestCrossBidQuantityToFillManyAskLevels()
{
	LimitOrderBook matcher;
	std::shared_ptr<OrderInfo> bidOrder1(new OrderInfo(10000, enOrdSide::SIDE_BID, 1005, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder1);

	std::shared_ptr<OrderInfo> bidOrder2(new OrderInfo(10001, enOrdSide::SIDE_BID, 1010, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder2);

	std::shared_ptr<OrderInfo> bidOrder3(new OrderInfo(10002, enOrdSide::SIDE_BID, 1015, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder3);

	std::shared_ptr<OrderInfo> askOrder1(new OrderInfo(20000, enOrdSide::SIDE_ASK, 1020, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(askOrder1);

	std::shared_ptr<OrderInfo> askOrder2(new OrderInfo(20001, enOrdSide::SIDE_ASK, 1025, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(askOrder2);

	std::shared_ptr<OrderInfo> askOrder3(new OrderInfo(20002, enOrdSide::SIDE_ASK, 1030, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(askOrder3);

	QueryOrderLevelResult result = matcher.QueryOrderBookLevel(0,enOrdSide::SIDE_BID);
	BOOST_REQUIRE(result.m_nQuantity == 100 && result.m_dblPrice == 10.15 && result.m_NumItem == 1);

	result = matcher.QueryOrderBookLevel(1,enOrdSide::SIDE_BID);
	BOOST_REQUIRE(result.m_nQuantity == 100 && result.m_dblPrice == 10.10 && result.m_NumItem == 1);

	result = matcher.QueryOrderBookLevel(2,enOrdSide::SIDE_BID);
	BOOST_REQUIRE(result.m_nQuantity == 100 && result.m_dblPrice == 10.05 && result.m_NumItem == 1);


	result = matcher.QueryOrderBookLevel(0,enOrdSide::SIDE_ASK);
	BOOST_REQUIRE(result.m_nQuantity == 100 && result.m_dblPrice == 10.2 && result.m_NumItem == 1);

	result = matcher.QueryOrderBookLevel(1,enOrdSide::SIDE_ASK);
	BOOST_REQUIRE(result.m_nQuantity == 100 && result.m_dblPrice == 10.25 && result.m_NumItem == 1);

	result = matcher.QueryOrderBookLevel(2,enOrdSide::SIDE_ASK);
	BOOST_REQUIRE(result.m_nQuantity == 100 && result.m_dblPrice == 10.30 && result.m_NumItem == 1);

	matcher.PrintOrderBook();

	std::shared_ptr<OrderInfo> bidOrder4_Greaterthanask(new OrderInfo(10003, enOrdSide::SIDE_BID, 1030, 400, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder4_Greaterthanask);
	matcher.PrintOrderBook();

	result = matcher.QueryOrderBookLevel(0,enOrdSide::SIDE_BID);
	BOOST_REQUIRE(result.m_nQuantity == 100 && result.m_dblPrice == 10.3 && result.m_NumItem == 1);

	result = matcher.QueryOrderBookLevel(1,enOrdSide::SIDE_BID);
	BOOST_REQUIRE(result.m_nQuantity == 100 && result.m_dblPrice == 10.15 && result.m_NumItem == 1);

	result = matcher.QueryOrderBookLevel(2,enOrdSide::SIDE_BID);
	BOOST_REQUIRE(result.m_nQuantity == 100 && result.m_dblPrice == 10.10 && result.m_NumItem == 1);

	result = matcher.QueryOrderBookLevel(3,enOrdSide::SIDE_BID);
	BOOST_REQUIRE(result.m_nQuantity == 100 && result.m_dblPrice == 10.05 && result.m_NumItem == 1);

	result = matcher.QueryOrderBookLevel(0,enOrdSide::SIDE_ASK);
	BOOST_REQUIRE(result.m_nQuantity == 0 && result.m_dblPrice == 0 && result.m_NumItem == 0);

	result = matcher.QueryOrderBookLevel(1,enOrdSide::SIDE_ASK);
	BOOST_REQUIRE(result.m_nQuantity == 0 && result.m_dblPrice == 0 && result.m_NumItem == 0);

	result = matcher.QueryOrderBookLevel(2,enOrdSide::SIDE_ASK);
	BOOST_REQUIRE(result.m_nQuantity == 0 && result.m_dblPrice == 0 && result.m_NumItem == 0);

	result = matcher.QueryOrderStatus(10003);
	BOOST_REQUIRE(result.m_enOrdStatus == enOrdStatus::ORDER_STAT_PART_FILLED && result.m_QueuePosition == 0 );

	result = matcher.QueryOrderStatus(20000);
	BOOST_REQUIRE(result.m_enOrdStatus == enOrdStatus::ORDER_STAT_FILLED && result.m_QueuePosition == -1 );

	result = matcher.QueryOrderStatus(20001);
	BOOST_REQUIRE(result.m_enOrdStatus == enOrdStatus::ORDER_STAT_FILLED && result.m_QueuePosition == -1  );

	result = matcher.QueryOrderStatus(20002);
	BOOST_REQUIRE(result.m_enOrdStatus == enOrdStatus::ORDER_STAT_FILLED && result.m_QueuePosition == -1  );
}

void TestLimitOrder::TestCrossAskQuantityToFillManyBidLevels()
{
	LimitOrderBook matcher;
	std::shared_ptr<OrderInfo> bidOrder1(new OrderInfo(10000, enOrdSide::SIDE_BID, 1005, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder1);

	std::shared_ptr<OrderInfo> bidOrder2(new OrderInfo(10001, enOrdSide::SIDE_BID, 1010, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder2);

	std::shared_ptr<OrderInfo> bidOrder3(new OrderInfo(10002, enOrdSide::SIDE_BID, 1015, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder3);

	std::shared_ptr<OrderInfo> askOrder1(new OrderInfo(20000, enOrdSide::SIDE_ASK, 1020, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(askOrder1);

	std::shared_ptr<OrderInfo> askOrder2(new OrderInfo(20001, enOrdSide::SIDE_ASK, 1025, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(askOrder2);

	std::shared_ptr<OrderInfo> askOrder3(new OrderInfo(20002, enOrdSide::SIDE_ASK, 1030, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(askOrder3);

	matcher.PrintOrderBook();
	QueryOrderLevelResult result = matcher.QueryOrderBookLevel(0,enOrdSide::SIDE_BID);
	BOOST_REQUIRE(result.m_nQuantity == 100 && result.m_dblPrice == 10.15 && result.m_NumItem == 1);

	result = matcher.QueryOrderBookLevel(1,enOrdSide::SIDE_BID);
	BOOST_REQUIRE(result.m_nQuantity == 100 && result.m_dblPrice == 10.10 && result.m_NumItem == 1);

	result = matcher.QueryOrderBookLevel(2,enOrdSide::SIDE_BID);
	BOOST_REQUIRE(result.m_nQuantity == 100 && result.m_dblPrice == 10.05 && result.m_NumItem == 1);


	result = matcher.QueryOrderBookLevel(0,enOrdSide::SIDE_ASK);
	BOOST_REQUIRE(result.m_nQuantity == 100 && result.m_dblPrice == 10.2 && result.m_NumItem == 1);

	result = matcher.QueryOrderBookLevel(1,enOrdSide::SIDE_ASK);
	BOOST_REQUIRE(result.m_nQuantity == 100 && result.m_dblPrice == 10.25 && result.m_NumItem == 1);

	result = matcher.QueryOrderBookLevel(2,enOrdSide::SIDE_ASK);
	BOOST_REQUIRE(result.m_nQuantity == 100 && result.m_dblPrice == 10.30 && result.m_NumItem == 1);

	std::shared_ptr<OrderInfo> askOrder4_GreaterthanBid(new OrderInfo(20003, enOrdSide::SIDE_ASK, 1005, 300, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(askOrder4_GreaterthanBid);
	matcher.PrintOrderBook();

	result = matcher.QueryOrderBookLevel(0,enOrdSide::SIDE_ASK);
	BOOST_REQUIRE(result.m_nQuantity == 100 && result.m_dblPrice == 10.2 && result.m_NumItem == 1);

	result = matcher.QueryOrderBookLevel(1,enOrdSide::SIDE_ASK);
	BOOST_REQUIRE(result.m_nQuantity == 100 && result.m_dblPrice == 10.25 && result.m_NumItem == 1);

	result = matcher.QueryOrderBookLevel(2,enOrdSide::SIDE_ASK);
	BOOST_REQUIRE(result.m_nQuantity == 100 && result.m_dblPrice == 10.30 && result.m_NumItem == 1);

	result = matcher.QueryOrderBookLevel(0,enOrdSide::SIDE_BID);
	BOOST_REQUIRE(result.m_nQuantity == 0 && result.m_dblPrice == 0 && result.m_NumItem == 0);

	result = matcher.QueryOrderBookLevel(1,enOrdSide::SIDE_BID);
	BOOST_REQUIRE(result.m_nQuantity == 0 && result.m_dblPrice == 0 && result.m_NumItem == 0);

	result = matcher.QueryOrderBookLevel(2,enOrdSide::SIDE_BID);
	BOOST_REQUIRE(result.m_nQuantity == 0 && result.m_dblPrice == 0 && result.m_NumItem == 0);

	result = matcher.QueryOrderStatus(20003);
	BOOST_REQUIRE(result.m_enOrdStatus == enOrdStatus::ORDER_STAT_FILLED && result.m_QueuePosition == -1 );

	result = matcher.QueryOrderStatus(10000);
	BOOST_REQUIRE(result.m_enOrdStatus == enOrdStatus::ORDER_STAT_FILLED && result.m_QueuePosition == -1 );

	result = matcher.QueryOrderStatus(10001);
	BOOST_REQUIRE(result.m_enOrdStatus == enOrdStatus::ORDER_STAT_FILLED && result.m_QueuePosition == -1  );

	result = matcher.QueryOrderStatus(10002);
	BOOST_REQUIRE(result.m_enOrdStatus == enOrdStatus::ORDER_STAT_FILLED && result.m_QueuePosition == -1  );
}

void TestLimitOrder::TestAddorderupdateandcancel()
{
	LimitOrderBook matcher;
	std::shared_ptr<OrderInfo> bidOrder1(new OrderInfo(10000, enOrdSide::SIDE_BID, 1005, 100, enOrdStatus::ORDER_STAT_NEW));
	matcher.Add(bidOrder1);

	QueryOrderLevelResult result = matcher.QueryOrderBookLevel(0,enOrdSide::SIDE_BID);
	BOOST_REQUIRE(result.m_nQuantity == 100 && result.m_dblPrice == 10.05 && result.m_NumItem == 1);
	result = matcher.QueryOrderStatus(10000);
	BOOST_REQUIRE(result.m_enOrdStatus == enOrdStatus::ORDER_STAT_NEW && result.m_QueuePosition == 0 );

	std::shared_ptr<OrderInfo> bidOrder2(new OrderInfo( 10001, enOrdSide::SIDE_BID, 1005, 200, enOrdStatus::ORDER_STAT_PENDING_REPLACE, 10000));
	matcher.Update(bidOrder2);

	result = matcher.QueryOrderBookLevel(0,enOrdSide::SIDE_BID);
	BOOST_REQUIRE(result.m_nQuantity == 200 && result.m_dblPrice == 10.05 && result.m_NumItem == 1);
	result = matcher.QueryOrderStatus(10001);
	BOOST_REQUIRE(result.m_enOrdStatus == enOrdStatus::ORDER_STAT_REPLACED && result.m_QueuePosition == 0 );

	result = matcher.QueryOrderStatus(10000);
	BOOST_REQUIRE(result.m_enOrdStatus == enOrdStatus::ORDER_STAT_CANCELLED && result.m_QueuePosition == -1 );

	std::shared_ptr<OrderInfo> bidOrder3(new OrderInfo(10002, enOrdSide::SIDE_BID, 1005, 200, enOrdStatus::ORDER_STAT_PENDING_CANCEL));
	matcher.Cancel(bidOrder3);
	result = matcher.QueryOrderStatus(10001);
	BOOST_REQUIRE(result.m_enOrdStatus == enOrdStatus::ORDER_STAT_REPLACED && result.m_QueuePosition ==0 );
}

test_suite* init_unit_test_suite(int aArgc, char *aArgv[])
{
	test_suite *suite = BOOST_TEST_SUITE("TestSuiteTestLimitOrder suite");
	suite->add(new TestSuiteTestLimitOrder());
	return suite;
}

