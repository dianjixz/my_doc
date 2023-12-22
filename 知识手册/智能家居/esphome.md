由于公司需要，有同事在适配 epshome 在 homeassistant 中的使用，在帮忙的过程中，感觉挺好用的，就记录一下。





io 操作
```yaml
esphome:
  name: nihao
  friendly_name: nihao

esp32:
  board: esp32dev
  framework:
    type: arduino

# Enable logging
logger:

# Enable Home Assistant API
api:
  encryption:
    key: "NvBm8GNJZq4NkuZAHYh5G7+mYmKjwQFhO2THyugg878="

ota:
  password: "8320de31442cf862a0c02f8a9384d499"

wifi:
  ssid: "xxx"
  password: "xxx"

  # Enable fallback hotspot (captive portal) in case wifi connection fails
  ap:
    ssid: "Nihao Fallback Hotspot"
    password: "LpYPNnpINZ1Y"

captive_portal:

web_server:  ## web控制，添加了这2行，就可以在网页端打开设备ip;
  port: 80

binary_sensor:
  - platform: gpio
    pin:
      number: GPIO33
      mode: INPUT_PULLUP
    name: "io0输入"
    id: io0_sensor

switch:
  - platform: gpio
    pin: GPIO22
    name: "wireless05_switch"
    id: relay2



```



自定义操作
``` yaml

esphome:
  name: m5cores3
  friendly_name: M5CoreS3
  libraries:
    - m5stack/M5GFX@^0.1.11
    - m5stack/M5Unified@^0.1.11
  on_boot:
    lambda:
      M5.begin();
      M5.Display.fillScreen(TFT_YELLOW);

esp32:
  board: esp32-s3-devkitc-1
  framework:
    type: arduino

psram:
  mode: octal
  speed: 80MHz



external_components:
  - source:
      type: git
      url: https://github.com/Forairaaaaa/cores3_esphome
      ref: ver1
    components: [ board_m5cores3 ]

board_m5cores3:
  id: board_m5cores3_1



# Enable logging
logger:

# Enable Home Assistant API
api:
  encryption:
    key: "lRdTPwdT4894Prkbk0hM0nIiCU/ZqPLlKKYxj2ZnKR0="

ota:
  password: "a7225cbff87b7797cb092b0b71b69a4c"

wifi:
  ssid: "xxx"
  password: 'xxx'

  # Optional manual IP
  manual_ip:
    static_ip: 192.168.2.140
    gateway: 192.168.2.1
    subnet: 255.255.255.0

  # Enable fallback hotspot (captive portal) in case wifi connection fails
  ap:
    ssid: "M5Cores3 Fallback Hotspot"
    password: "k9NQ3aRdLcWw"

captive_portal:
    


switch:

# Lcd Backlight 
- platform: template
  name: "LCD Backlight"
  id: switch_lcd_backlight
  restore_mode: "RESTORE_DEFAULT_ON"
  turn_on_action:
    - lambda:
        M5.Display.setBrightness(127);
  turn_off_action:
    - lambda: |-
        M5.Display.setBrightness(0);


```



