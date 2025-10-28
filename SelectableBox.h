#ifndef SelectableBox_h
#define SelectableBox_h
// TFT screen stuff.
#include <SPI.h>
#include <TFT_eSPI.h>  // Hardware-specific library
// Remember to update the box in a loop. Otherwise, it's background won't update if it's pressed. It also won't have the 
// corner 4 pixels blink when you hover over it. 
// to make a button interact with it, utilize a physcial button's state (linked to digitalRead somewhere). 
// the selectable box can be set to call its flip() method whenever an event happens. 
class SelectableBox {
public:
  int x;
  int y;
  int width;
  int height;
  String title;
  String content;
  bool selected;
  unsigned int backgroundColor = 0;
  unsigned int textColor = 16777215;
  unsigned long lastPress = 0;
  unsigned long coolDownTime = 100;
  bool toggled = false;
  bool initialDrawing = true;
  bool hoveredOver = false;
  unsigned long lastCornerBlink = 0;
  unsigned long cornerBlinkFrequency = 500;
  bool cornerPixelBlackOrWhite = false;
  SelectableBox(int x, int y, int width, int height, String title, String content, bool selected, TFT_eSPI tft) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->title = title;
    this->content = content;
    this->selected = selected;
  };
  void update() {
    if (initialDrawing) {
      tft.drawRect(this->x, this->y, this->width, this->height, TFT_WHITE);
      tft.fillRect(this->x + 1, this->y + 1, this->width - 2, this->height - 2, this->backgroundColor);
      tft.setCursor(this->x, this->y);
      tft.setTextColor(this->textColor);
      tft.println(this->title);
      tft.println(this->content);
      initialDrawing = false;
    }
    if(hoveredOver) {
      blinkCornerPixel(); 
    }
    if (toggled) {
      // if the box is selected, change the text to black, and the box background to white.
      if (this->selected) {
        this->backgroundColor = 16777215;
        this->textColor = 0;
      } else {
        this->backgroundColor = 0;
        this->textColor = 16777215;
      }
      tft.drawRect(this->x, this->y, this->width, this->height, TFT_WHITE);
      tft.fillRect(this->x + 1, this->y + 1, this->width - 2, this->height - 2, this->backgroundColor);
      tft.setCursor(this->x, this->y);
      tft.setTextColor(this->textColor);
      tft.println(this->title);
      tft.println(this->content);
      this->toggled = false;
    }
  }
  void flip() {
    if (millis() - this->lastPress > this->coolDownTime) {
      this->toggled = true;
      this->lastPress = millis();
      this->selected = !this->selected;
    }
  }
  void blinkCornerPixel() {
    if (millis() - this->lastCornerBlink > this->cornerBlinkFrequency) {
      if (cornerPixelBlackOrWhite) {
        tft.fillRect(this->x+this->width-4,this->y,4,4,TFT_WHITE); 
      } else {
        tft.fillRect(this->x+this->width-4,this->y,4,4,TFT_BLACK);
      }
      lastCornerBlink = millis(); 
      cornerPixelBlackOrWhite = !cornerPixelBlackOrWhite; 
    }
  }
};
#endif