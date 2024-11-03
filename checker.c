#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

// Function to check if a parameter is within range and return the status message
const char* getStatus(const char* parameter, float value, float minimumRange, float maximumRange, float tolerance, bool enableWarning, const char* lowRangeMessageIndication, const char* highRangeMessageIndication) {
    static char message[100];  // Static to persist the value between function calls

    // Calculate warning ranges
    float lowerWarningLimit = minimumRange + tolerance;
    float upperWarningLimit = maximumRange - tolerance;

    // Determine message based on value
    if (value < minimumRange) {
        snprintf(message, sizeof(message), "%s%s", parameter, lowRangeMessageIndication);
    } else if (value > maximumRange) {
        snprintf(message, sizeof(message), "%s%s", parameter, highRangeMessageIndication);
    } else if (enableWarning && value <= lowerWarningLimit) {
        snprintf(message, sizeof(message), "Warning: %s approaching discharge!", parameter);
    } else if (enableWarning && value >= upperWarningLimit) {
        snprintf(message, sizeof(message), "Warning: %s approaching charge-peak!", parameter);
    } else {
        snprintf(message, sizeof(message), "%s is within range.", parameter);
    }
    return message;
}

// Function to check if the battery parameters are within range
bool batteryIsOk(float temperature, float soc, float chargeRate) {
    bool isBatteryOk = true;

    // Define tolerance values for each parameter
    float temperatureTolerance = 0.05f * 45;  // 5% of max temperature
    float socTolerance = 0.05f * 80;          // 5% of max SOC
    float chargeRateTolerance = 0.05f * 0.8f; // 5% of max charge rate

    // Get status messages for each parameter, enabling warnings for all parameters initially
    const char* tempStatus = getStatus("Temperature", temperature, 0, 45, temperatureTolerance, true, " is too low!", " is too high!");
    const char* socStatus = getStatus("State of Charge", soc, 20, 80, socTolerance, true, " is too low!", " is too high!");
    const char* chargeRateStatus = getStatus("Charge Rate", chargeRate, 0, 0.8f, chargeRateTolerance, true, "", " is too high!");

    const char* statusMessages[] = { tempStatus, socStatus, chargeRateStatus };

    // Loop through each status message and print if out of range
    for (int i = 0; i < 3; i++) {
        if (!strstr(statusMessages[i], "within range")) {
            printf("%s\n", statusMessages[i]);
            isBatteryOk = false;
        }
    }
    return isBatteryOk;
}

// Main function with assertions to test the batteryIsOk function
int main() {
    // Testing different conditions
    assert(batteryIsOk(25, 70, 0.7f) == true);             // Normal case
    assert(batteryIsOk(22, 70, 0.7f) == true);             // Within SOC warning limit
    assert(batteryIsOk(25, 78, 0.7f) == true);             // Within upper SOC warning limit
    assert(batteryIsOk(25, 16, 0.7f) == false);            // SOC below range
    assert(batteryIsOk(25, 70, 0.85f) == false);           // Charge rate above max range
    assert(batteryIsOk(42, 70, 0.7f) == true);             // Within temperature warning limit
    assert(batteryIsOk(46, 70, 0.7f) == false);            // Temperature above max range

    printf("All tests passed successfully.\n");
    return 0;
}
