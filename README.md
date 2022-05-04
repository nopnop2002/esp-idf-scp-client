# esp-idf-scp-client
scp client example for esp-idf.   
This project use [this](https://github.com/libssh2/libssh2) ssh library.   

# Software requirements
- esp-idf v4.4   
This is because this version supports ESP32-S3 and uses mbed TLS 2.16.11.   
ESP-IDF Ver5 has been updated to mbed TLS 3.1.0, but [this](https://github.com/libssh2/libssh2) ssh library does not support mbed TLS 3.1.0.   

# Installation

```
git clone https://github.com/nopnop2002/esp-idf-scp-client
cd esp-idf-scp-client
git clone https://github.com/libssh2/libssh2 components/libssh2
cp esp-idf/libssh2_config.h components/libssh2/include
cp esp-idf/CMakeLists.txt components/libssh2
cd scp-listdir
idf.py set-target {esp32/esp32s2/esp32s3/esp32c3}
idf.py menuconfig
idf.py flash
```

# Configuration   

![config-main](https://user-images.githubusercontent.com/6020549/119904361-33782700-bf85-11eb-8a90-c5f05ac83f87.jpg)
![config-listdir](https://user-images.githubusercontent.com/6020549/166656776-74668eae-a92d-4acd-8d4f-ca2eca964579.jpg)

# API   
https://www.libssh2.org/docs.html

You can use these APIs to work with remote files and directories.   
```
libssh2_sftp_close()
libssh2_sftp_close_handle()
libssh2_sftp_closedir()
libssh2_sftp_fsetstat()
libssh2_sftp_fstat()
libssh2_sftp_fstat_ex()
libssh2_sftp_fstatvfs()
libssh2_sftp_fsync()
libssh2_sftp_get_channel()
libssh2_sftp_init()
libssh2_sftp_last_error()
libssh2_sftp_lstat()
libssh2_sftp_mkdir()
libssh2_sftp_mkdir_ex()
libssh2_sftp_open()
libssh2_sftp_open_ex()
libssh2_sftp_opendir()
libssh2_sftp_read()
libssh2_sftp_readdir()
libssh2_sftp_readdir_ex()
libssh2_sftp_readlink()
libssh2_sftp_realpath()
libssh2_sftp_rename()
libssh2_sftp_rename_ex()
libssh2_sftp_rewind()
libssh2_sftp_rmdir()
libssh2_sftp_rmdir_ex()
libssh2_sftp_seek()
libssh2_sftp_seek64()
libssh2_sftp_setstat()
libssh2_sftp_shutdown()
libssh2_sftp_stat()
libssh2_sftp_stat_ex()
libssh2_sftp_statvfs()
libssh2_sftp_symlink()
libssh2_sftp_symlink_ex()
libssh2_sftp_tell()
libssh2_sftp_tell64()
libssh2_sftp_unlink()
libssh2_sftp_unlink_ex()
libssh2_sftp_write()
```

# Examples   
- scp-listdir   
Obtain directory list using ssh connection

- scp-mkdir   
Create new directory using ssh connection

- scp-rmdir   
Delete a directory using ssh connection

- scp-get   
Get from a file using ssh connection

- scp-put   
Put to a file using ssh connection

- scp-unlink   
Unlink a file using ssh connection

# Reference
https://github.com/nopnop2002/esp-idf-ssh-client

You can use scp and ssh to do heavy processing that esp32 alone cannot.  
For example, image processing:   
- Copy file from esp32 to remote using scp-put.   
- Execute remote command using ssh-client.   
- The processing result is redirected to a file.   
- Copy file from remote to esp32 using scp-get.   

