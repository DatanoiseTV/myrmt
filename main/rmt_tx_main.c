/* RMT transmit example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

/*

*************************************************************
Ref Clock = 80000000 Hz, Prescaler = 7, RMT Clock = 11428571.43 Hz
Fout = 509.000 Hz => 508.977 Hz (-0.00%), Duty Cycle = 50.00% => 50.00 (0.00%)
Ntot = 22454, Nhigh = 11227, Nlow = 11227
Nitems = 2, NChannels = 1
This sequence can be repeated 63 times + final EoTx (0,0,0,0)
Jitter due to repetition 0.0001%
[(11227, 1, 11227, 0), (0, 0, 0, 0)]
*************************************************************
Ref Clock = 80000000 Hz, Prescaler = 250, RMT Clock = 320000.00 Hz
Fout = 5.000 Hz => 5.000 Hz (0.00%), Duty Cycle = 50.00% => 50.00 (0.00%)
Ntot = 64000, Nhigh = 32000, Nlow = 32000
Nitems = 2, NChannels = 1
This sequence can be repeated 63 times + final EoTx (0,0,0,0)
Jitter due to repetition 0.0000%
[(32000, 1, 32000, 0), (0, 0, 0, 0)]
*************************************************************
Ref Clock = 80000000 Hz, Prescaler = 250, RMT Clock = 320000.00 Hz
Fout = 5.000 Hz => 5.000 Hz (0.00%), Duty Cycle = 99.50% => 99.50 (0.00%)
Ntot = 64000, Nhigh = 63680, Nlow = 320
Nitems = 3, NChannels = 1
This sequence can be repeated 31 times + final EoTx (0,0,0,0)
Jitter due to repetition 0.0001%
[(32767, 1, 30913, 1), (320, 0, 0, 0), (0, 0, 0, 0)]
*************************************************************
Ref Clock = 80000000 Hz, Prescaler = 250, RMT Clock = 320000.00 Hz
Fout = 5.000 Hz => 5.000 Hz (0.00%), Duty Cycle = 50.00% => 50.00 (0.00%)
Ntot = 64000, Nhigh = 32000, Nlow = 32000
Nitems = 2, NChannels = 1
This sequence can be repeated 63 times + final EoTx (0,0,0,0)
Jitter due to repetition 0.0000%
[(32000, 1, 32000, 0), (0, 0, 0, 0)]
*************************************************************
Ref Clock = 80000000 Hz, Prescaler = 17, RMT Clock = 4705882.35 Hz
Fout = 6473.000 Hz => 6464.124 Hz (-0.14%), Duty Cycle = 50.00% => 50.00 (0.00%)
Ntot = 728, Nhigh = 364, Nlow = 364
Nitems = 2, NChannels = 1
This sequence can be repeated 63 times + final EoTx (0,0,0,0)
Jitter due to repetition 0.0022%
[(364, 1, 364, 0), (0, 0, 0, 0)]
*************************************************************
Ref Clock = 80000000 Hz, Prescaler = 185, RMT Clock = 432432.43 Hz
Fout = 43247.000 Hz => 43243.243 Hz (-0.01%), Duty Cycle = 70.00% => 70.00 (0.00%)
Ntot = 10, Nhigh = 7, Nlow = 3
Nitems = 2, NChannels = 1
This sequence can be repeated 63 times + final EoTx (0,0,0,0)
Jitter due to repetition 0.1587%
[(7, 1, 3, 0), (0, 0, 0, 0)]
*************************************************************
Ref Clock = 80000000 Hz, Prescaler = 200, RMT Clock = 400000.00 Hz
Fout = 50000.000 Hz => 50000.000 Hz (0.00%), Duty Cycle = 70.00% => 75.00 (7.14%)
Ntot = 8, Nhigh = 6, Nlow = 2
Nitems = 2, NChannels = 1
This sequence can be repeated 63 times + final EoTx (0,0,0,0)
Jitter due to repetition 0.1984%
[(6, 1, 2, 0), (0, 0, 0, 0)]
*************************************************************
Ref Clock = 80000000 Hz, Prescaler = 87, RMT Clock = 919540.23 Hz
Fout = 70729.000 Hz => 65681.445 Hz (-7.14%), Duty Cycle = 50.00% => 50.00 (0.00%)
Ntot = 14, Nhigh = 7, Nlow = 7
Nitems = 2, NChannels = 1
This sequence can be repeated 63 times + final EoTx (0,0,0,0)
Jitter due to repetition 0.1134%
[(7, 1, 7, 0), (0, 0, 0, 0)]
*************************************************************
Ref Clock = 80000000 Hz, Prescaler = 200, RMT Clock = 400000.00 Hz
Fout = 100000.000 Hz => 100000.000 Hz (0.00%), Duty Cycle = 50.00% => 50.00 (0.00%)
Ntot = 4, Nhigh = 2, Nlow = 2
Nitems = 2, NChannels = 1
This sequence can be repeated 63 times + final EoTx (0,0,0,0)
Jitter due to repetition 0.3968%
[(2, 1, 2, 0), (0, 0, 0, 0)]
*************************************************************
Ref Clock = 80000000 Hz, Prescaler = 80, RMT Clock = 1000000.00 Hz
Fout = 500000.000 Hz => 500000.000 Hz (0.00%), Duty Cycle = 50.00% => 50.00 (0.00%)
Ntot = 2, Nhigh = 1, Nlow = 1
Nitems = 2, NChannels = 1
This sequence can be repeated 63 times + final EoTx (0,0,0,0)
Jitter due to repetition 0.7937%
[(1, 1, 1, 0), (0, 0, 0, 0)]

*/


/* ************************************************************************* */
/*                         INCLUDE HEADER SECTION                            */
/* ************************************************************************* */

// -------------------
// C standard includes
// -------------------

#include <math.h>
#include <stdio.h>

// -----------------------------------
// Expressif SDK-IDF standard includes
// -----------------------------------

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/rmt.h"

// --------------
// Local includes
// --------------

/* ************************************************************************* */
/*                      DEFINES AND ENUMERATIONS SECTION                     */
/* ************************************************************************* */

#define GPIO_5   5
#define GPIO_18 18
#define GPIO_19 19
#define GPIO_21 21

#define NO_RX_BUFFER 0
#define DEFAULT_ALLOC_FLAGS 0

#define FGEN_APB 80000000.0
#define FGEN_TAG "FGen"

#define FGEN_CHECK(a, str, ret_val) \
    if (!(a)) { \
        ESP_LOGE(FGEN_TAG,"%s(%d): %s", __FUNCTION__, __LINE__, str); \
        return (ret_val); \
    }


#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })


/* ************************************************************************* */
/*                               DATATYPES SECTION                           */
/* ************************************************************************* */

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
/*                        AUXILIAR FUNCTIONS SECTION                         */
/* ************************************************************************* */


// Find two fgen N and Prescaler so that
// FGEN_APB = Fout * (Prescaler * N)
// being Prescaler and N both integers

static 
esp_err_t fgen_find_freq(double fout, double duty_cycle, fgen_params_t* fgen)
{

    double whole; 
    double new_N;
    double err, new_err;
    double dNhigh, dNlow;

    fgen->prescaler = 255;   // Assume highest prescaler

    whole   = round(FGEN_APB/fout);
    fgen->N = whole / fgen->prescaler;
    err     = fmod(whole,  fgen->prescaler);

    while (fgen->prescaler > 1) {
        new_N     = whole / fgen->prescaler;
        new_err   = fmod(whole, fgen->prescaler);
        if (new_err == 0.0 && new_N > 1.0) {
            err     = new_err;
            fgen->N = new_N;
            break;
        } else if (new_err < err) {
            err     = new_err;
            fgen->N = new_N;
        }
        fgen->prescaler -= 1; 
    }

    if (fgen->prescaler == 2 && err != 0.0) {
        fgen->prescaler = 1;
        fgen->N         = whole;
    }

    // Now that N has been fixed, we find its High and low part
    // taking into account the duty cycle
    // If N is odd, there will be a roundoff error
    // and we will increment N by one to preserve 50% duty cycle if requested

    dNhigh = fgen->N * duty_cycle; // floating point
    dNlow  = fgen->N - dNhigh;     // still floating point

    FGEN_CHECK(dNhigh >=1.0, "High state count NH < 1", ESP_ERR_INVALID_SIZE);
    FGEN_CHECK(dNlow >= 1.0, "Low  state count NL < 1", ESP_ERR_INVALID_SIZE);

    // If Ntot is odd, we'd better round to even parts
    // and increment Ntot by one to preserve 50% duty cycle if requested
    fgen->NH = (uint32_t)(round(dNhigh));
    fgen->NL = (uint32_t)(round(dNlow));
    fgen->N  = fgen->NH + fgen->NL;   // May be changed by one unit by rounding

    return ESP_OK;
}



/* -------------------------------------------------------------------------- */

static 
uint16_t fgen_count_items(uint32_t NH, uint32_t NL)
{
    uint16_t count = 0;

    // Quick way out for only 1 item
    if (NH < 32768 && NL < 32768) {
        count += 1;  // item
        return count;
    }

    // Long high period
    while (NH > 32767*2) {
        NH   -= 32767*2;
        count += 1;
    }

    // Ending high part
    if ((32767 < NH) && (NH <= 32767*2)) {
        NH -= 32767;
        count += 1;
    } else {
        uint32_t padding = min(NL, 32767);
        NL -= padding;
        count += 1;
    }
        
    // Long low period
    while (NL > 32767*2) {
        NL   -= 32767*2;
        count += 1;
    }

    // Ending low part
    if ((32767 < NL) && (NL <= 32767*2)) {
        NL -= 32767;
        count += 1;
    } else if (NL > 0) {
        count += 1;
    }

    // The count does not include a final EoTx item
    return count;
}

/* -------------------------------------------------------------------------- */

// Fills an array of items
// Give an updated pointer so that we can mark the end of transmission
// or concatenate more items
// NO EoTX value is written, must be written outside
static 
rmt_item32_t* fgen_fill_items(rmt_item32_t* item, uint32_t NH, uint32_t NL)
{

    // Quick way out for only 1 item
    if (NH < 32768 && NL < 32768) {
        item->duration0 = NH; item->level0 = 1;
        item->duration1 = NL; item->level1 = 0;
        item++;
        return item;
    }

    // Long high period
    while (NH > 32767*2) {
        NH   -= 32767*2;
        item->duration0 = 32767; item->level0 = 1;
        item->duration1 = 32767; item->level1 = 1;
        item++;
    }

    // Ending high part
    if ((32767 < NH) && (NH <= 32767*2)) {
        NH -= 32767;
        item->duration0 = 32767; item->level0 = 1;
        item->duration1 = NH;    item->level1 = 1;
        item++;
        
    } else {
        uint32_t padding = min(NL, 32767);
        item->duration0 = NH;      item->level0 = 1;
        item->duration1 = padding; item->level1 = 0;
        item++;
        NL -= padding;
    }
        
    // Long low period
    while (NL > 32767*2) {
        NL   -= 32767*2;
        item->duration0 = 32767; item->level0 = 0;
        item->duration1 = 32767; item->level1 = 0;
        item++;
    }

    // Ending low part
    if ((32767 < NL) && (NL <= 32767*2)) {
        NL -= 32767;
        item->duration0 = 32767; item->level0 = 0;
        item->duration1 = NL;    item->level1 = 0;
        item++;
    } else if (NL > 0) {
        item->duration0 = NL; item->level0 = 0;
        item->duration1 = 0;  item->level1 = 0;
        item++;
    }
    return item;
}

/* -------------------------------------------------------------------------- */
   
static 
void fgen_print_items(const rmt_item32_t* p, uint32_t N)
{
    
    const uint32_t NN = 8;
    uint32_t rows = N / NN;
    uint32_t rem  = N % NN;
    uint32_t row, offset, i, j;

    ESP_LOGI(FGEN_TAG,"Displaying %d items + EoTx\n", N-1);
    ESP_LOGI(FGEN_TAG,"rows = %d, rem = %d", rows, rem);

    printf("-----------------------------------------------------\n");
    for (row = 0; row < rows; row++) {
        for (i=0; i<NN; i++) {
            offset = NN*row+i;
            printf("{{{%d,%d,%d,%d}}},\t", p[offset].duration0, p[offset].level0, p[offset].duration1, p[offset].level1);
        }
        printf("\n");
    }
    for (j=0; j<rem; j++) {
        offset = NN*rows+j;
        printf("{{{%d,%d,%d,%d}}},\t", p[offset].duration0, p[offset].level0, p[offset].duration1, p[offset].level1);
    }
    if (rem) printf("\n");
    printf("-----------------------------------------------------\n");
}

/* -------------------------------------------------------------------------- */

static 
void fgen_log_params(double Fout, double duty_cycle, fgen_params_t* fgen)
{
    double Tclk, ErrFreq, Errduty_cycle;

    // Recompute the Fout frequency with all that rounding taking place
    // and check the relative  error
    fgen->freq       = FGEN_APB / (fgen->prescaler * (double)(fgen->N));
    fgen->duty_cycle = fgen->NH / (double)(fgen->N);
    ErrFreq          = (fgen->freq - Fout)/Fout;
    Errduty_cycle    = (fgen->duty_cycle - duty_cycle)/duty_cycle; 
    Tclk             = (double) fgen->prescaler / FGEN_APB;

    ESP_LOGI(FGEN_TAG,"Ref Clock = %.0f Hz, Prescaler = %d, RMT Clock = %.2f Hz", FGEN_APB, fgen->prescaler, 1/Tclk);    
    ESP_LOGI(FGEN_TAG,"Ntot = %d, Nhigh = %d, Nlow = %d", fgen->N, fgen->NH, fgen->NL);
    ESP_LOGI(FGEN_TAG,"Fout = %.3f Hz => %.3f Hz (%.2f%%), Duty Cycle = %.2f%% => %.2f%% (%.2f%%)", Fout, fgen->freq, ErrFreq*100, duty_cycle*100, fgen->duty_cycle*100, Errduty_cycle*100);
}

/* -------------------------------------------------------------------------- */

static 
esp_err_t fgen_allocate(fgen_params_t* fgen)
{
    // Allocate memory and fill it with pattern
    fgen->items      = (rmt_item32_t*) calloc(fgen->nitems, sizeof(rmt_item32_t));
    FGEN_CHECK(fgen->items != NULL, "Out of memory allocating RMT items",  ESP_ERR_NO_MEM);

    // Generate the pattern and repeat it as much as we can within a 64 -item block
    rmt_item32_t* p = fgen->items;
    for(int i = 0 ; i<fgen->nrep; i++) {
        p = fgen_fill_items(p, fgen->NH, fgen->NL);
    }
    p->val = 0; // mark end of sequence
    fgen_print_items(fgen->items, fgen->nitems);
    return ESP_OK;
}

/* -------------------------------------------------------------------------- */

static 
esp_err_t fgen_config(double Fout, double duty_cycle, bool allocate, fgen_params_t* fgen)
{

    double jitter;

    // Decompose Frequency into the product of 2 factors: prescaler and N
    // Decompose N into NH and NL taking into account dyty cycle
    fgen_find_freq(Fout, duty_cycle, fgen);
    fgen_log_params(Fout, duty_cycle, fgen);
 
    // See how many RMT 32-bit items needs this frequency generation
    // How many channes does it take and how many repetitions withon a channel
    // to minimize wraparround jitter (1 Tclk delay is introduced by wraparound)

    fgen->onitems = fgen_count_items(fgen->NH, fgen->NL);  // without EoTx
    fgen->mem_blocks = (fgen->onitems > 0 ) ? 1 + (fgen->onitems / 64) : 0;
    // if only 1 memory block, make it double
    fgen->mem_blocks = (fgen->mem_blocks == 1 ) ? 2 : fgen->mem_blocks;
    
    fgen->nrep       = (fgen->mem_blocks * 63) / fgen->onitems;
    jitter = 1/((double)(fgen->N) * fgen->nrep);

    FGEN_CHECK(fgen->mem_blocks <= 8, "Fout needs more than 8 RMT channels",  ESP_ERR_INVALID_SIZE);
    ESP_LOGI(FGEN_TAG,"Nitems = %d, Mem Blocks = %d", fgen->onitems, fgen->mem_blocks);
    ESP_LOGI(FGEN_TAG,"This sequence can be duplicated %d times + final EoTx (0,0,0,0)",fgen->nrep);
    ESP_LOGI(FGEN_TAG,"Loop jitter %.02f%%", jitter*100);

    fgen->nitems     = fgen->onitems * fgen->nrep + 1; // global array size including final EoTx
    if (!allocate ) {
        fgen->items = NULL;
        return ESP_OK;
    }
    
    return fgen_allocate(fgen);
}

/* ************************************************************************* */
/*                               API FUNCTIONS                               */
/* ************************************************************************* */

/*
 * Initialize a RMT Tx channel
 */
esp_err_t fgen_init(uint8_t channel, uint8_t gpio_num, double freq, bool allocate, fgen_params_t* fparams)
{
   
    esp_err_t ret;

    ret = fgen_config(freq, 0.5, allocate, fparams);
    FGEN_CHECK(ret == ESP_OK, "Error calculating frequency generator parameters",  ret);
    if (! allocate )
        return ESP_OK;

    rmt_config_t config = {
        // Common config
        .channel              = channel,
        .rmt_mode             = RMT_MODE_TX,
        .gpio_num             = gpio_num,
        .mem_block_num        = fparams->mem_blocks,
        .clk_div              = fparams->prescaler,
        // Tx only config
        .tx_config.loop_en    = true,
        .tx_config.carrier_en = false,
        .tx_config.idle_level = RMT_IDLE_LEVEL_LOW,
        .tx_config.idle_output_en = false
    };

    ret = rmt_config(&config);
    FGEN_CHECK(ret == ESP_OK, "Error configure RMT module",  ret);

    ret = rmt_driver_install(config.channel, NO_RX_BUFFER, DEFAULT_ALLOC_FLAGS);
    FGEN_CHECK(ret == ESP_OK, "Error installing RMT driver",  ret);

    // Copy the pattern we've just generated to the internal RMT buffers
    ret = rmt_fill_tx_items(config.channel, fparams->items, fparams->nitems, 0);
    FGEN_CHECK(ret == ESP_OK, "Error copying RMT items to shared mem",  ret);

    // we no longer need the allocated memory, since we copied the sequence 
    // to the RMT buffers
    free(fparams->items);
    fparams->items = NULL;

    return ESP_OK;
}

/* -------------------------------------------------------------------------- */

esp_err_t fgen_start(uint8_t channel)
{
    return rmt_tx_start(channel, true);
}

/* ************************************************************************* */
/*                             MAIN ENTRY POINT                              */
/* ************************************************************************* */

void app_main(void *ignore)
{
    int i = 1;
    fgen_params_t fparams[4];

    ESP_LOGI(FGEN_TAG, "Configuring transmitter");
    fgen_init(RMT_CHANNEL_0, GPIO_5,  0.04,  true, &fparams[0]);
    fgen_init(RMT_CHANNEL_2, GPIO_18, 0.1,   true, &fparams[1]);
    fgen_init(RMT_CHANNEL_4, GPIO_19, 1.0,   true, &fparams[2]);
    fgen_init(RMT_CHANNEL_6, GPIO_21, 50012, true, &fparams[3]);

    ESP_ERROR_CHECK(fgen_start(RMT_CHANNEL_0));
    ESP_ERROR_CHECK(fgen_start(RMT_CHANNEL_2));
    ESP_ERROR_CHECK(fgen_start(RMT_CHANNEL_4));
    ESP_ERROR_CHECK(fgen_start(RMT_CHANNEL_6));

    while (1) {
        ESP_LOGI(FGEN_TAG, "Forever loop (%d)", i++);
        vTaskDelay(120000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}
