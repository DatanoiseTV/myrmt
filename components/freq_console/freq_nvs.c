
/* ************************************************************************* */
/*                         INCLUDE HEADER SECTION                            */
/* ************************************************************************* */

// -------------------
// C standard includes
// -------------------


// -----------------------------------
// Expressif SDK-IDF standard includes
// -----------------------------------

#include <esp_log.h>
#include <nvs_flash.h>
#include <nvs.h>

// --------------
// Local includes
// --------------

#include "freq_nvs.h"


/* ************************************************************************* */
/*                      DEFINES AND ENUMERATIONS SECTION                     */
/* ************************************************************************* */

#define NVS_TAG "nvs"

// namespace for NVS storage
#define FREQ_NVS_NAMESPACE "freq"

#define NVS_CHECK(a, str, ret_val) \
    if (!(a)) { \
        ESP_LOGE(NVS_TAG,"%s(%d): %s", __FUNCTION__, __LINE__, str); \
        return (ret_val); \
    }



/* ************************************************************************* */
/*                               DATATYPES SECTION                           */
/* ************************************************************************* */




/* ************************************************************************* */
/*                          GLOBAL VARIABLES SECTION                         */
/* ************************************************************************* */

/* ************************************************************************* */
/*                        AUXILIAR FUNCTIONS SECTION                         */
/* ************************************************************************* */


/* ************************************************************************* */
/*                               API FUNCTIONS                               */
/* ************************************************************************* */


esp_err_t freq_autoboot_load(uint32_t* flag)
{
	nvs_handle_t my_handle;
	esp_err_t res;

    res = nvs_open(FREQ_NVS_NAMESPACE, NVS_READONLY, &my_handle);
    NVS_CHECK(res == ESP_OK, "Error opening NVS handle",res);

    printf("Reading autoboot flag from NVS ... ");
    *flag = 0; // flag will default to 0, if not set yet in NVS

    res = nvs_get_u32(my_handle, "autoboot", flag);
    switch (res) {
    case ESP_OK:
        printf("Done\n");
        printf("autoboot flag = %d\n", *flag);
        break;
    case ESP_ERR_NVS_NOT_FOUND:
        printf("The value is not initialized yet!\n");
        break;
     default :
        printf("Error (%s) reading!\n", esp_err_to_name(res));
    }
    nvs_close(my_handle);
    return res;
}

/* ************************************************************************* */

esp_err_t freq_autoboot_save(uint32_t flag)
{
	nvs_handle_t my_handle;
	esp_err_t res;

    res = nvs_open(FREQ_NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    NVS_CHECK(res == ESP_OK, "Error opening NVS handle", res);

    // Write
    printf("Updating  autoboot flag in NVS ... ");
    res = nvs_set_u32(my_handle, "autoboot", flag);
    printf((res != ESP_OK) ? "Failed!\n" : "Done\n");

    // Commit written value.
    // After setting any values, nvs_commit() must be called to ensure changes are written
    // to flash storage. Implementations may write to storage at other times,
    // but this is not guaranteed.
    printf("Committing updates in NVS ... ");
    res = nvs_commit(my_handle);
    printf((res != ESP_OK) ? "Failed!\n" : "Done\n");

    // Close
    nvs_close(my_handle);
    return res;
}

/* ************************************************************************* */
//esp_err_t nvs_get_blob(nvs_handle_t handle, const char *key, void *out_value, size_t *length)

//esp_err_t nvs_set_blob(nvs_handle_t handle, const char *key, const void *value, size_t length)
//esp_err_t nvs_erase_key(nvs_handle_t handle, const char *key)
//esp_err_t nvs_erase_key(nvs_handle_t handle, const char *key)
/* ************************************************************************* */

esp_err_t freq_info_load(uint32_t channel, freq_nvs_info_t* info)
{
	nvs_handle_t my_handle;
	esp_err_t    res;
	char         key[2];
	size_t       length;

    res = nvs_open(FREQ_NVS_NAMESPACE, NVS_READONLY, &my_handle);
    NVS_CHECK(res == ESP_OK, "Error opening NVS handle",res);
    printf("Loading freq_nvs_info_t info for channel %d from NVS ... ", channel);

    // Default value if not found
    info->freq       = 0;
    info->duty_cycle = 0;
    info->gpio_num   = GPIO_NUM_NC;

    // compose the key string
    key[0] = channel + '0';
    key[1] = 0;

    res = nvs_get_blob(my_handle, key, info, &length);
    switch (res) {
    case ESP_OK:
        printf("Done\n");
        break;
    case ESP_ERR_NVS_NOT_FOUND:
        printf("The value is not initialized yet!\n");
        break;
     default :
        printf("Error (%s) reading!\n", esp_err_to_name(res));
    }
    nvs_close(my_handle);
    NVS_CHECK(length == sizeof(freq_nvs_info_t), "Read size does not match freq_nvs_info_t size",ESP_FAIL);
    return res;
}

/* ************************************************************************* */

esp_err_t freq_info_save(uint32_t channel, const freq_nvs_info_t* info)
{	
	nvs_handle_t my_handle;
	esp_err_t    res;
	char         key[2];

    res = nvs_open(FREQ_NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    NVS_CHECK(res == ESP_OK, "Error opening NVS handle", res);

    // compose the key string
    key[0] = channel + '0';
    key[1] = 0;

    // Write
    printf("Updating  freq_nvs_info_t info for channel %d in NVS ... ", channel);
    res = nvs_set_blob(my_handle, key, info, sizeof(freq_nvs_info_t));
    printf((res != ESP_OK) ? "Failed!\n" : "Done\n");

    // Commit written value.
    // After setting any values, nvs_commit() must be called to ensure changes are written
    // to flash storage. Implementations may write to storage at other times,
    // but this is not guaranteed.
    printf("Committing updates for channel %d in NVS ... ", channel);
    res = nvs_commit(my_handle);
    printf((res != ESP_OK) ? "Failed!\n" : "Done\n");

    // Close
    nvs_close(my_handle);
    return res;
}

/* ************************************************************************* */

esp_err_t freq_info_erase(uint32_t channel)
{	
	nvs_handle_t my_handle;
	esp_err_t    res;
	char         key[2];

    res = nvs_open(FREQ_NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    NVS_CHECK(res == ESP_OK, "Error opening NVS handle", res);

    // compose the key string
    key[0] = channel + '0';
    key[1] = 0;

    // Write
    printf("Erasing freq_nvs_info_t info for channel %d in NVS ... ", channel);
    res = nvs_erase_key(my_handle, key);
    printf((res != ESP_OK) ? "Failed!\n" : "Done\n");

    // Commit written value.
    // After setting any values, nvs_commit() must be called to ensure changes are written
    // to flash storage. Implementations may write to storage at other times,
    // but this is not guaranteed.
    printf("Committing updates for channel %d in NVS ... ", channel);
    res = nvs_commit(my_handle);
    printf((res != ESP_OK) ? "Failed!\n" : "Done\n");

    // Close
    nvs_close(my_handle);
    return res;
}

