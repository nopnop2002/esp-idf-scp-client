# esp-idf-scp-client
scp client example for esp-idf.   
You can use the ssh API to work with remote files and directories.   
This project use [this](https://gitlab.com/ch405labs/ch405labs_esp_libssh2) ssh library.   
This is great work.   

# Software requirements
ESP-IDF V5.0 or later.   
ESP-IDF V4.4 release branch reached EOL in July 2024.   

# Installation
```
git clone https://github.com/nopnop2002/esp-idf-scp-client
cd esp-idf-scp-client/scp-listdir
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
libssh2_agent_connect()
libssh2_agent_disconnect()
libssh2_agent_free()
libssh2_agent_get_identity()
libssh2_agent_get_identity_path()
libssh2_agent_init()
libssh2_agent_list_identities()
libssh2_agent_set_identity_path()
libssh2_agent_userauth()
libssh2_banner_set()
libssh2_base64_decode()
libssh2_channel_close()
libssh2_channel_direct_tcpip()
libssh2_channel_direct_tcpip_ex()
libssh2_channel_eof()
libssh2_channel_exec()
libssh2_channel_flush()
libssh2_channel_flush_ex()
libssh2_channel_flush_stderr()
libssh2_channel_forward_accept()
libssh2_channel_forward_cancel()
libssh2_channel_forward_listen()
libssh2_channel_forward_listen_ex()
libssh2_channel_free()
libssh2_channel_get_exit_signal()
libssh2_channel_get_exit_status()
libssh2_channel_handle_extended_data()
libssh2_channel_handle_extended_data2()
libssh2_channel_ignore_extended_data()
libssh2_channel_open_ex()
libssh2_channel_open_session()
libssh2_channel_process_startup()
libssh2_channel_read()
libssh2_channel_read_ex()
libssh2_channel_read_stderr()
libssh2_channel_receive_window_adjust()
libssh2_channel_receive_window_adjust2()
libssh2_channel_request_auth_agent()
libssh2_channel_request_pty()
libssh2_channel_request_pty_ex()
libssh2_channel_request_pty_size()
libssh2_channel_request_pty_size_ex()
libssh2_channel_send_eof()
libssh2_channel_set_blocking()
libssh2_channel_setenv()
libssh2_channel_setenv_ex()
libssh2_channel_shell()
libssh2_channel_subsystem()
libssh2_channel_wait_closed()
libssh2_channel_wait_eof()
libssh2_channel_window_read()
libssh2_channel_window_read_ex()
libssh2_channel_window_write()
libssh2_channel_window_write_ex()
libssh2_channel_write()
libssh2_channel_write_ex()
libssh2_channel_write_stderr()
libssh2_channel_x11_req()
libssh2_channel_x11_req_ex()
libssh2_exit()
libssh2_free()
libssh2_hostkey_hash()
libssh2_init()
libssh2_keepalive_config()
libssh2_keepalive_send()
libssh2_knownhost_add()
libssh2_knownhost_addc()
libssh2_knownhost_check()
libssh2_knownhost_checkp()
libssh2_knownhost_del()
libssh2_knownhost_free()
libssh2_knownhost_get()
libssh2_knownhost_init()
libssh2_knownhost_readfile()
libssh2_knownhost_readline()
libssh2_knownhost_writefile()
libssh2_knownhost_writeline()
libssh2_poll()
libssh2_poll_channel_read()
libssh2_publickey_add()
libssh2_publickey_add_ex()
libssh2_publickey_init()
libssh2_publickey_list_fetch()
libssh2_publickey_list_free()
libssh2_publickey_remove()
libssh2_publickey_remove_ex()
libssh2_publickey_shutdown()
libssh2_scp_recv()
libssh2_scp_recv2()
libssh2_scp_send()
libssh2_scp_send64()
libssh2_scp_send_ex()
libssh2_session_abstract()
libssh2_session_banner_get()
libssh2_session_banner_set()
libssh2_session_block_directions()
libssh2_session_callback_set()
libssh2_session_disconnect()
libssh2_session_disconnect_ex()
libssh2_session_flag()
libssh2_session_free()
libssh2_session_get_blocking()
libssh2_session_get_timeout()
libssh2_session_handshake()
libssh2_session_hostkey()
libssh2_session_init()
libssh2_session_init_ex()
libssh2_session_last_errno()
libssh2_session_last_error()
libssh2_session_method_pref()
libssh2_session_methods()
libssh2_session_set_blocking()
libssh2_session_set_last_error()
libssh2_session_set_timeout()
libssh2_session_startup()
libssh2_session_supported_algs()
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

- sftp-client   
Reading and writing files using sftp protocol

# Reference
https://github.com/nopnop2002/esp-idf-ssh-client

You can use scp and ssh to do heavy processing that esp32 alone cannot.  
For example, image processing:   
- Take a picture using the esp32-cam.   
- Send image files to remote using scp-put.   
- Image processing such as shading is performed on the remote side using ssh-client.   
- Receive image file from remote using scp-get.   
