// Radio Modem Program
//    Send messages between RFD900x Enabled ESP32 devices.
// RULES:
//    pins 6 - 11 are reserved for SPI flash.
//    pins 34, 35, 36, 39 are input only.
// LEDs utilized for important indications.
int POWER_LED_PIN = 16;
unsigned long POWER_LED_PIN_DIM_FREQUENCY = 10;
unsigned long POWER_LED_BRIGHTNESS = 0;
unsigned long POWER_LED_LAST_UPDATE = 0;
bool POWER_LED_ANALOG_DIRECTION = true;
int TX_LED_PIN = 13;
int RX_LED_PIN = 22;

// RFD900X Variables
int RFD900X_SERIAL_RX_PIN = 16;  // pin number on the esp32 utilized to receive data from rfd900x
int RFD900X_SERIAL_TX_PIN = 17;  // pin number on the esp32 utilized to send data to the rfd900x.
int RFD900X_BAUD = 9600;         // I'll have to make sure I edit the radio initially in RFD900x Tools - Default baud is 57600
HardwareSerial RFD900X_UART(2);  // use hardware UART #2 with RFD900X.
// I guess you need to have a terminating character for messages. Huh.
char MESSAGE_CHAR_TERMINATOR = 'Z';
// RFD900X Command Strings
String EEPROM_VERSION = "ATS0";      // EEPROM Version - should not be changed. Set by firmware.
String SERIAL_SPEED = "ATS1";        // Serial speed in one byte form. Accepted values are: 1, 2, 4, 9, 19, 38, 57, 115, 230, 460
                                     // corresponding to 1200 bps, 2400 bps, 4800bps, 9600 bps, 19200bps, 38400bps, 57600bps, 115200bps,
                                     // 230400bps, 460800bps and 1000000bps respectively.
String AIR_SPEED = "ATS2";           // Air data rate in ‘one-byte form’. Accepted values are 4,64,125,250,500, 750 corresponding to 4000bps,
                                     // 64000bps, 125000bps, 25000bps,
                                     // 500000bps and 750000bps respectively
String TXPOWER = "ATS4";             // Transmit power in dBm. Maximum is 30dBm.
String ECC = "ATS5";                 // Enables or disable the golay error correcting code. When enabled it doubles the over the air data usage.
String MAVLINK = "ATS6";             // enables or disables the MAVLink framing and reporting.
String OP_RESEND = "ATS7";           // Opportunistic resend allows the node to resend packets if it has sapre bandwidth.
String MIN_FREQ = "ATS8";            // Min frequency in KHz.
String MAX_FREQ = "ATS9=";           // Max frequency in KHz.
String NUM_CHANNELS = "ATS10";       // number of frequency hopping channels. Defaults to 20. Maximum = 50. Minimum value = 1, must be same
                                     // at both ends of link.
String DUTY_CYCLE = "ATS11";         // The percentage of time to allow transmit. Default = 100, Maximum = 100, minimum = 10. Must be same
                                     // at both ends of link.
String RTSCTS = "ATS13";             // Ready to send and clear to send flow control. Default: 0, maximum: 1, minimum: 0, doesn't have to be the
                                     // same at each end of radio link.
String MAX_WINDOW = "ATS14";         // Max transit window size used to limit max time / lateny if required otherwise will be set automatically
String ENCRYPTION_LEVEL = "ATS15";   // encryption level. 0 = off, 1 = 128 bit AES.
String RC_INPUT_GPIO_1 = "ATS16";    // Set GPIO 1.1 as R/C (PPM) input. Default = 0, maximum = 1, minimum = 0.
String RC_OUTPUT_GPIO_1 = "ATS17";   // Set GPIO 1.1 as RC (PPM) output. Defautl 0, maximum = 1, minimum = 0.
String ANT_MODE = "ATS18";           // 0 = diversity
                                     // 1 = Antenna 1 only
                                     // 2 = antenna 2 only
                                     // 3 = antenna 1 TX and antenna 2 RX
String PKT_DROP_RSSI = "ATS19=";     // Sets a RSSI threshold below which the packet will be discarded. 0 disables the feature.
                                     // 0 default, maximum = 255, minimum = 0.
String RADIO_SAVE = "AT&W";          // Save a newly changed register to the radio.
String RADIO_REBOOT = "ATZ";         // Reboot the radio for new changes to take effect.
String ENTER_COMMAND_MODE = "+++";   // Orders the radio to enter command mode. You must wait 1000ms to ensure the radio properly
                                     // enters command mode.
String EXIT_COMMAND_MODE = "ATO";    // Orders Radio to exit command mode.
int COMMAND_MODE_DELAY_TIME = 1000;  // Amount of time in milliseconds to wait after the "ENTER_COMAND_MODE" string has been
                                     // sent to the radio.
// these RFD900X terminal commands have a letter I inserted into them for some reason. They are in a different table.
String RADIO_VERSION = "ATI";       // shows radio version.
String BOARD_TYPE = "ATI2";         // shows the board type
String BOARD_FREQUENCY = "ATI3";    // shows board frequency
String BOARD_VERSION = "ATI4";      // shows the board's version.
String TDM_TIMING_REPORT = "ATI6";  // Displays TDM timing report.
String RSSI_REPORT = "ATI7";        // Displays RSSI signal report.
String RADIO_ID = "ATI8";           // Displays device 64-bit unique ID.
String FACTORY_RESET = "AT&F";      // Resets all parameters to factory defaults.

// Button Variables
unsigned long BUTTON_SAMPLE_SPEED = 100;         // Sample buttons every this many milliseconds.
unsigned long LAST_BUTTON_SAMPLE_TIMESTAMP = 0;  // Controls when we sample button digital pins.
int OK_BUTTON_PIN = 34;
bool OK_BUTTON_STATE = false;  // pin number on esp32 utilized to sense an OK button.
unsigned long LAST_OK_BUTTON_PRESS = 0;
int TX_BUTTON = 35;  // Utilized to transmit the message.
bool TX_BUTTON_STATE = false;
unsigned long LAST_TX_BUTTON_PRESS = 0;
int SLEEP_BUTTON = 12;  // pin number on esp32 utilized to put the radio to sleep.
int RIGHT_ARROW_KEY = 39;
bool RIGHT_ARROW_KEY_STATE = false;
unsigned long LAST_RIGHT_ARROW_KEY_PRESS = 0;
int LEFT_ARROW_KEY = 22;
bool LEFT_ARROW_KEY_STATE = false;
unsigned long LAST_LEFT_ARROW_KEY_PRESS = 0;
unsigned long BUTTON_COOLDOWN_TIME = 100;

// Deep Sleep Variables
#include "driver/rtc_io.h"
#define BUTTON_PIN_BITMASK(GPIO) (1ULL << GPIO)  // 2 ^ GPIO_NUMBER in hex
#define USE_EXT0_WAKEUP 1                        // 1 = EXT0 wakeup, 0 = EXT1 wakeup
#define WAKEUP_GPIO GPIO_NUM_33                  // Only RTC IO are allowed - ESP32 Pin example

// TFT screen stuff.
#include <SPI.h>
#include <TFT_eSPI.h>       // Hardware-specific library
TFT_eSPI tft = TFT_eSPI();  // Invoke custom library

// variables for the message writing screen.
// +==============================================================================================================+
// |                                            W I R I N G
// |  ESP32
// |

// Buzzer Variables
// I chose to use this website, which has the octaves and frequencies of each note!
// https://mixbutton.com/music-tools/frequency-and-pitch/music-note-to-frequency-chart#1st
// I chose the frequencies of each note for octave 4, which seemed to be the center of the chart.
int BUZZER_PIN = 14;
unsigned long BUTTON_SOUND_LENGTH = 100;
unsigned int A = 440;
unsigned int B = 493;
unsigned int C = 261;
unsigned int D = 293;
unsigned int E = 329;
unsigned int F = 349;
unsigned int G = 392;

// a test for a button class.
#include "SelectableBox.h"
SelectableBox myBox = SelectableBox(0, 200, 100, 100, "Title", "This is the content. ", false, tft);
void setup() {
  coolStartupScreen();
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE);
  tft.println("Power on.");
  tft.println("Setting up buttons.");
  // Initialize communication with the RFD900X on UART 2
  RFD900X_UART.begin(RFD900X_BAUD, SERIAL_8N1, RFD900X_SERIAL_RX_PIN, RFD900X_SERIAL_TX_PIN);
  // Set up pins
  tft.println("Configuring OK button as input");
  pinMode(OK_BUTTON_PIN, INPUT_PULLDOWN);  // OK button pin
  tft.println("Configuring TX button as input.");
  pinMode(TX_BUTTON, INPUT_PULLDOWN);  // TX button pin
  Serial.begin(9600);
  if (RFD900X_UART) {
    tft.println("Hardware UART2 functioning.");
  } else {
    tft.println("Hardware UART2 not functioning!");
  }
  pinMode(POWER_LED_PIN, OUTPUT);
  tft.println("Configuring input sleep button as input pulldown.");
  pinMode(SLEEP_BUTTON, INPUT_PULLDOWN);

  tft.print("Right arrow key attached to pin ");
  tft.print(RIGHT_ARROW_KEY);
  tft.println();
  pinMode(RIGHT_ARROW_KEY, INPUT);
  tft.print("Left arrow key attached to pin ");
  tft.print(LEFT_ARROW_KEY);
  tft.println();
  pinMode(LEFT_ARROW_KEY, INPUT);
  tft.println("Setup Complete.");
}
void loop() {
  tft.setCursor(0, 0);
  // Pressing the sleep button puts the radio to sleep.
  if (digitalRead(SLEEP_BUTTON) == HIGH) {
    DEEP_SLEEP();
  }
  updatePowerLED(POWER_LED_PIN);
  if (millis() - LAST_BUTTON_SAMPLE_TIMESTAMP > BUTTON_SAMPLE_SPEED) {
    LAST_BUTTON_SAMPLE_TIMESTAMP = millis();
    processButtonPresses();
  }
  myBox.update();
  if (OK_BUTTON_STATE) {
    myBox.flip();
  }
  if(RIGHT_ARROW_KEY_STATE) {
    myBox.hoveredOver=true; 
  } else {
    myBox.hoveredOver = false; 
  }
}
// utilized to pulse the power LED to indicate the radio's state.
void updatePowerLED(int POWER_LED_PIN) {
  if (millis() - POWER_LED_LAST_UPDATE > POWER_LED_PIN_DIM_FREQUENCY) {
    if (POWER_LED_ANALOG_DIRECTION) {
      // if true, increase pwm value. Otherwise, decrease it.
      POWER_LED_BRIGHTNESS += 1;
    } else {
      POWER_LED_BRIGHTNESS -= 1;
    }
    // if the analog value goes above 254, change the boolean which changes whether
    // we increase or decreaes the analog value we write to the pin.
    if (POWER_LED_BRIGHTNESS > 254) {
      POWER_LED_ANALOG_DIRECTION = false;
    }
    if (POWER_LED_BRIGHTNESS < 1) {
      POWER_LED_ANALOG_DIRECTION = true;
    }
    // update our last power LED update time.
    POWER_LED_LAST_UPDATE = millis();
  }
  analogWrite(POWER_LED_PIN, POWER_LED_BRIGHTNESS);
}

// Pulls files from the SD card and applies settings to the radio.
void LOAD_CONFIG() {
}

void DEEP_SLEEP() {
  esp_sleep_enable_ext0_wakeup(WAKEUP_GPIO, 1);  //1 = High, 0 = Low
  // Configure pullup/downs via RTCIO to tie wakeup pins to inactive level during deepsleep.
  // EXT0 resides in the same power domain (RTC_PERIPH) as the RTC IO pullup/downs.
  // No need to keep that power domain explicitly, unlike EXT1.
  rtc_gpio_pullup_dis(WAKEUP_GPIO);
  rtc_gpio_pulldown_en(WAKEUP_GPIO);
  //Go to sleep now
  tft.println("Going to sleep in 5 seconds ");
  unsigned long interval = 1000;
  unsigned long previousMillis = 0;
  unsigned long currentMillis = 0;
  bool waiting = true;
  int counter = 5;
  while (waiting) {
    currentMillis = millis();
    if (currentMillis - previousMillis > interval) {
      tft.print(counter);
      tft.print("s");
      tft.println();
      previousMillis = currentMillis;
      counter--;
      if (counter < 0) {
        waiting = false;
      }
    }
  }
  esp_deep_sleep_start();
}

void coolStartupScreen() {
  // first, initialize communication with the TFT screen.
  tft.init();
  tft.setRotation(2);
  tft.fillScreen(TFT_BLACK);
  unsigned long animationDuration = 2000;
  bool continueAnimation = true;
  unsigned long interval = 1;
  unsigned long previousMillis = 0;
  unsigned long currentMillis = 0;
  double x = 0;
  double y = 0;
  double counter = 0;
  bool backwards = false;
  double limit = 5;
  double stepIncrement = 0.05;
  while (continueAnimation) {
    currentMillis = millis();
    // determine if the animation has shown long enough to end:
    if (millis() > animationDuration) {
      continueAnimation = false;
    } else {
      if (currentMillis - previousMillis >= interval) {
        counter = 0;
        y = 0;
        backwards = false;
        int increment = random(1, 35);
        for (int i = 0; i < 480; i++) {
          if (!backwards) {
            counter += stepIncrement;
          } else {
            counter -= stepIncrement;
          }

          if (counter > limit) {
            backwards = true;
          }
          x = sin(counter) * random(1, 150);
          tft.drawPixel(x + (320 / 2), y, TFT_WHITE);
          previousMillis = currentMillis;
          y += increment;
        }
      }
    }
  }


  // clear the startup screen.
  tft.fillScreen(TFT_BLACK);
}

// Modify a radio setting.
// NOTE: this function assumes you are already in command mode.
// Remember that the String VALUE is usually a number. Cast to a string when using this command.
void RFD900X_CHANGE_SETTING(String RADIO_COMMAND, String VALUE) {
  RFD900X_UART.print(RADIO_COMMAND + "=" + VALUE);
  RFD900X_UART.print(RADIO_SAVE);
}
// Puts the radio into a mdoe where you can send AT commands.
void RFD900X_ENTER_COMMAND_MODE() {
  RFD900X_UART.print(ENTER_COMMAND_MODE);
  delay(1000);
}
// Exit command mode
void RFD900X_EXIT_COMMAND_MODE() {
  RFD900X_UART.print(EXIT_COMMAND_MODE);
}
// If you want to query a setting on the radio, use this function.
// NOTE: this function assumes you are already in command mode.
String RFD900X_SETTING_QUERY(String SETTING_NAME) {
  RFD900X_UART.print(SETTING_NAME + "?");
  int incomingByte;
  String result = "";
  while (RFD900X_UART.available() > 0) {
    incomingByte = RFD900X_UART.read();
    result += incomingByte;
  }
  return result;
}
// Rebooting the radio enables the settings to take effect that you've changed.
// NOTE: this function assumes you are already in command mode.
void RFD900X_REBOOT() {
  RFD900X_UART.print(RADIO_REBOOT);
}
void RFD900X_TRANSMIT_STRING(String message) {
  RFD900X_UART.print(message);
}

// Button Variables
// Here, we process inputs and act accordingly. Hopefully it's not a terrible idea . . . (Because different screens
// will use the same button for different purposes . .. . . )
void processButtonPresses() {
  if (millis() - LAST_OK_BUTTON_PRESS > BUTTON_COOLDOWN_TIME) {
    if (digitalRead(OK_BUTTON_PIN)) {
      Serial.println("OK BUTTON");
      LAST_OK_BUTTON_PRESS = millis();
      tone(BUZZER_PIN, A, BUTTON_SOUND_LENGTH);
      OK_BUTTON_STATE = true;
    } else {
      OK_BUTTON_STATE = false;
    }
  }

  if (millis() - LAST_RIGHT_ARROW_KEY_PRESS > BUTTON_COOLDOWN_TIME) {
    if (digitalRead(RIGHT_ARROW_KEY)) {
      Serial.println("RIGHT ARROW KEY ");
      LAST_RIGHT_ARROW_KEY_PRESS = millis();
      tone(BUZZER_PIN, B, BUTTON_SOUND_LENGTH);
      RIGHT_ARROW_KEY_STATE = true;
    } else {
      RIGHT_ARROW_KEY_STATE = false;
    }
  }

  if (millis() - LAST_LEFT_ARROW_KEY_PRESS > BUTTON_COOLDOWN_TIME) {
    if (digitalRead(LEFT_ARROW_KEY)) {
      Serial.println("LEFT ARROW KEY");
      tone(BUZZER_PIN, C, BUTTON_SOUND_LENGTH);
      LAST_LEFT_ARROW_KEY_PRESS = millis();
      LEFT_ARROW_KEY_STATE = true;
    } else {
      LEFT_ARROW_KEY_STATE = false;
    }
  }
}