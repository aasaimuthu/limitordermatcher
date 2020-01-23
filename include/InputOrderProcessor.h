#pragma once
#include"Common.h"
#include"LimitOrderBook.h"

class InputOrderProcessor
{
public:
    static void StringTokenize(const std::string& str, const std::string& delimiters, stringvector& tokens);
    void ProcessOrderInput(const stringvector& inputorder);
private:
    LimitOrderBook m_LimitOrderBook;
};
