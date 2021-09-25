
# Development Folder :

1. **Custom-Font_Module**
2. **Default_Settings**  
3. **Dependency-Library**
4. **Scale Setup**
5. **Setting-Module**
6. **User-Key Pad-Input-Module**
7. **Weighing-Images**
   

## Custom-Font_Module 
*This Folder contains all the .c Files of Fonts which are we using in this project.
Different fonts of different sizes are available here.*

## Default Settings
*This is a independent module. This folder does not required any kind of image. It provides access of different settings parameters to developer.*

## Dependency-Library
*This folder has ZIP files  of all **latest** library used in this project. It is developer responsibility to check and update this folder with new **Stable** library.*

**_Stable Library Versions_**

```
1. CustomFont-18-02-21
2. customImages-06-03-21
3. default-settings-SPIFFS-12-03-21
4. esp32-InputModule-08-03-21
5. settingModule-13-03-21
6. TFT_eSPI-master
```

## Scale Setup
*This the is **main project** which used all libraries.* 

## Setting-Module
*This is a module which handles all the setting parameters execution and saving of setting parameters into SPIFFS of esp32 (permanent storage). This module inherits Default_Settings module, Custom-Font_Module, Weighing-Images, and User-Keypad_input-Modules.* 

## User Key Pad Input-Module
*This module is created as independent module to take user input through various modes.* 
```
1. It has fix format module.
2. It supports Password entery mode.
3. String type data entry i.e name
4. It supports Alpha numeric keypad. Which can we switch at any time. If developer enable that.
```

## Weighing-Images
*This Folder contains all the images data. These images are converted into C type array using software. These images are used in various modules as per requirements.*



# Hardware Connections :

 1. We are using 3 **Serial( UART )** of ESP32.
	Serial_0 ===>>
	Serial_1 ===>>
	Serial_2 ===>>
 2. We are using **SPI lines** to operate TFT.
 3. We are using **I2C line** for getting data.























