# scp-rmdir 
Delete a directory using ssh connection

# Installation

```
git clone https://github.com/nopnop2002/esp-idf-scp-client
cd esp-idf-scp-client
git clone https://github.com/libssh2/libssh2 components/libssh2
cp esp-idf/libssh2_config.h components/libssh2/include
cp esp-idf/CMakeLists.txt components/libssh2
cd scp-rmdir
idf.py set-target {esp32/esp32s2/esp32s3/esp32c3}
idf.py menuconfig
idf.py flash
```

![config-rmdir](https://user-images.githubusercontent.com/6020549/166658292-aaf1ac3d-2cc1-4653-bfc6-8b35adc8a2c8.jpg)


