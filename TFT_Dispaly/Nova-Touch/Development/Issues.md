

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
3. Need to interface Real time clock module. 
4. Battery level Indicator
5. Flip the Price window -->> Need approval from Sir  
6. update clock at real time

# Close Issues
----------------
1.  Time lag while updating digit. 
2.  Reduce window dot radius.

# Under Development
---------------------
1. Lower Strip Text with various Kind of imformation.  



## Date : 1/4/2021
---------

1. Some times error occured which says that we don't have heap anymore.
   HEAP !=NULL && free() .......

   Solution : Please erase esp32 chip using ESP-IDF
  
   C:\Users\Usewr\esp\esp-idf\components\esptool_py\esptool>esptool.py --chip esp32 -p com3 erase_flash

