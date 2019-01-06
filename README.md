# M2M exam

## This is a exam project for measuring if a person is enjoying a movie or a series

### Parts used 
- Wemos D1 mini ESP8266
- MPU6050
- MAX30102
- 0.91" OLED display
- Wemos D1 relay shield

### Built with
- PlatformIO
- ThingsBoard 
- CloudMQTT broker
- IoT MQTT Panel android app

### How it works
- Place fingertip on MAX30102 sensor to measure pulse
- Place MPU6050 on thigh/arm to measure movement

The unit will measure your pulse and movement, and the OLED display will show a simplified view of this. 
Each heartbeat is displayed, alongside your pulse. Your movement will be displayed with a line of dashes. 
The longer the line, the greater the movement. 
The unit will publish the pulse and movement data to ThingsBoard, where it can be shown in multiple graphs. 

To turn off the unit, open the app and push the off button. The unit will turn off instantly. This is done
by using the ESP deepsleep function. 
To turn it on again, simply press the on button. The unit will wake up each one minute to check if the button 
has been pressed. It will then either turn on, or go back to sleep. 

### Sources
- PlatformIO
  https://platformio.org/
- ThingsBoard
  https://thingsboard.io/
- CloudMQTT broker
  https://www.cloudmqtt.com/ 
- IoT MQTT Panel
  https://play.google.com/store/apps/details?id=snr.lab.iotmqttpanel.prod&hl=no 
