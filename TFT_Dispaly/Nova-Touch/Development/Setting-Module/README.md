

# Setting Module :

1. This is a Independent Module. Which Takes care all the setting related configuration. 
2. Please USE latest library defined by date.


## Know Issues :
@ Date : 25-03-2021 
    
 >   1. Under calibartion setting, we required two more options. 
>
>       @ Quick calibartion
> 
>       @ Manual calibartion
>
> 2. Right now after getting NOLOAD Count, LOAD Count and entering weight value. When we are trying to get data from machine. But not getting calibated continuous data.
> Althoutgh command is correct to calibarte. we are getting "OK" from Machine. [Solved]
> 
> 3. We are getting error when we giving tare values to machine. [Solved]
>
> 4. we need to implement password also.
> 5. we need to increase touch sensitivity.[  look uinto Touch.h ]
> may be I have to change #define Z_THRESHOLD 350 to #define Z_THRESHOLD 500.
> 
> 6. pre value must be print fast
> 7. when ever we enter into Weighing Mode. First ask tare weight.[Done]

@ Date : 27-03-2021 

> 1. implement Tare command in Counting Mode.
> 2. 

@ Date : 01-04-2020

1. Some times error occured which says that we don't have heap anymore.
   HEAP !=NULL && free() .......

   Solution : Please erase esp32 chip using ESP-IDF
  
   C:\Users\Usewr\esp\esp-idf\components\esptool_py\esptool>esptool.py --chip esp32 -p com3 erase_flash
