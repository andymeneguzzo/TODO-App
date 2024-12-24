// Validator.h

#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>
#include <ctime>

class Validator {
public:
    // Validazione del formato della data/ora
    static bool validateDateTime(const std::string& dateTimeStr, std::time_t& outTime);

    // Validazione della priorità
    static bool validatePriority(int priority, int min = 1, int max = 3);
};

#endif // VALIDATOR_H
