

# Development Logs:
-----------


```
it has daily programming issues while developing this project.
it is For developer purpose. 
```


## Open Issues
---------------
1. Sampling in Counting Mode.
2. price computing need previous data value. 
3. Battery level Indicator[ Ask Neeraj Sir for This ]
4. Flip the Price window -->> Need approval from Sir .
5. Time Format AM and PM must be changed But it is not updating till now.
6. Make a Terminal Which will be used for debugging.
	This Terminal must have below attributes.
	1 > data continuous data Comming from Machine i.e Serial Terminal
	2 > 

 

# Close Issues
----------------
1. Lower Strip Text with various Kind of imformation.  
2. Time lag while updating digit. 
3. Reduce window dot radius.
4. Need to interface Real time clock module. update clock at real time.
5. RTC-DS1307 has been replaced with RTC-R8025 and Libary is supporting.



# Under Development
---------------------
1.

## Date : 1/4/2021
---------

1. Some times error occured which says that we don't have heap anymore.
   HEAP !=NULL && free() .......

   Solution : Please erase esp32 chip using ESP-IDF
  
   C:\Users\Usewr\esp\esp-idf\components\esptool_py\esptool>esptool.py --chip esp32 -p com3 erase_flash

