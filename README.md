# esp-idf-scp-client
scp client example for esp-idf.   
This project use [this](https://github.com/libssh2/libssh2) ssh library.   

# Software requirements
esp-idf v4.4 or later.   

# Installation for ESP32

```
git clone https://github.com/nopnop2002/esp-idf-scp-client
cd esp-idf-scp-client/scp-listdir
git clone https://github.com/libssh2/libssh2 components/libssh2
cp esp-idf/libssh2_config.h components/libssh2/include
cp esp-idf/CMakeLists.txt components/libssh2
idf.py set-target esp32
idf.py menuconfig
idf.py flash
```

# Installation for ESP32-S2

```
git clone https://github.com/nopnop2002/esp-idf-scp-client
cd esp-idf-scp-client/scp-listdir
git clone https://github.com/libssh2/libssh2 components/libssh2
cp esp-idf/libssh2_config.h components/libssh2/include
cp esp-idf/CMakeLists.txt components/libssh2
idf.py set-target esp32s2
idf.py menuconfig
idf.py flash
```

# Configuration   
You have to set this config value with menuconfig.   
- CONFIG_ESP_WIFI_SSID   
SSID (network name) to connect to.
- CONFIG_ESP_WIFI_PASSWORD   
WiFi password (WPA or WPA2) to use.
- CONFIG_ESP_MAXIMUM_RETRY   
Set the Maximum retry to avoid station reconnecting to the AP unlimited when the AP is really inexistent.
- CONFIG_SSH_USER   
Username of SSH.
- CONFIG_SSH_PASSWORD   
Password of SSH.
- CONFIG_SSH_HOST   
IP address of SSH host.   
__mDMS name cannot be used.__   
- CONFIG_SSH_PATH   
Path to list.   

![config-main](https://user-images.githubusercontent.com/6020549/119904361-33782700-bf85-11eb-8a90-c5f05ac83f87.jpg)
![config-listdir](https://user-images.githubusercontent.com/6020549/119904366-3541ea80-bf85-11eb-9f85-90f93d144a35.jpg)

# Examples
- scp-listdir   
 Obtain directory list

- scp-get   
 Get from a file

- scp-put   
 Put to a file

- scp-mkdir   
 Create new directory
