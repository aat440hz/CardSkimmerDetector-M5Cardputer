#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <regex>

// Undefine min and max macros to avoid conflicts
#undef min
#undef max

#include <M5Cardputer.h>

int scanTime = 10; // Duration for a BLE scan
String knownSkimmerNames[] = {"HC-03", "HC-05", "HC-06", "HC-08", "RNBT"}; // Known skimmer module names
std::vector<String> detectedDevices; // List to hold detected devices
std::vector<String> detectedSkimmers; // List to hold detected skimmers
int maxDevicesDisplayed = 12; // Adjust based on your display size
int maxDevicesStored = 12; // Adjust based on desired history length and memory constraints
// Global variables to track the debounce state
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

// Function prototypes
void updateDisplay();
void displaySkimmers();

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        String deviceInfo = "Name: " + String(advertisedDevice.getName().c_str()) +
                            ", Addr: " + String(advertisedDevice.getAddress().toString().c_str());
        
        // Regular expression to match "RNBT-xxxx" pattern (xxxx being any alphanumeric characters)
        std::regex rnbtPattern("RNBT-[A-Za-z0-9]{4}");

        // Check for RN modules with the prefix 00:06:66 in the MAC address
        String macPrefix = "00:06:66";
        String deviceAddress = String(advertisedDevice.getAddress().toString().c_str());
        bool isRnSkimmer = deviceAddress.startsWith(macPrefix);

        // Assume MAC format is YY:YY:MM:DD:XX:XX for manufacturing date check
        int year1 = (deviceAddress.charAt(0) - '0') * 10 + (deviceAddress.charAt(1) - '0');
        int year2 = (deviceAddress.charAt(3) - '0') * 10 + (deviceAddress.charAt(4) - '0');
        int month = (deviceAddress.charAt(6) - '0') * 10 + (deviceAddress.charAt(7) - '0');
        int day = (deviceAddress.charAt(9) - '0') * 10 + (deviceAddress.charAt(10) - '0');
        int year = year1 * 100 + year2;

        // Update to the current year as the upper limit for year check
        int currentYear = 2025; // Replace 20XX with the current year or fetch dynamically

        // Convert the device name to std::string for regex matching
        std::string devName = advertisedDevice.getName();

        // Check if device name matches any known skimmer names or the RNBT pattern
        bool isSkimmer = false;
        for (String skimmerName : knownSkimmerNames) {
            if (devName.find(skimmerName.c_str()) != std::string::npos || 
                std::regex_search(devName, rnbtPattern) || 
                isRnSkimmer || 
                (year >= 2013 && year <= currentYear && month >= 1 && month <= 12 && day >= 1 && day <= 31)) {
                detectedSkimmers.push_back(deviceInfo); // Add to skimmer list
                isSkimmer = true;
                displaySkimmers(); // Display only skimmers
                return; // Exit early to avoid overwriting the display with normal devices
            }
        }

        // If not a skimmer, add to general device list
        if (!isSkimmer) {
            if (detectedDevices.size() >= maxDevicesStored) {
                detectedDevices.erase(detectedDevices.begin());
            }
            detectedDevices.push_back(deviceInfo); // Add to general device list
            updateDisplay(); // Update display with all detected devices
        }
    }
};

void setup() {
    M5Cardputer.begin();
    Serial.begin(115200);
    BLEDevice::init("");

    // Rotate the M5Cardputer screen by 180 degrees
    M5Cardputer.Lcd.setRotation(1); // Assuming 2 is the correct value for 180-degree rotation; adjust if necessary

    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true);
    pBLEScan->start(scanTime, true);
}

void loop() {
    M5Cardputer.update(); // Update M5Cardputer status

    // Handling Button A press for scanning
    if (M5Cardputer.BtnA.wasPressed()) {
        detectedDevices.clear(); // Optionally clear previous devices
        detectedSkimmers.clear(); // Clear previous skimmers
        BLEScan* pBLEScan = BLEDevice::getScan();
        pBLEScan->clearResults(); // Clear previous scan results
        pBLEScan->start(scanTime, true); // Start a new scan
    }

    // Keyboard input handling for Enter key as BtnB
    if (M5Cardputer.Keyboard.isChange()) {
        unsigned long currentMillis = millis();

        if (M5Cardputer.Keyboard.isPressed()) {
            Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();

            if (status.enter && (currentMillis - lastDebounceTime) > debounceDelay) {
                // Perform the action originally assigned to BtnB
                // For instance, clearing skimmers or updating the display can go here
                detectedSkimmers.clear(); // Clear the list of detected skimmers
                updateDisplay(); // Update display to reflect changes
                
                lastDebounceTime = currentMillis; // Reset the debouncing timer
            }
        }
    }
}

void updateDisplay() {
    M5Cardputer.Lcd.fillScreen(BLACK);
    M5Cardputer.Lcd.setCursor(0, 0);
    for (int i = std::max(0, (int)detectedDevices.size() - maxDevicesDisplayed); i < detectedDevices.size(); ++i) {
        M5Cardputer.Lcd.println(detectedDevices[i]);
    }
    M5Cardputer.Lcd.setCursor(0, M5Cardputer.Lcd.height() - 16);
    M5Cardputer.Lcd.println("Press A to scan");
}

void displaySkimmers() {
    M5Cardputer.Lcd.fillScreen(RED);
    M5Cardputer.Lcd.setCursor(0, 0);
    M5Cardputer.Lcd.println("Alert: Skimmer Detected!");
    for (String& skimmer : detectedSkimmers) {
        M5Cardputer.Lcd.println(skimmer);
    }

    // Wait for the Enter key to be pressed to clear the display
    bool enterPressed = false;
    while (!enterPressed) {
        M5Cardputer.update();
        if (M5Cardputer.Keyboard.isChange()) {
            if (M5Cardputer.Keyboard.isPressed()) {
                Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();
                if (status.enter) {
                    enterPressed = true; // Exit the loop when Enter is pressed
                }
            }
        }
        delay(100); // Small delay to prevent excessive CPU usage
    }

    detectedSkimmers.clear();
    updateDisplay(); // Return to normal display
}
