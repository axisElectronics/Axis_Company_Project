
# **Setup ESP32 DEVKIT with new 280x320 ILI9341 Driver based TFT with TFT_eSPI library.**

Few Chnages into below files are required to work with this library.

1. After installing TFT_eSPI library.
2. **Open Documets -> Arduino -> Arduino -> library -> TFT_eSPI -> User_Setup.h**
3. uncomment your required TFT driver from given list.
   ```
   #define ILI9341_DRIVER 
   ```
4. Search ESP32 Dev Board where set your used SPI and control pins. Here we are using

```
// ###### EDIT THE PIN NUMBERS IN THE LINES FOLLOWING TO SUIT YOUR ESP32 SETUP   ######

// For ESP32 Dev board (only tested with ILI9341 display)
// The hardware SPI can be mapped to any pins

#define TFT_MISO 19
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   05  // Chip select control pin
#define TFT_DC   25  // Data Command control pin
//#define TFT_RST   2  // Reset pin (could connect to RST pin)
#define TFT_RST  -1  // Set TFT_RST to -1 if display RESET is connected to ESP32 board RST

#define TOUCH_CS 26     // Chip select pin (T_CS) of touch screen
```
5. Setup SPI speed. Please read Document carefully. It is recommended that ILI9341 support 40MHZ very well. But we are using 80 MHZ for testing purpose. You may reduce it later. Also set XPT2046 SPI speed.
6. **Open Documets -> Arduino -> Arduino -> library -> TFT_eSPI -> User_Setup_Select.h**
7. findout required TFT Driver and uncomment that. **ONLY ONE Driver must be available.**
In our case we are using ILI9341 Driver.
> #include <User_Setups/Setup1_ILI9341.h>

8. open **Open Documets -> Arduino -> Arduino -> library -> TFT_eSPI -> User_Setups** 
9. locate your TFT driver File and update this file with pin modification which we did in user_setup.h. **Pin configuration must be same in both file.**
10. Now we are REDAY to ROCK.
11. Compile and UpLoad. 



