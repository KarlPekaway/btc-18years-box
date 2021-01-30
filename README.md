# btc-18years-box
 a box that hides Bitcoins for 18 years.


Based on a esp32 - az-Delivery Board and an adafruit real time clock. 
The RTC is connectet to 2x AA Lithium Battery (shelf time 20years). 
The Battery as a capactiy ~2300mah, current of rtc 150 nA --> 2300/0,00015=15.333.333h ->theoretically it would run 1.750 years.
So 20 years should be sure possible. (self discharge) 
The drift is around 2s per day. So after 18 years there should be a maximum of around 4h error. 


after reaching the 18. birthday date the box can be powered ofer usb. 
the esp32 check the date from the RTC and provides a Wifi-AP with Captive Portal -> it will redirectet 
to a website where the BTC and PrivateKey are shown. 

If the box earlier pluged in, the esp32 wifi will show something like "be cool" :) 

You find the arduino Sketch and the libraries here.
