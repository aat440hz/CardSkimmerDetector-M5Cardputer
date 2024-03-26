# CardSkimmerDetector

CardSkimmerDetector is an innovative, open-source tool designed to assist law enforcement officers and security experts in detecting and mitigating the threats posed by Bluetooth Low Energy (BLE) skimming devices. Built on the versatile M5Cardputer platform, this device is engineered to identify potential skimmers through their unique device names and MAC address prefixes, offering a proactive defense mechanism against digital theft.

## Features

- **Advanced BLE Scanning:** Continuously scans for and identifies nearby BLE devices, leveraging sophisticated algorithms to distinguish potential threats from harmless devices.
- **Skimmer Identification:** Uses a database of known skimmer module names and MAC address patterns to accurately detect skimming devices.
- **Immediate Alerts:** Provides real-time alerts upon the detection of a skimmer, displaying essential information about the threat on the M5Cardputer screen for prompt action.
- **Manual Scan Control:** Allows users to manually initiate scans, offering enhanced security and flexibility in monitoring for skimming devices.
- **Highly Customizable:** Offers extensive customization options, including the ability to adjust scan durations, the number of devices displayed, and the detection history length, making the tool adaptable to various operational needs.

## Usage

1. **Library Installation:** Start by installing the necessary libraries detailed in the Arduino sketch to ensure the device operates effectively.
2. **Sketch Upload:** Load the provided sketch onto the M5Cardputer to equip it with the skimming detection capabilities.
3. **Device Activation:** Power on the device to begin automatic scanning for BLE devices. The device will continuously scan and analyze nearby Bluetooth signals.
4. **Skimmer Alert:** If a known skimmer module is detected, the device will immediately issue an alert and present the relevant details on the display.
5. **Alert Clearance:** Press the Enter key on the connected keyboard to clear any active alerts and reset the device to its standard scanning mode.
6. **Initiate Scans:** Utilize the Button A on the M5Cardputer to manually trigger a new scanning cycle, increasing vigilance and responsiveness to potential threats.

## Known Skimmer Module Names

This tool comes pre-loaded with a list of widely recognized skimmer module names. Users are encouraged to periodically update this list to include new skimmers as they emerge. Alter the `knownSkimmerNames` array within the sketch to enhance the device's detection capabilities.

## Customization

Designed for versatility, the CardSkimmerDetector allows users to modify its operation via several parameters within the sketch. Tailor the scan time, display options, and device history to meet your unique operational needs and limitations.

## Disclaimer

This tool is intended solely for educational purposes and to aid law enforcement and security professionals in identifying BLE skimming devices. Its use should comply with all relevant laws and guidelines. While aiming for precise detection, users should be cognizant of the possibility of false positives and understand that it cannot guarantee the identification of all skimming devices.

## Contributions

Contributions from the community to improve the CardSkimmerDetector's functionality and detection range are highly welcomed. Feel free to fork the repository, apply your enhancements, and submit pull requests to share your advancements with the broader community.

## License

The CardSkimmerDetector project is made available under the MIT License, encouraging open-source collaboration and the exchange of knowledge and technology in combating digital skimming threats.
