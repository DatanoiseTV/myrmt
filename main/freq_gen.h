

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

#include "driver/gpio.h"
#include "driver/rmt.h"

/* ************************************************************************* */
/*                      DEFINES AND ENUMERATIONS SECTION                     */
/* ************************************************************************* */

#define GPIO_5   5
#define GPIO_18 18
#define GPIO_19 19
#define GPIO_21 21


/* ************************************************************************* */
/*                               DATATYPES SECTION                           */
/* ************************************************************************* */

typedef struct {
    rmt_item32_t* items;      // Array of RMT items including EoTx
    size_t        nitems;     // number of RMT items in the array, including EoTx
    gpio_num_t    gpio_num;   // GPIO pin allocated to this frequency generator
    rmt_channel_t channel;    // Allocated RMT channel
    uint8_t       mem_blocks; // number of memory blocks consumed (1 block = 64 RMT items)
} fgen_resources_t;

typedef struct {
    double        freq;       // real frequency after adjustment (Hz)
    double        duty_cycle; // duty cycle after adjustments (0 < x < 1)
    rmt_item32_t* items;      // Array of RMT items including EoTx
    size_t        nitems;     // number of RMT items in the array, including EoTx
    size_t        onitems;    // original items sequence length without duplication nor  EoTx
    uint8_t       nrep;       // how many times the items sequence is repeated (1 < nrep)
    uint8_t       mem_blocks; // number of memory blocks consumed (1 block = 64 RMT items)
    uint8_t       prescaler;  // RMT prescaler value
    uint32_t      N;          // Big divisor to decompose in items (internal value)
    uint32_t      NH;         // The high level part of N (N = NH + NL)
    uint32_t      NL;         // The low level part of N  (N = NH + NL)
} fgen_params_t;



/* ************************************************************************* */
/*                          GLOBAL VARIABLES SECTION                         */
/* ************************************************************************* */



/* ************************************************************************* */
/*                               API FUNCTIONS                               */
/* ************************************************************************* */

esp_err_t fgen_info(double freq, double duty_cycle, fgen_params_t* fparams);

esp_err_t fgen_allocate(const fgen_params_t* fparams, gpio_num_t gpio_num, fgen_resources_t* res);

esp_err_t fgen_free(const fgen_params_t* fparams, fgen_resources_t* res);

esp_err_t fgen_start(rmt_channel_t channel);

esp_err_t fgen_stop(rmt_channel_t channel);




#ifdef __cplusplus
}
#endif


