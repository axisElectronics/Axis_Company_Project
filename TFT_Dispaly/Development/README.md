
# Development Folder :

1. **Custom-Font_Module**
2. **Defualt_Settings**  
3. **Dependency-Library**
4. **ScaleSetup**
5. **Setting-Module**
6. **User-KeyPad-Input-Module**
7. **Weighing-Images**
   

## Custom-Font_Module 
*This Folder contains all the .c Files of Fonts which are we using in this project.
Differnts fonts of differnet sizes are available here.*

## Defualt_Settings
*This is a independent module. This folder doesnot required any kind of image. It provides access of different settings parameters to developer.*

## Dependency-Library
*This folder has ZIP files  of all **latest** library used in this project. It is developer responsibilty to check and update this folder with new **Stable** library.*

**_Stable Library Versions_**
```
1. CustomFont-18-02-21
2. customImages-06-03-21
3. default-settings-SPIFFS-12-03-21
4. esp32-InputModule-08-03-21
5. settingModule-13-03-21
6. TFT_eSPI-master
```   

## ScaleSetup
*This the is **main project** which used all libraries.* 

## Setting-Module
*This is a module which handles all the setting parameters execution and saving of setting parameters into SPIFFS of esp32 (permanent storage). This module inherets Default_Settings module, Custom-Font_Module, Weighing-Images, and User-Keypad_inout-Modules.* 

## User-KeyPad-Input-Module
*This module is created as independent module to take user input through various modes.* 
```
1. It has fix format module.
2. It supports Password entery mode.
3. String type data entry i.e name
4. It supports Alpha numeric keypad. Which can we switch at any time. If developer enable that.
```

## Weighing-Images
*This Folder contains all the images data. These images are converted into C type array using software. These images are used in various modules as per requirements.*


  










