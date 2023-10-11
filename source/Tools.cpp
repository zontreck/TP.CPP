#include "Tools.h"
#include "ZHash.h"

#include <string>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <chrono>

#include <ctime>



using namespace std;


std::string Tools::ZHX(const std::string &input){
    ZHash hsh;
    hsh.NewKey();
    hsh.CalculateKey(input);

    return hsh.ToString();
}


std::chrono::seconds Tools::DecodeTimeNotation(const std::string& TimeStr) {
    // Define regex patterns to match numeric values followed by time units (w, d, h, m, s).
    std::regex pattern("([0-9]+(\\.[0-9]+)?)\\s*([wdhms])");
    std::smatch match;

    // Initialize the total duration to zero seconds.
    std::chrono::seconds totalDuration(0);

    // Search for matches in the input string.
    std::string remaining = TimeStr;
    while (std::regex_search(remaining, match, pattern)) {
        // Extract the numeric value and time unit.
        double value = std::stod(match[1]);
        char unit = match[3].str()[0];

        // Convert the value to seconds based on the time unit and add it to the total duration.
        switch (unit) {
            case 'w':
                totalDuration += std::chrono::duration_cast<std::chrono::seconds>(std::chrono::duration<double>(value * 7 * 24 * 3600));
                break;
            case 'd':
                totalDuration += std::chrono::duration_cast<std::chrono::seconds>(std::chrono::duration<double>(value * 24 * 3600));
                break;
            case 'h':
                totalDuration += std::chrono::duration_cast<std::chrono::seconds>(std::chrono::duration<double>(value * 3600));
                break;
            case 'm':
                totalDuration += std::chrono::duration_cast<std::chrono::seconds>(std::chrono::duration<double>(value * 60));
                break;
            case 's':
                totalDuration += std::chrono::duration_cast<std::chrono::seconds>(std::chrono::duration<double>(value));
                break;
        }

        // Update the remaining string to continue searching for matches.
        remaining = match.suffix();
    }

    return totalDuration;
}

std::string Tools::EncodeTimeNotation(int ts) {
    const int ONE_DAY = 60 * 60 * 24;

    int Days = ts / ONE_DAY;
    ts -= (ONE_DAY * Days);
    int Hours = ts / 3600;
    ts -= (Hours * 3600);
    int Minutes = ts / 60;
    ts -= (Minutes * 60);
    int Weeks = Days / 7;
    Days -= Weeks * 7;

    stringstream ss;

    if (Weeks > 0) {
        ss << to_string(Weeks) << "w";
    }
    if (Days > 0) {
        ss << to_string(Days) << "d";
    }
    if (Hours > 0) {
        ss << to_string(Hours) << "h";
    }
    if (Minutes > 0) {
        ss << to_string(Minutes) << "m";
    }
    if (ts > 0) {
        ss << to_string(ts) << "s";
    }

    return ss.str();
}


std::chrono::seconds Tools::GetCurrentUnixTimestamp() {
    // Get the current time point using std::chrono.
    auto now = std::chrono::system_clock::now();

    // Convert the time point to a std::time_t object (Unix timestamp).
    std::time_t timestamp = std::chrono::system_clock::to_time_t(now);

    return std::chrono::seconds(timestamp);
}

std::string Tools::EncodeTimeNotation(std::chrono::seconds t)
{
    return EncodeTimeNotation(t.count());
}