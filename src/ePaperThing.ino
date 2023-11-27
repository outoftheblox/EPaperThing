#include <Thing.h>
#include <BlinkPattern.h>
#include <LOLIN_EPD.h>
#include <Adafruit_GFX.h>

using namespace ootb;

Thing thing;
BlinkPattern led(BUILTIN_LED);


/*D1 mini*/
#define EPD_CS D0
#define EPD_DC D8
#define EPD_RST -1  // can set to -1 and share with microcontroller Reset!
#define EPD_BUSY -1 // can set to -1 to not use a pin (will wait a fixed delay)

/*D32 Pro*/
// #define EPD_CS 14
// #define EPD_DC 27
// #define EPD_RST 33  // can set to -1 and share with microcontroller Reset!
// #define EPD_BUSY -1 // can set to -1 to not use a pin (will wait a fixed delay)

LOLIN_IL3897 EPD(250, 122, EPD_DC, EPD_RST, EPD_CS, EPD_BUSY); //hardware SPI

// #define EPD_MOSI D7
// #define EPD_CLK D5
// LOLIN_SSD1680 EPD(250,122, EPD_MOSI, EPD_CLK, EPD_DC, EPD_RST, EPD_CS, EPD_BUSY); //IO

BlinkPattern::Pattern<2> initialize{{1,1},50};
BlinkPattern::Pattern<2> on{{39,1},25};

uint8_t lineCount = 0;

void setupEpaper()
{
    EPD.begin();
    EPD.setRotation(0);
    EPD.clearBuffer();
    EPD.setTextSize(1);
    EPD.setTextColor(EPD_BLACK);
    EPD.setCursor(0,0);
    //EPD.println("hello world! Welcome to wemos.cc, this is test for long text.");
    //Serial.println("hello world! Welcome to wemos.cc, this is test for long text.");
    //EPD.display();
    //EPD.deepSleep();
}

void setup()
{
    Serial.begin(230400);
    Serial.println();
    Serial.println("ClientID:" + thing.clientId());

    setupEpaper();

    led.setPattern(initialize);

    showUpdate(thing.clientId());

    thing.onStateChange([](const String& msg)
    {
        showUpdate(msg);
        Serial.println(msg);
    });

    thing.begin();
    thing.setClientBufferSize(1024);

    String topic = "things/" + thing.clientId() + "/oled/text";
    thing.addActuator(topic, [](Value& value)
    {
        showText((String)value);
        Serial.println((String)value);
    });

    led.setPattern(on);
}

void loop()
{
    thing.handle();
}

void showUpdate(const String& value)
{
    if(lineCount > 15) lineCount = 0;
    uint8_t y = 8 * lineCount++; 
    EPD.setCursor(0, y);
    EPD.setTextColor(EPD_BLACK);
    EPD.setTextSize(1);
    EPD.println(value);
    EPD.display();
    //EPD.deepSleep();
}

void showText(const String& value)
{
    EPD.clearBuffer();
    EPD.setCursor(0,0);
    EPD.setTextColor(EPD_BLACK);
    EPD.setTextSize(1);
    EPD.println(value);
    EPD.display();
    //EPD.deepSleep();
}
