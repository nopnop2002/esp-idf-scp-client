# scp-put 
Put to a file

# Installation for ESP32

```
git clone https://github.com/nopnop2002/esp-idf-scp-client
cd esp-idf-scp-client/scp-put
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
cd esp-idf-scp-client/scp-put
git clone https://github.com/libssh2/libssh2 components/libssh2
cp esp-idf/libssh2_config.h components/libssh2/include
cp esp-idf/CMakeLists.txt components/libssh2
idf.py set-target esp32s2
idf.py menuconfig
idf.py flash
```

![config-put](https://user-images.githubusercontent.com/6020549/119905119-90c0a800-bf86-11eb-9071-8bf6742a1f87.jpg)

