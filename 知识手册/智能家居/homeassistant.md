


# 在 homeassistant ui 的网页上编辑 configuration.yaml 
需要在 add-ones 中安装，`File editor`



# 问题
## 在安装 node-red 的时候，会出现 ssl 错误的情况
在设置中关闭 ssl 便可解决这个问题。













添加自定义 mqtt 设备:  
configuration.yaml  
```yaml
mqtt:
	broker: <IP address>  # 添加MQTT服务器的IP地址
	port: <port> # 添加MQTT服务器的端口号
	username: <username>  # 添加MQTT服务器的用户名
	password: <password>  # 添加MQTT服务器的用户密码
	# 配置自动发现
	discovery: true
	# 自动发现使用的主题位置前缀，缺省为homeassistant
	discovery_prefix: homeassistant

    # 添加一个灯
    light:
        # 设备类型 MQTT
        - platform: mqtt

        # 设备名称
        name: "TestLight"

        # 订阅主题
        state_topic: "testled"

        # 发布命令的主题
        command_topic: "testled"

        # 命令格式 on 开灯 和 off 关灯
        payload_on: "on"
        payload_off: "off"
        
        # Unique ID
        unique_id: "<MAC Address>" # 可以是任何数字、字母的组合，但不能与已有实体的unique_id重复


```

一个测试例子，最新的版本貌似已经不能使用了。
```yaml
mqtt:
  broker: 127.0.0.1
  port: 1883
  username: pi #将 pi 换成你的用户名
  password: mima #将 mima 换成你的 mosquitto 密码

sersor:  
  - platform: mqtt
    state_topic: "home-assistant/arduino/test"
    name: 'HelloArduino'

```
```bash
mosquitto_pub -h 127.0.0.1 -u pi -P mima -t home-assistant/arduino/test -m "Hello" #将 pi 换成你的用户名，将 mima 换成你的密码
```

另一个测试例子，最新的版本貌似已经不能使用了。
```yaml
mqtt:
  broker: 127.0.0.1
  port: 1883
  username: pi #将 pi 换成你的用户名
  password: mima #将 mima 换成你的 mosquitto 密码

sensor:
  - platform: mqtt
    state_topic: "home-assistant/arduino/arduino1"
    name: 'Arduino_Temp_1'
    unit_of_measurement: '°C'
    value_template: '{{ value_json.Temp1 }}'
  - platform: mqtt
    state_topic: "home-assistant/arduino/arduino1"
    name: 'Arduino_Temp_2'
    unit_of_measurement: '°C'
    value_template: '{{ value_json.Temp2 }}'
  - platform: mqtt
    state_topic: "home-assistant/arduino/arduino1"
    name: 'Arduino_Humi'
    unit_of_measurement: '%'
    value_template: '{{ value_json.Humidity }}'
  - platform: mqtt
    state_topic: "home-assistant/arduino/arduino1"
    name: 'Arduino_Humi'
    unit_of_measurement: '%'
    value_template: '{{ value_json.Humidity }}'

```
```c
// file : arduino
#include <AM2320.h>
#include <SFE_BMP180.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>
#include "SR04.h"

#define Fire_AI 0
#define Brightness_AI 1
#define Fire_DI 2
#define Shock_DI 3
#define TempOnly_DI 4
#define Human1_DI 5
#define HCSR04_TRIG_PIN 6
#define HCSR04_ECHO_PIN 7
#define WIFI_RX 8
#define WIFI_TX 9

// 初始连接在单总线上的单总线设备
OneWire oneWire(TempOnly_DI);
SFE_BMP180 AirPresure;
DallasTemperature DS18B20sensors(&oneWire);
AM2320 Temp_Humi;
SR04 Obstacle = SR04(HCSR04_ECHO_PIN, HCSR04_TRIG_PIN);
SoftwareSerial MySerial(WIFI_RX, WIFI_TX);

float currentTemperature;
int currentBright = 0;
int bodySensor = 0;
int FireStatus = 0;
int FireDegree = 0;
int shockStatus = 0;
long DistanceObstance = 0;
char presureDelayTime;
double presureP, presureT;
long int lastSendTime, Now;
String JsonData = "{\"Temp1\":\"TEMP_VALUE1\",\"Temp2\":\"TEMP_VALUE2\",\"Humidity\":\"HUMI_VALUE\",\"Brightness\":\"BRIGHT_VALUE\",\"Preasure\":\"PREA_VALUE\",\"Atmosphere\":\"ATM_VALUE\",\"Fire\":\"FIRE_VALUE\",\"FireState\":\"FIRE_STATE\",\"BodyState\":\"BODY_STATE\",\"ObjectDistance\":\"DIST_VALUE\",\"ShockState\":\"SHOCK_STATE\"}";
SoftwareSerial mySerial(WIFI_RX, WIFI_TX); // RX, TX
bool lightOn = false;

void setup() {
  mySerial.begin(9600);
  mySerial.println("Hello, world?");
  pinMode(Fire_DI, INPUT);
  pinMode(Shock_DI, INPUT);
  pinMode(TempOnly_DI, INPUT);
  pinMode(Human1_DI, INPUT);
  pinMode(HCSR04_TRIG_PIN, OUTPUT);
  pinMode(HCSR04_ECHO_PIN, INPUT);

  DS18B20sensors.begin();
  AirPresure.begin();
  Now = millis();
  lastSendTime = Now;
}

void loop()
{
  Now = millis();
  if (Now - lastSendTime > 1000) {
JsonData = "{\"Temp1\":\"TEMP_VALUE1\",\"Temp2\":\"TEMP_VALUE2\",\"Humidity\":\"HUMI_VALUE\",\"Brightness\":\"BRIGHT_VALUE\",\"Preasure\":\"PREA_VALUE\",\"Atmosphere\":\"ATM_VALUE\",\"Fire\":\"FIRE_VALUE\",\"FireState\":\"FIRE_STATE\",\"BodyState\":\"BODY_STATE\",\"ObjectDistance\":\"DIST_VALUE\",\"ShockState\":\"SHOCK_STATE\"}";

//温度传感器
DS18B20sensors.requestTemperatures();
currentTemperature = DS18B20sensors.getTempCByIndex(0);
JsonData.replace("TEMP_VALUE1", String(currentTemperature));
delay(2);

//温湿度传感器
switch (Temp_Humi.Read())
{
  case 2:
  case 1:
    JsonData.replace("TEMP_VALUE2", "CRC faild");
    JsonData.replace("HUMI_VALUE", "CRC faild");
    break;
  case 0:
    JsonData.replace("TEMP_VALUE2", String(Temp_Humi.t));
    JsonData.replace("HUMI_VALUE", String(Temp_Humi.h));
    break;
}
delay(2);

//亮度传感器
currentBright = analogRead(Brightness_AI);
JsonData.replace("BRIGHT_VALUE", String(currentBright));
delay(2);

//人体传感器
bodySensor = digitalRead(Human1_DI);
switch (bodySensor)
{
  case 1:
    JsonData.replace("BODY_STATE", "Yes");
    break;
  case 0:
    JsonData.replace("BODY_STATE", "No");
    break;
}
delay(2);

//距离传感器
DistanceObstance = Obstacle.Distance();
DistanceObstance = DistanceObstance * 10.0;
JsonData.replace("DIST_VALUE", String(DistanceObstance));
delay(2);

//震动传感器
shockStatus = digitalRead(Shock_DI);
switch (shockStatus)
{
  case 1:
    JsonData.replace("SHOCK_STATE", "Vibrating");
    break;
  case 0:
    JsonData.replace("SHOCK_STATE", "Stable");
    break;
}
delay(2);

//火焰传感器 状态
FireStatus = digitalRead(Fire_DI);
switch (FireStatus)
{
  case 1:
    JsonData.replace("FIRE_STATE", "Safety");
    break;
  case 0:
    JsonData.replace("FIRE_STATE", "Fired");
    break;
}
delay(2);

//火焰传感器 红外值
FireDegree = analogRead(Fire_AI);
JsonData.replace("FIRE_VALUE", String(FireDegree));
delay(2);

//大气压
presureDelayTime = AirPresure.startPressure(3);
if (presureDelayTime != 0)
{
  delay(presureDelayTime);
  presureDelayTime = AirPresure.getPressure(presureP, presureT);
  if (presureDelayTime != 0)
  {
    JsonData.replace("PREA_VALUE", String(presureP));
    JsonData.replace("ATM_VALUE", String(presureP / 1000.0));
  }
  else
  {
    JsonData.replace("PREA_VALUE", "ERROR");
    JsonData.replace("ATM_VALUE", "ERROR");
  }
}
else
{
  JsonData.replace("PREA_VALUE", "ERROR");
  JsonData.replace("ATM_VALUE", "ERROR");
}
delay(2);

mySerial.print(JsonData);
  }
  //开关灯设备写在这里
}
```
```c
// file : esp8266
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define wifi_ssid "MyWiFi_SSID"
#define wifi_password "WIFI_Psssword"

#define mqtt_server "Raspberry_IpAddress" #这里建议将 树莓派设置为固定IP地址
#define mqtt_user "pi" #使用你的 MQTT 用户名
#define mqtt_password "password" //MQTT 密码
#define mqtt_topic "home-assistant/arduino/arduino1"

String strRecv = "";
long now = 0;
long lastRecv = 0;
bool newDataComing = false;
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("- ");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
} else {
     Serial.print("failed, rc=");
     Serial.print(client.state());
     Serial.println(" try again in 5 seconds");
     delay(5000);
}
}
}

void loop() {
  if (!client.connected()) {
reconnect();
  }
  client.loop();

if (Serial.available() > 0) {
char str = char(Serial.read());
strRecv = strRecv + str;
lastRecv = millis();
newDataComing = true;
delay(2);
  }
  else {
now = millis();
if ((now - lastRecv > 100) && (newDataComing == true)) {

  boolean isOK = client.publish(mqtt_topic, String(strRecv).c_str(), true);

  Serial.println(isOK);

  strRecv = "";
  newDataComing = false;
}
  }
}

```






homeassistant 调用脚本的例子。没有测试过
```python
import os
import json

# Return CPU temperature as a float
def getCPUtemperature():
    f = os.popen("cat /sys/class/thermal/thermal_zone0/temp")
    temp = int(f.readline().strip())/1000
    return round(temp, 1)

# Return RAM information (unit=MB) in a list
# Index 0: total RAM
# Index 1: used RAM
# Index 2: free RAM
def getRAMinfo():
    f = os.popen("free | awk '/Mem/ {print $2,$3,$4}'")
    info = f.readline().split()
    info = [round(int(i)/1024, 1) for i in info]
    return info

# Return % of CPU used by user as float
def getCPUinfo():
    # Docker外部(真实环境内）
    ## info = os.popen("top -n1 | awk '/Cpu\(s\):/ {print $2}'").readline().strip()
    # Docker内部(Home Assistant Docker内)
    info = os.popen("top -n1 | awk '/CPU:/ {print $2}'").readline().strip()
    if info=="":
        info=0
    return info

# Return information about disk space as a list (unit included)
# Index 0: total disk space
# Index 1: used disk space
# Index 2: remaining disk space
# Index 3: percentage of disk used
def getDiskinfo():
    f = os.popen("df -h /")
    info = f.readlines()[1].split()[1:5]
    return info

if __name__ == '__main__':
    RaspiInfo = {}
    RaspiInfo['CPUtemp'] = getCPUtemperature()
    RaspiInfo['RAMinfo'] = getRAMinfo()
    RaspiInfo['DISKinfo'] = getDiskinfo()
    RaspiInfo['CPUuse'] = getCPUinfo()
    # 必须转化为标准 JSON 格式
    print(json.dumps(RaspiInfo)) 作者：Mintimate https://www.bilibili.com/read/cv16224993/ 出处：bilibili

```


Command Senior :
参考官方文档：https://www.home-assistant.io/integrations/sensor.command_line/

```yaml
- platform: command_line
  name: RaspInfo
  scan_interval: 60
  command: "python3 /config/scripts/queryRaspi.py" # 脚本路径问题参考下面注意事项
  json_attributes: # 键名可为大小写
    - RAMinfo
    - DISKinfo
    - CPUuse
    - CPUtemp

- platform: template
  # 传感器列表
  sensors:
    # 实体名称：小写，下划线
    cpu_temp:
      # （可选）在前端显示的传感器昵称
      friendly_name: "CPU Temperature"
      # （可选）传感器数值的单位
      unit_of_measurement: '℃'
      #（必须）定义一个获取传感器状态（数值）的模板
      # 这里就是获取上面定义的命令行传感器实体 sensor.raspinfo 的相应属性值，注意大小写
      value_template: "{{state_attr('sensor.raspinfo', 'CPUtemp')}}"
    # 以下配置类似，不再赘述
    cpu_used:
      friendly_name: "CPU Used"
      # unit_of_measurement: '%'
      value_template: "{{state_attr('sensor.raspinfo', 'CPUuse')}}"
    ram_total:
      friendly_name: "RAM total"
      unit_of_measurement: 'MB'
      value_template: "{{state_attr('sensor.raspinfo', 'RAMinfo')[0]}}"
    ram_used:
      friendly_name: "RAM used"
      unit_of_measurement: 'MB'
      value_template: "{{state_attr('sensor.raspinfo', 'RAMinfo')[1]}}"
    ram_free:
      friendly_name: "RAM free"
      unit_of_measurement: 'MB'
      value_template: "{{state_attr('sensor.raspinfo', 'RAMinfo')[2]}}"
    disk_total:
      friendly_name: "DISK total"
      value_template: "{{state_attr('sensor.raspinfo', 'DISKinfo')[0]}}"
    disk_used:
      friendly_name: "DISK used"
      value_template: "{{state_attr('sensor.raspinfo', 'DISKinfo')[1]}}"
    disk_left:
      friendly_name: "DISK left"
      value_template: "{{state_attr('sensor.raspinfo', 'DISKinfo')[2]}}"
    disk_percentage:
      friendly_name: "DISK percentage"
      value_template: "{{state_attr('sensor.raspinfo', 'DISKinfo')[3]}}"

```

```yaml
# groups.yaml
raspinfo:
  name: 树莓派
  entities:
    - sensor.cpu_used
    - sensor.cpu_temp
    - sensor.ram_total
    - sensor.ram_used
    - sensor.ram_free
    - sensor.disk_total
    - sensor.disk_used
    - sensor.disk_left
    - sensor.disk_percentage

```

```yaml
# card
type: custom:apexcharts-card
color_list:
  - FF8394
  - FA8C16
header:
  show: true
  title: 树莓派CPU信息
  show_states: true
  colorize_states: true
series:
  - entity: sensor.cpu_temp
    name: 当前CPU温度
  - entity: sensor.cpu_used
    name: 当前CPU使用率
    type: column
    unit: '%'
graph_span: 24h
span:
  start: day
  offset: '-12h' 
```
作者：Mintimate https://www.bilibili.com/read/cv16224993/ 出处：bilibili




最新的 mqtt 传感器：
```yaml
# configuration.yaml
mqtt:
  sensor:
    - name: "温度"
      state_topic: "zigbee2mqtt/无线温度计1"
      unit_of_measurement: "°C"
      value_template: "{{ value_json.temperature }}"
    - name: "湿度"
      state_topic: "zigbee2mqtt/无线温度计1"
      unit_of_measurement: "%"
      value_template: "{{ value_json.humidity }}"

```
————————————————
版权声明：本文为CSDN博主「Chirnyo」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/adocir/article/details/127565977



