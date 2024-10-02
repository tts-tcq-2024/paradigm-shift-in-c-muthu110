#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
 
// Function to check if a parameter is within range and return the status message
const char* getStatus(const char* parameter, float value, float minimumRange, float maximumRange, const char* lowRangeMessageindiaction, const char* highRangeMessageindiaction) {
    static char message[100];  // Static to persist the value between function calls
    if (value < minimumRange) {
        printf(message, sizeof(message), "%s%s", parameter, lowRangeMessageindiaction);
    } else if (value > maximumRange) {
        printf(message, sizeof(message), "%s%s", parameter, highRangeMessageindiaction);
    } else {
        printf(message, sizeof(message), "%s is within range.", parameter);
    }
    return message;
}
 
// Function to check if the battery parameters are within range
bool batteryIsOk(float temperature, float soc, float chargeRate) {
    bool isBatteryOk = true;
    const char* tempStatus = getStatus("Temperature", temperature, 0, 45, " is too low!", " is too high!");
    const char* socStatus = getStatus("State of Charge", soc, 20, 80, " is too low!", " is too high!");
    const char* chargeRateStatus = getStatus("Charge Rate", chargeRate, 0, 0.8f, "", " is too high!");
 
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
    assert(batteryIsOk(25, 70, 0.7f)) ;
    assert(batteryIsOk(-1, 70, 0.7f)); 
    return 0;
}
