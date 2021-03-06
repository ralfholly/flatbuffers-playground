#include "current_date_time_generated.h"

#include <cstring>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <functional>
#include <chrono>

using namespace std;
using namespace protobuftest;

#define TIMEIT(code) timeit([&]() { code; })

inline double steadyClockDelta(std::chrono::time_point<std::chrono::steady_clock> start, std::chrono::time_point<std::chrono::steady_clock> end)
{
    using steady_clock = std::chrono::steady_clock;
    return (end - start).count() * steady_clock::period::num / static_cast<double>(steady_clock::period::den);
}

inline double timeit(const std::function<void()>& lambda) {
    using steady_clock = std::chrono::steady_clock;
    auto start = steady_clock::now();
    lambda();
    auto end = steady_clock::now();
    return steadyClockDelta(start, end);
}

void buildMessage(flatbuffers::FlatBufferBuilder& builder) {
    volatile static int m_year = 2000;
    // Bottom-up creation is required.
    auto weekday = builder.CreateString("Monday");
    DateBuilder dateBuilder(builder);
    dateBuilder.add_day(11);
    dateBuilder.add_month(9);
    dateBuilder.add_year(m_year++);
    dateBuilder.add_weekday(weekday);
    auto date = dateBuilder.Finish();
    TimeBuilder timeBuilder(builder);
    timeBuilder.add_hours(1);
    timeBuilder.add_minutes(2);
    timeBuilder.add_seconds(3);
    auto time = timeBuilder.Finish();
    auto title = builder.CreateString("A simple test");
    CurrentDateTimeMessageBuilder currentDateTimeMessageBuilder(builder);
    currentDateTimeMessageBuilder.add_title(title);
    currentDateTimeMessageBuilder.add_counter(42);
    currentDateTimeMessageBuilder.add_date(date);
    currentDateTimeMessageBuilder.add_time(time);
    auto currentDateTimeMessage = currentDateTimeMessageBuilder.Finish();
    builder.Finish(currentDateTimeMessage);
}

bool storeMessage(flatbuffers::FlatBufferBuilder& builder, const char* fname) {
    ofstream serializingStream(fname, ofstream::out | ofstream::binary);
    serializingStream.write(reinterpret_cast<char*>(builder.GetBufferPointer()), builder.GetSize());
    return serializingStream.good();
}

bool loadMessage(vector<char>& buffer, const CurrentDateTimeMessage** msg, const char* fname) {
    std::ifstream file(fname, std::ios::binary);
    std::streampos fileSize;
    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    buffer.resize(fileSize);
    file.read(buffer.data(), fileSize);
    const CurrentDateTimeMessage* currentDateTimeMessage = flatbuffers::GetRoot<CurrentDateTimeMessage>(buffer.data());
    *msg = currentDateTimeMessage;
    return currentDateTimeMessage != nullptr;
}

void printMessage(const CurrentDateTimeMessage* msg) {
    cout << "Received a message:" << endl;
    cout << "\tTitle: " << msg->title()->c_str() << endl;
    cout << "\tCounter: " << msg->counter() << endl;
    if (msg->date() != nullptr) {
        const Date* date = msg->date();
        cout << "\t" << date->weekday()->c_str() << ", " << date->year() << "-" <<
            setw(2) << setfill('0') << date->month() << "-" <<
            setw(2) << setfill('0') << date->day() << endl;
    } else {
        cout << "No date element found!" << endl;
    }
    if (msg->time() != nullptr) {
        const Time* time = msg->time();
        cout << "\t" << setw(2) << setfill('0') << time->hours() << ":" <<
            setw(2) << setfill('0') << time->minutes() << ":" <<
            setw(2) << setfill('0') << time->seconds() << endl;
    } else {
        cout << "No time element found!" << endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Please provide a filename!" << endl;
        exit(1);
    }
    const char* fname = argv[1];

    ifstream f(fname);
    if (!f.good()) {
        cout << "Creating current date/time entry" << endl;
        flatbuffers::FlatBufferBuilder builder(1024);
        buildMessage(builder);
        storeMessage(builder, fname);
    }

    const CurrentDateTimeMessage* msg2;
    vector<char> buffer;
    if (loadMessage(buffer, &msg2, fname)) {
        printMessage(msg2);
    } else {
        cerr << "Failed to load message!" << endl;
    }

    constexpr size_t reps = 10'000'000;
    double runtime = TIMEIT(
        for (size_t i = 0; i < reps; ++i) {
            flatbuffers::FlatBufferBuilder builder(1024);
            buildMessage(builder);
        }
    );
    cout << "Runtime: " << runtime << endl;
    cout << "Average message build time: " << 1'000'000 * runtime / reps << " usec" << endl;

    return 0;
}
