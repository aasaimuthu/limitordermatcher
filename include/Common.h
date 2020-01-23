#pragma once

#include <cinttypes>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <set>
#include <unordered_map>
#include <string>
#include <unordered_map>
#include <memory>


#define LOG_HEADER __LINE__ << ":"<< __FILE__ <<":" << __func__ << ":"

typedef int8_t int8;
typedef uint8_t uint8;
typedef int32_t int32;
typedef int64_t int64;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef std::vector<std::string> stringvector;
static const uint32_t PRICE_MULTIPLIER = 100;

enum class enOrdSide
{
    SIDE_UNKNOWN = '0',
    SIDE_BID = 'B',
    SIDE_ASK = 'A'
};

enum class enOrdStatus
{
    ORDER_STAT_UNKNOWN = -1,
    ORDER_STAT_NEW = 0,
    ORDER_STAT_PART_FILLED = 2,
    ORDER_STAT_FILLED = 3,
    ORDER_STAT_PENDING_CANCEL = 4,
    ORDER_STAT_CANCELLED = 5, 
	ORDER_STAT_PENDING_REPLACE = 6,
    ORDER_STAT_REPLACED = 7,
	ORDER_STAT_REJECTED = 8
};

static std::string GetOrderStatusStr(enOrdStatus stat)
{
    switch (stat)
    {
        case enOrdStatus::ORDER_STAT_NEW:
            return "NEW";
        case enOrdStatus::ORDER_STAT_PART_FILLED:
            return "PART_FILLED";
        case enOrdStatus::ORDER_STAT_FILLED:
            return "FILLED";
        case enOrdStatus::ORDER_STAT_PENDING_CANCEL:
            return "PENDING_CANCEL";
        case enOrdStatus::ORDER_STAT_CANCELLED:
            return "CANCELLED";
        case enOrdStatus::ORDER_STAT_PENDING_REPLACE:
            return "PENDING_REPLACE";
        case enOrdStatus::ORDER_STAT_REPLACED:
            return "REPLACED";
        case enOrdStatus::ORDER_STAT_REJECTED:
            return "REJECTED";
        default:
            return "UNKNOWN";
    }
}

struct QueryOrderLevelResult
{
    QueryOrderLevelResult() :m_nQuantity(0), m_dblPrice(0), m_NumItem() ,
                             m_enOrdStatus(enOrdStatus::ORDER_STAT_UNKNOWN),
                             m_QueuePosition(-1),
                             m_eOrdSide(enOrdSide::SIDE_UNKNOWN){}

    uint64     m_nQuantity;
    double     m_dblPrice;
    uint64     m_NumItem;
    enOrdStatus     m_enOrdStatus;
    int32          m_QueuePosition;
    enOrdSide      m_eOrdSide;
};

static std::string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    str.erase(0, str.find_first_not_of(chars));
    return str;
}

static  std::string& rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}

static  std::string& trim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    return ltrim(rtrim(str, chars), chars);
}

