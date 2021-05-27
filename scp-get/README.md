# scp-get 
Get from a file

# Installation for ESP32

```
git clone https://github.com/nopnop2002/esp-idf-scp-client
cd esp-idf-scp-client/scp-get
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
cd esp-idf-scp-client/scp-get
git clone https://github.com/libssh2/libssh2 components/libssh2
cp esp-idf/libssh2_config.h components/libssh2/include
cp esp-idf/CMakeLists.txt components/libssh2
idf.py set-target esp32s2
idf.py menuconfig
idf.py flash
```

![config-get](https://user-images.githubusercontent.com/6020549/119904928-2871c680-bf86-11eb-839e-1ac6167e01db.jpg)

