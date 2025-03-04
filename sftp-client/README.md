# sftp-client
Reading and writing files using sftp protocol

# Installation

```
git clone https://github.com/nopnop2002/esp-idf-scp-client
cd esp-idf-scp-client/sftp-client
idf.py menuconfig
idf.py flash
```

![Image](https://github.com/user-attachments/assets/66065823-190b-4143-9234-abf823193fd4)


# How to Set Up an SFTP Server on Linux
I built SFTP Serve according to [this](https://www.techrepublic.com/article/how-to-set-up-an-sftp-server-on-linux/).   


# SFTP API   
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
