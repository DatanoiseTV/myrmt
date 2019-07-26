/* RMT transmit example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/



/* ************************************************************************* */
/*                         INCLUDE HEADER SECTION                            */
/* ************************************************************************* */

// -------------------
// C standard includes
// -------------------

// -----------------------------------
// Expressif SDK-IDF standard includes
// -----------------------------------

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

// --------------
// Local includes
// --------------

#include "freq_gen.h"


/* ************************************************************************* */
/*                      DEFINES AND ENUMERATIONS SECTION                     */
/* ************************************************************************* */

#define MAIN_TAG "main"

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
/*                             MAIN ENTRY POINT                              */
/* ************************************************************************* */

void app_main(void *ignore)
{
    int i = 1;
    fgen_params_t    fparams[4];
    fgen_resources_t resource[4];

    ESP_LOGI(MAIN_TAG, "Configuring transmitters");
    fgen_info( 50012, 0.5, &fparams[0]);
    fgen_info( 1.0,   0.5, &fparams[1]);
    fgen_info( 0.1,   0.5, &fparams[2]);
    fgen_info( 0.04,  0.5, &fparams[3]);

    fgen_allocate(&fparams[0], GPIO_5,  &resource[0]);
    fgen_allocate(&fparams[1], GPIO_18, &resource[1]);
    fgen_allocate(&fparams[2], GPIO_19, &resource[2]);
    fgen_allocate(&fparams[3], GPIO_21, &resource[3]);

    //fgen_init(RMT_CHANNEL_0, GPIO_5,  0.04,  true, &fparams[0]);
    //fgen_init(RMT_CHANNEL_2, GPIO_18, 0.1,   true, &fparams[1]);
    //fgen_init(RMT_CHANNEL_4, GPIO_19, 1.0,   true, &fparams[2]);
    //fgen_init(RMT_CHANNEL_6, GPIO_21, 50012, true, &fparams[3]);

    ESP_ERROR_CHECK(fgen_start(resource[0].channel));
    ESP_ERROR_CHECK(fgen_start(resource[1].channel));
    ESP_ERROR_CHECK(fgen_start(resource[2].channel));
    ESP_ERROR_CHECK(fgen_start(resource[3].channel));

    while (1) {
        ESP_LOGI(MAIN_TAG, "Forever loop (%d)", i++);
        vTaskDelay(120000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}
