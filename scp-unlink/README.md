# scp-unlink 
Unlink a file using ssh connection

# Installation

```
git clone https://github.com/nopnop2002/esp-idf-scp-client
cd esp-idf-scp-client
git clone https://github.com/libssh2/libssh2 components/libssh2
cp esp-idf/libssh2_config.h components/libssh2/include
cp esp-idf/CMakeLists.txt components/libssh2
cd scp-unlink
idf.py set-target {esp32/esp32s2/esp32s3/esp32c3}
idf.py menuconfig
idf.py flash
```

![config-unlink](https://user-images.githubusercontent.com/6020549/166658815-6e100462-3226-4720-96ca-19f90f62f95d.jpg)

