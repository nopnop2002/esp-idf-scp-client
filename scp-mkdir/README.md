# scp-mkdir 
Create new directory

# Installation for ESP32

```
git clone https://github.com/nopnop2002/esp-idf-scp-client
cd esp-idf-scp-client/scp-mkdir
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
cd esp-idf-scp-client/scp-mkdir
git clone https://github.com/libssh2/libssh2 components/libssh2
cp esp-idf/libssh2_config.h components/libssh2/include
cp esp-idf/CMakeLists.txt components/libssh2
idf.py set-target esp32s2
idf.py menuconfig
idf.py flash
```

![config-mkdir](https://user-images.githubusercontent.com/6020549/119905041-62db6380-bf86-11eb-89d7-cfae3e515522.jpg)

