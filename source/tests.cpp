#include <QByteArray>
#include <parser.h>
#include <iostream>
#include <random>
#include <chrono>
#include <render_lantern.h>
#include <log_duration.h>
#include <tests.h>

template<typename T>
std::ostream& operator <<(std::ostream& out, const std::vector<T>& v)
{
    for(const auto& item : v)
    {
        out << item << " ";
    }
    return out;
}

const size_t TLV_COUNT = 1000000;

QByteArray createByteArray()
{
    QByteArray given;

    QByteArray off;     QByteArray on;      QByteArray color;
    off.resize(3);      on.resize(3);       color.resize(6);
    off[0] = 0x13;      on[0] = 0x12;       color[0] = 0x20;    color[3] = 0xeb;
    off[1] = 0x00;      on[1] = 0x00;       color[1] = 0x00;    color[4] = 0x72;
    off[2] = 0x00;      on[2] = 0x00;       color[2] = 0x03;    color[5] = 0x2e;

    return given.append(on).append(color).append(off);
}

QByteArray createBigByteArray()
{
    QByteArray given;

    QByteArray off;     QByteArray on;      QByteArray color;
    off.resize(3);      on.resize(3);       color.resize(6);
    off[0] = 0x13;      on[0] = 0x12;       color[0] = 0x20;    color[3] = 0xeb;
    off[1] = 0x00;      on[1] = 0x00;       color[1] = 0x00;    color[4] = 0x72;
    off[2] = 0x00;      on[2] = 0x00;       color[2] = 0x03;    color[5] = 0x2e;

    std::srand(time(0));

    for(size_t i = 0; i < TLV_COUNT; ++i)
    {
        if(i == 0)
        {
            given.append(on);
            continue;
        }
        if(i == (TLV_COUNT - 1))
        {
            given.append(off);
            continue;
        }
        color[3] = rand() % 255;
        color[4] = rand() % 255;
        color[5] = rand() % 255;
        given.append(color);
    }
    return given;
}

void socketDataParsing()
{
    QByteArray data = createByteArray();
    std::deque<tlv::TLV> received = Parser::parseData(data);
    {
        ASSERT(received.size() == 3);
    }
    {
        ASSERT_EQUAL(received[0].getType(), TYPE_ON);
        ASSERT_EQUAL(received[1].getType(), TYPE_CHANGE_COLOR);
        ASSERT_EQUAL(received[2].getType(), TYPE_OFF);
    }
    {
        ASSERT_EQUAL(received[0].getLength(), 0);
        ASSERT_EQUAL(received[1].getLength(), 3);
        ASSERT_EQUAL(received[2].getLength(), 0);
    }
    {
        std::vector<unsigned char> given = {};
        ASSERT_EQUAL(received[0].getValue(), given);
    }
    {
        std::vector<unsigned char> given = {0xeb, 0x72, 0x2e};
        ASSERT_EQUAL(received[1].getValue(), given);
    }
    {
        std::vector<unsigned char> given = {};
        ASSERT_EQUAL(received[2].getValue(), given);
    }
}

void socketBigDataParsing()
{
    QByteArray data = createBigByteArray();
    std::deque<tlv::TLV> received;
    {
        LOG_DURATION("multi thread big data parsing");
        received = Parser::multiThreadParseData(data);
    }
    {
        LOG_DURATION("one thread big data parsing");
        Parser::parseData(data);
    }
    {
        ASSERT(received.size() == TLV_COUNT);
    }
    {
        ASSERT_EQUAL(received[0].getType(), TYPE_ON);
        ASSERT_EQUAL(received[1].getType(), TYPE_CHANGE_COLOR);
        ASSERT_EQUAL(received[TLV_COUNT - 1].getType(), TYPE_OFF);
    }
}

void testAll()
{
    TestRunner tr;
    RUN_TEST(tr, socketDataParsing);
    RUN_TEST(tr, socketBigDataParsing);
}
