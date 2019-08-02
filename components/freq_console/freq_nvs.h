
#pragma once

#ifdef __cplusplus
extern "C" {
#endif


/* ************************************************************************* */
/*                         INCLUDE HEADER SECTION                            */
/* ************************************************************************* */

// -----------------------------------
// Expressif SDK-IDF standard includes
// -----------------------------------

#include <driver/gpio.h>

/* ************************************************************************* */
/*                               DATATYPES SECTION                           */
/* ************************************************************************* */

typedef struct {
    double        freq;       // frequency (Hz)
    double        duty_cycle; // duty cycle (0 < x < 1)
    gpio_num_t    gpio_num;	  // GPIO number
} freq_nvs_info_t;

/* ************************************************************************* */
/*                               API FUNCTIONS                               */
/* ************************************************************************* */

esp_err_t freq_autoboot_load(uint32_t* flag);

esp_err_t freq_autoboot_save(uint32_t  flag);

esp_err_t freq_info_load(uint32_t channel, freq_nvs_info_t* info);

esp_err_t freq_info_save(uint32_t channel, const freq_nvs_info_t* info);

esp_err_t freq_info_erase(uint32_t channel);


#ifdef __cplusplus
}
#endif


