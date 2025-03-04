/*	sftp client Example

	This example code is in the Public Domain (or CC0 licensed, at your option.)

	Unless required by applicable law or agreed to in writing, this
	software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
	CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "netdb.h" // gethostbyname
#include "nvs_flash.h"
#include "esp_vfs.h"
#include "esp_spiffs.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#include "libssh2_config.h"
#include <libssh2.h>
#include <libssh2_sftp.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>

/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

static const char *TAG = "MAIN";

static int s_retry_num = 0;

static void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
	if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
		esp_wifi_connect();
	} else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
		if (s_retry_num < CONFIG_ESP_MAXIMUM_RETRY) {
			esp_wifi_connect();
			s_retry_num++;
			ESP_LOGI(TAG, "retry to connect to the AP");
		} else {
			xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
		}
		ESP_LOGI(TAG,"connect to the AP fail");
	} else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
		ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
		ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
		s_retry_num = 0;
		xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
	}
}

esp_err_t wifi_init_sta(void)
{
	s_wifi_event_group = xEventGroupCreate();

	ESP_ERROR_CHECK(esp_netif_init());
	ESP_ERROR_CHECK(esp_event_loop_create_default());
	esp_netif_create_default_wifi_sta();

	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_wifi_init(&cfg));

	esp_event_handler_instance_t instance_any_id;
	esp_event_handler_instance_t instance_got_ip;
	ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
		ESP_EVENT_ANY_ID,
		&event_handler,
		NULL,
		&instance_any_id));
	ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
		IP_EVENT_STA_GOT_IP,
		&event_handler,
		NULL,
		&instance_got_ip));

	wifi_config_t wifi_config = {
		.sta = {
			.ssid = CONFIG_ESP_WIFI_SSID,
			.password = CONFIG_ESP_WIFI_PASSWORD,
			/* Setting a password implies station will connect to all security modes including WEP/WPA.
			 * However these modes are deprecated and not advisable to be used. Incase your Access point
			 * doesn't support WPA2, these mode can be enabled by commenting below line */
			.threshold.authmode = WIFI_AUTH_WPA2_PSK,

			.pmf_cfg = {
				.capable = true,
				.required = false
			},
		},
	};
	ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));
	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
	ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
	ESP_ERROR_CHECK(esp_wifi_start());

	/* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
	 * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
	esp_err_t ret_value = ESP_OK;
	EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
		WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
		pdFALSE,
		pdFALSE,
		portMAX_DELAY);

	/* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
	 * happened. */
	if (bits & WIFI_CONNECTED_BIT) {
		ESP_LOGI(TAG, "connected to ap SSID:%s", CONFIG_ESP_WIFI_SSID);
	} else if (bits & WIFI_FAIL_BIT) {
		ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s", CONFIG_ESP_WIFI_SSID, CONFIG_ESP_WIFI_PASSWORD);
		ret_value = ESP_FAIL;
	} else {
		ESP_LOGE(TAG, "UNEXPECTED EVENT");
		ret_value = ESP_FAIL;
	}

	/* The event will not be processed after unregister */
	ESP_ERROR_CHECK(esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, instance_got_ip));
	ESP_ERROR_CHECK(esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, instance_any_id));
	vEventGroupDelete(s_wifi_event_group);
	return ret_value;
}

esp_err_t mountSPIFFS(char * partition_label, char * base_path) {
	esp_vfs_spiffs_conf_t conf = {
		.base_path = base_path,
		.partition_label = partition_label,
		.max_files = 8,
		.format_if_mount_failed = true
	};

	// Use settings defined above to initialize and mount SPIFFS filesystem.
	// Note: esp_vfs_spiffs_register is an all-in-one convenience function.
	esp_err_t ret = esp_vfs_spiffs_register(&conf);

	if (ret != ESP_OK) {
		if (ret == ESP_FAIL) {
			ESP_LOGE(TAG, "Failed to mount or format filesystem");
		} else if (ret == ESP_ERR_NOT_FOUND) {
			ESP_LOGE(TAG, "Failed to find SPIFFS partition");
		} else {
			ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
		}
		return ret;
	}

	size_t total = 0, used = 0;
	ret = esp_spiffs_info(partition_label, &total, &used);
	if (ret != ESP_OK) {
		ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
	} else {
		ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
	}
	ESP_LOGI(TAG, "Mount SPIFFS filesystem");
	return ret;
}

void sftp(void *pvParameters)
{
	// SSH Staff
	int sock;
	struct sockaddr_in sin;
	const char *fingerprint;
	LIBSSH2_SESSION *session;
	LIBSSH2_SFTP *sftp_session;
	LIBSSH2_SFTP_HANDLE *sftp_handle;
	LIBSSH2_SFTP_ATTRIBUTES attrs;

	ESP_LOGI(TAG, "libssh2_version is %s", LIBSSH2_VERSION);
	int rc = libssh2_init(0);
	if(rc) {
		ESP_LOGE(TAG, "libssh2 initialization failed (%d)", rc);
		while(1) { vTaskDelay(1); }
	}

	ESP_LOGD(TAG, "CONFIG_SFTP_HOST=%s", CONFIG_SFTP_HOST);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(22);
	sin.sin_addr.s_addr = inet_addr(CONFIG_SFTP_HOST);
	ESP_LOGD(TAG, "sin.sin_addr.s_addr=%"PRIx32, sin.sin_addr.s_addr);
	if (sin.sin_addr.s_addr == 0xffffffff) {
		struct hostent *hp;
		hp = gethostbyname(CONFIG_SFTP_HOST);
		if (hp == NULL) {
			ESP_LOGE(TAG, "gethostbyname fail");
			ESP_LOGE(TAG, "CONFIG_SFTP_HOST=%s", CONFIG_SFTP_HOST);
			while(1) { vTaskDelay(1); }
		}
		struct ip4_addr *ip4_addr;
		ip4_addr = (struct ip4_addr *)hp->h_addr;
		sin.sin_addr.s_addr = ip4_addr->addr;
		ESP_LOGD(TAG, "sin.sin_addr.s_addr=%"PRIx32, sin.sin_addr.s_addr);
	}

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1) {
		ESP_LOGE(TAG, "failed to create socket!");
		while(1) { vTaskDelay(1); }
	}

	if(connect(sock, (struct sockaddr*)(&sin), sizeof(struct sockaddr_in)) != 0) {
		ESP_LOGE(TAG, "failed to connect!");
		ESP_LOGE(TAG, "CONFIG_SFTP_HOST=%s", CONFIG_SFTP_HOST);
		while(1) { vTaskDelay(1); }
	}

	/* Create a session instance */
	session = libssh2_session_init();
	if(!session) {
		ESP_LOGE(TAG, "failed to session init");
		while(1) { vTaskDelay(1); }
	}

	/* ... start it up. This will trade welcome banners, exchange keys,
	 * and setup crypto, compression, and MAC layers
	 */
	rc = libssh2_session_handshake(session, sock);
	if(rc) {
		ESP_LOGE(TAG, "Failure establishing SSH session: %d", rc);
		while(1) { vTaskDelay(1); }
	}

	/* At this point we havn't yet authenticated.  The first thing to do
	 * is check the hostkey's fingerprint against our known hosts Your app
	 * may have it hard coded, may go to a file, may present it to the
	 * user, that's your call
	 */
	fingerprint = libssh2_hostkey_hash(session, LIBSSH2_HOSTKEY_HASH_SHA1);
	printf("Fingerprint: ");
	for(int i = 0; i < 20; i++) {
		printf("%02X ", (unsigned char)fingerprint[i]);
	}
	printf("\n");

	/* We could authenticate via password */
	if(libssh2_userauth_password(session, CONFIG_SFTP_USER, CONFIG_SFTP_PASSWORD)) {
		ESP_LOGE(TAG, "Authentication by password failed.");
		ESP_LOGE(TAG, "Authentication username : [%s].", CONFIG_SFTP_USER);
		while(1) { vTaskDelay(1); }
	}

	// Initialize sftp
	sftp_session = libssh2_sftp_init(session);
	if(!sftp_session) {
		ESP_LOGE(TAG, "Unable to init SFTP session");
		while(1) { vTaskDelay(1); }
	}

	const char *localFile = "/spiffs/local.txt";
	const char *remoteFile = "remote.txt";

	FILE *local = fopen(localFile, "rb");
	if(!local) {
		ESP_LOGE(TAG, "Can't open local file %s", localFile);
		while(1) { vTaskDelay(1); }
	}

	int stat = libssh2_sftp_stat(sftp_session, remoteFile, &attrs);
	ESP_LOGI(TAG, "libssh2_sftp_stat=%d", stat);
	if (stat == 0) {
		libssh2_sftp_unlink(sftp_session, remoteFile);
		ESP_LOGI(TAG, "remote file exist. sftp_unlink done");
	}

	/* Request a file via SFTP */ 
	sftp_handle = libssh2_sftp_open(sftp_session, remoteFile,
					LIBSSH2_FXF_WRITE |
					LIBSSH2_FXF_CREAT |
					LIBSSH2_FXF_TRUNC,
					LIBSSH2_SFTP_S_IRUSR |
					LIBSSH2_SFTP_S_IWUSR |
					LIBSSH2_SFTP_S_IRGRP |
					LIBSSH2_SFTP_S_IROTH);
	if(!sftp_handle) {
		ESP_LOGE(TAG, "Unable to open file with SFTP: %ld", libssh2_sftp_last_error(sftp_session));
		while(1) { vTaskDelay(1); }
	}

	char mem[1024];
	char *ptr;
	ssize_t nwritten;
	do {
		size_t nread = fread(mem, 1, sizeof(mem), local);
		if(nread <= 0) {
			/* end of file */
			break;
		}
		ptr = mem;
 
		do {
			/* write data in a loop until we block */ 
			nwritten = libssh2_sftp_write(sftp_handle, ptr, nread);

			if(nwritten < 0)
				break;
			ptr += nwritten;
			nread -= (size_t)nwritten;
		} while(nread);
	} while(nwritten > 0);

	fclose(local);
	libssh2_sftp_close(sftp_handle);
	ESP_LOGI(TAG, "sftp_write done");

	/* Request a file via SFTP */ 
	sftp_handle = libssh2_sftp_open(sftp_session, remoteFile, LIBSSH2_FXF_READ, 0);
	if(!sftp_handle) {
		ESP_LOGE(TAG, "Unable to open file with SFTP: %ld", libssh2_sftp_last_error(sftp_session));
		while(1) { vTaskDelay(1); }
	}
 
	do {
		char mem[1024];
		ssize_t nread;
 
		/* loop until we fail */ 
		nread = libssh2_sftp_read(sftp_handle, mem, sizeof(mem));
		if(nread > 0) {
			write(1, mem, (size_t)nread);
		}
		else {
			break;
		}
	} while(1);
 
	libssh2_sftp_close(sftp_handle);
	ESP_LOGI(TAG, "sftp_read done");

	// Shutdown sftp
	libssh2_sftp_shutdown(sftp_session);

	// Close a session
	libssh2_session_disconnect(session, "Normal Shutdown, Thank you for playing");
	libssh2_session_free(session);

	// Close socket
	shutdown(sock, 2);
	close(sock);

	libssh2_exit();
	vTaskDelete(NULL);
}

void app_main(void)
{
	// Initialize NVS
	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret);

	// Initialize WiFi
	ESP_ERROR_CHECK(wifi_init_sta());

	// Initialize SPIFFS
	char *partition_label = "storage";
	char *base_path = "/spiffs";
	ret = mountSPIFFS(partition_label, base_path);
	if (ret != ESP_OK) return;

	xTaskCreate(sftp, "SFTP", 1024*6, NULL, 5, NULL);
}
