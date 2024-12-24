// Validator.cpp

#include "Validator.h"
#include <sstream>
#include <iomanip>

bool Validator::validateDateTime(const std::string& dateTimeStr, std::time_t& outTime) {
    // Formato atteso: YYYY-MM-DD HH:MM
    std::tm tm = {};
    std::istringstream ss(dateTimeStr);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M");
    if (ss.fail()) {
        return false;
    }
    outTime = std::mktime(&tm);
    return true;
}

bool Validator::validatePriority(int priority, int min, int max) {
    return (priority >= min && priority <= max);
}
