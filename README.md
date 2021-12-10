# AfayaFurby
Nos hemos propuesto crear el "Furby" mas sencillo que se nos ocurriese con Arduino. Con un sensor de humedad, dos botones, un led de colores, un giroscopio y un reproductor mp3, este es el resultado.


## Initial idea

To create a custom and simple Furby. For these reason I bought and Arduino Mega, and some components:
  Elegoo MEga 2560 Controller Board
  CQRobot Speaker 3 Watt 8
  DHT11 Wingoneer
  MicroSD Card Adapter
  GY-521 Modulo MPU-6050 Gyroscope

I also used switchs, wires, leds, resistences and a protoboard that I had at home.

## First day.

I started creating the method por turning on the blue led that indicates if the program is running. I connected it to pin 2 and the code is based on:
https://www.arduino.cc/en/Tutorial/BuiltInExamples/BlinkWithoutDelay

Also I created a method party in which I turned on a rgb led changing the colors from red, to blue and green. I discovered that if I do not add a delay after each color, the change could not be seen. I created an unparty method too, that puts this led on white. I used pins 3, 4 and 5.
The code is based on:
https://aprendiendoarduino.wordpress.com/2019/03/02/led-rgb-con-arduino/

After that I created the method for the back button. This button when clicked makes the furby say: "Party!!" (unavailable right now) and then turn on the rgb led on the tail on different colours (the method made before). I connected it on pin 6.
This was based on:
https://www.arduino.cc/en/tutorial/switch

## Second day.
I created the method for the mouth button, using the same logic as the back button.

I also created the claimAttention method, that is called inside the method for the DHT11.

For obtaining the DHT11 info, I added the following libraries:
	Adafruit Unified Sensor
	DHT sensor library
	DHT table

Also I joined it with my board, and code following this link:
https://programarfacil.com/blog/arduino-blog/sensor-dht11-temperatura-humedad-arduino/


The, I rewrited the logic, because I detected that every time the back button was pressed does not translate in the function associated working. It was a matter of knowledge abotu how the code was processed on Arduino and in which order.

I tried to make the giroscope work following this instructions but it did not work good:
https://mschoeffler.com/2017/10/05/tutorial-how-to-use-the-gy-521-module-mpu-6050-breakout-board-with-the-arduino-uno/

## Third day.

After soldering the wires to the gyroscope, I tested again yesterday´s code and it worked perfectly. After that I changed the code, to add a new simple algorithm to test if the furby has been moved and joined it with the rest of the code developed.

I also used the MP3 Player, with a SD card and a speaker to make the project speak with specific mp3 audios that I saved in the SD card. I followed the instructions of this video:
https://www.youtube.com/watch?v=ojBdH23Ol-c

I also modified a little bit the code to make it work as I wanted.

## Rest of days
We created the circuit diagram, we sold all the components and put it inside our plush toy.


