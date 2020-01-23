#pragma once

#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test_framework.hpp>
#include<iostream>
#include "LimitOrderBook.h"

using boost::unit_test::test_suite;
using boost::unit_test::test_case;
using namespace std;

class TestLimitOrder
{
public:
	TestLimitOrder(){}
    virtual ~TestLimitOrder(){}
    void TestBidPriceTick();
    void TestAskPriceTick();
    void TestAddDuplicateOrderId();
	void TestAddBidAndAskOrderBook();

	void TestAddBidSideWithOrderInQueue();
	void TestAddAskSideWithOrderInQueue();
	void TestCancelBidSideOrders();
	void TestCancelAskSideOrders();
	void TestUpdateBidSideOrder();
	void TestUpdateAskSideOrder();

	void TestCrossBidAndAskQuantityEqual_FullFill();
	void TestCrossBidPriceQuantityGreaterThanAsk_PartFill();
	void TestCrossBidQuantityLesserThanAsk_PartFill();
	void TestCrossBidQuantityToFillManyAskLevels();
	void TestCrossAskQuantityToFillManyBidLevels();

	void TestAddorderupdateandcancel();
};

class TestSuiteTestLimitOrder : public test_suite
{
public:
	TestSuiteTestLimitOrder() : test_suite("TestSuiteTestLimitOrder unitet"), instance(new TestLimitOrder())
	{
			add(BOOST_CLASS_TEST_CASE(&TestLimitOrder::TestBidPriceTick,  instance));
			add(BOOST_CLASS_TEST_CASE(&TestLimitOrder::TestAskPriceTick,  instance));
			add(BOOST_CLASS_TEST_CASE(&TestLimitOrder::TestAddDuplicateOrderId,  instance));

			add(BOOST_CLASS_TEST_CASE(&TestLimitOrder::TestAddBidAndAskOrderBook,  instance));
			add(BOOST_CLASS_TEST_CASE(&TestLimitOrder::TestAddBidSideWithOrderInQueue,  instance));
			add(BOOST_CLASS_TEST_CASE(&TestLimitOrder::TestAddAskSideWithOrderInQueue,  instance));
			add(BOOST_CLASS_TEST_CASE(&TestLimitOrder::TestCancelBidSideOrders,  instance));
			add(BOOST_CLASS_TEST_CASE(&TestLimitOrder::TestCancelAskSideOrders,  instance));
			add(BOOST_CLASS_TEST_CASE(&TestLimitOrder::TestUpdateBidSideOrder,  instance));
			add(BOOST_CLASS_TEST_CASE(&TestLimitOrder::TestUpdateAskSideOrder,  instance));

			add(BOOST_CLASS_TEST_CASE(&TestLimitOrder::TestCrossBidAndAskQuantityEqual_FullFill,  instance));
			add(BOOST_CLASS_TEST_CASE(&TestLimitOrder::TestCrossBidPriceQuantityGreaterThanAsk_PartFill,  instance));
			add(BOOST_CLASS_TEST_CASE(&TestLimitOrder::TestCrossBidQuantityLesserThanAsk_PartFill,  instance));
			add(BOOST_CLASS_TEST_CASE(&TestLimitOrder::TestCrossBidQuantityToFillManyAskLevels,  instance));
			add(BOOST_CLASS_TEST_CASE(&TestLimitOrder::TestCrossAskQuantityToFillManyBidLevels,  instance));
			add(BOOST_CLASS_TEST_CASE(&TestLimitOrder::TestAddorderupdateandcancel,  instance));

	}
private:
	boost::shared_ptr<TestLimitOrder> instance;
};

