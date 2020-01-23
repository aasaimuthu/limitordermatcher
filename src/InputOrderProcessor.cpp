#include "InputOrderProcessor.h"
using namespace std;


void InputOrderProcessor::StringTokenize(const string& str, const string& delimiters, stringvector& tokens)
{
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    string::size_type pos = str.find_first_of(delimiters, lastPos);
    while (string::npos != pos || string::npos != lastPos)
    {
        string strtoken = str.substr(lastPos, pos - lastPos);
        strtoken = trim(strtoken);
        tokens.push_back(strtoken);
        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
    }
}

void InputOrderProcessor::ProcessOrderInput(const stringvector& inputorder)
{
    try
    {
        if (inputorder.size() >= 5 && inputorder[0] == "order")
        {
            //order 1000 buy 100 100.15
            uint64 ord_id = stoll(inputorder[1]);
            enOrdSide side = inputorder[2] == "buy" ? enOrdSide::SIDE_BID : enOrdSide::SIDE_ASK;
            uint64 quantity = stoll(inputorder[3]);
            uint64  price = static_cast<int64>(stof(inputorder[4]) * PRICE_MULTIPLIER);
            m_LimitOrderBook.Add(ord_id, side, price, quantity);
        }
        else if (inputorder.size() >= 2 && inputorder[0] == "cancel")
        {
            //cancel 1000
                uint64 ord_id = stoll(inputorder[1]);
        m_LimitOrderBook.Cancel(ord_id);
        }
        else if (inputorder.size() >= 5 && inputorder[0] == "amend")
        {
            //order 1004 buy 100 100.15
            //amend 1004 300 1005 600   -- amend 1004 600” will try to change order 1005 from buy 300 shares to buy 600 shares
    	    //void LimitOrderBook::Update(uint64 prev_ord_id, uint64 prevquantity, uint64 new_ord_id, uint64 newquantity)
            uint64 prev_ord_id = stoll(inputorder[1]);
            uint64 prevquantity = stoll(inputorder[2]);
            uint64 new_ord_id = stoll(inputorder[3]);
            uint64 newquantity = stoll(inputorder[4]);
            m_LimitOrderBook.Update( prev_ord_id,  prevquantity,  new_ord_id,  newquantity);
        }
        else if (inputorder.size() >= 4 && inputorder[0] == "q" && inputorder[1] == "level")
        {
            enOrdSide side = inputorder[2] == "bid" ? enOrdSide::SIDE_BID : enOrdSide::SIDE_ASK;
            int level = stoi(inputorder[3]);
            QueryOrderLevelResult result = m_LimitOrderBook.QueryOrderBookLevel(level, side);
            cout << (side == enOrdSide::SIDE_BID ? "  bid" : "  ask")  << ""
                    << "    "  << level
                    <<"    "  << result.m_dblPrice
                    << "    "  << result.m_nQuantity
                    << endl;
        }
        else if (inputorder.size() >= 3 && inputorder[0] == "q" && inputorder[1] == "order")
        {
            uint64 prev_ord_id = stoll(inputorder[2]);
            QueryOrderLevelResult result = m_LimitOrderBook.QueryOrderStatus(prev_ord_id);
            if(result.m_enOrdStatus == enOrdStatus::ORDER_STAT_REPLACED || result.m_enOrdStatus ==  enOrdStatus::ORDER_STAT_NEW)
                cout << " open "  ;
            else
                cout << GetOrderStatusStr(result.m_enOrdStatus)  ;
            cout << " queuepos " << result.m_QueuePosition   << endl;
        }
	}
    catch(...)
    {
        cout << " invalid input...." << endl;
    }
    //m_LimitOrderBook.PrintOrderBook();
}
