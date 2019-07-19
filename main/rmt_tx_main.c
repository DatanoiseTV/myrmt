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

#define RMT_TX_CHANNEL RMT_CHANNEL_0
#define RMT_TX_GPIO 18

#define NO_RX_BUFFER 0
#define DEFAULT_ALLOC_FLAGS 0

#define FREQ_APB 80000000.0
#define FREQ_TAG "Freq"

#define FREQ_CHECK(a, str, ret_val) \
    if (!(a)) { \
        ESP_LOGE(FREQ_TAG,"%s(%d): %s", __FUNCTION__, __LINE__, str); \
        return (ret_val); \
    }


#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })



typedef struct {
    double        freq;       // real frequency after adjustment
    double        duty_cycle; // duty cycle after adjustments
    rmt_item32_t* items;      // Array of items including EoTx
    uint16_t      nitems;     // number of RMT items in the array, including EoTx
    uint8_t       mem_blocks; // number of 64 item memory blocks consumed
    uint8_t       prescaler;  // prescaler value
} freq_params_t;



/* ************************************************************************* */
/*                          GLOBAL VARIABLES SECTION                         */
/* ************************************************************************* */

static const char *RMT_TX_TAG = "RMT Tx";


rmt_item32_t items[] = {
    {{{ 2, 1, 2, 0 }}},
    {{{ 2, 1, 2, 0 }}},
    {{{ 2, 1, 2, 0 }}},
    {{{ 2, 1, 2, 0 }}},
    {{{ 2, 1, 2, 0 }}},
    {{{ 2, 1, 2, 0 }}},
    {{{ 2, 1, 2, 0 }}},
    {{{ 2, 1, 2, 0 }}},
    {{{ 2, 1, 2, 0 }}},
    // RMT end marker
    {{{ 0, 0, 0, 0 }}}
};


/* ************************************************************************* */
/*                        AUXILIAR FUNCTIONS SECTION                         */
/* ************************************************************************* */

static 
rmt_item32_t* freq_items_alloc(uint16_t nitems)
{
    return (rmt_item32_t*) calloc(nitems, sizeof(rmt_item32_t));
}

/* -------------------------------------------------------------------------- */

// Find two divisors N and Prescaler so that
// FREQ_APB = Fout * (Prescaler * N)
// being Prescaler and N both integers

typedef struct {
    uint32_t N;         // Big number to decompose in items
    uint8_t  prescaler; // RMT prescaler
} freq_divisors_t;

static 
void freq_find_divisors(const double fout, freq_divisors_t* divisors)
{

    double whole; 
    double N, new_N;
    double err, new_err;

    divisors->prescaler = 255;   // Assume highest prescaler

    whole = round(FREQ_APB/fout);
    divisors->N     = whole / divisors->prescaler;
    err   = fmod(whole,  divisors->prescaler);

    while (divisors->prescaler > 1) {
        new_N     = whole / divisors->prescaler;
        new_err   = fmod(whole, divisors->prescaler);
        if (new_err == 0.0 && new_N > 1.0) {
            err = new_err;
            divisors->N   = new_N;
            break;
        } else if (new_err < err) {
            err = new_err;
            divisors->N   = new_N;
        }
        divisors->prescaler -= 1; 
    }

    if (divisors->prescaler == 2 && err != 0.0) {
        divisors->prescaler = 1;
        divisors->N = whole;
    }
}

/* -------------------------------------------------------------------------- */

static 
uint16_t freq_count_items(uint32_t NH, uint32_t NL)
{
    uint16_t count = 0;

    // Quick way out
    if (NH < 32768 && NL < 32768) {
        count += 2;  // item + End of transmission
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

    // The count includes a final EoTx item
    count += 1;     
    return count;
}

/* -------------------------------------------------------------------------- */

// Fills an array of items
// Give an updated pointer so that we can mark the end of transmission
// or concatenate more items
// NO EoTX value is written, must be written outside
static rmt_item32_t* freq_fill_items(rmt_item32_t* item, uint32_t NH, uint32_t NL)
{

    // Quick way out
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
        item->duration1 = NH; item->level1 = 1;
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
void freq_print_items(const rmt_item32_t* p, uint32_t N)
{
    for (int i=0; i<N; i++) {
        printf("(%d,%d,%d,%d)\t", p[i].duration0, p[i].level0, p[i].duration1, p[i].level1);
    }
}

/* -------------------------------------------------------------------------- */

static 
esp_err_t freq_calc_params(freq_params_t* fp, double Fout, double Dcyc)
{

    double Tclk, dNhigh, dNlow;
    double Fout2, Dcyc2, ErrFreq, ErrDcyc;
    double jitter;

    uint32_t Ntot, Nhigh, Nlow;
    uint16_t Nitems;
    uint8_t  Prescaler, Nchan, Nrep;

    // Decompose Frequency into the product of 2 factors: prescaler and Ntot
    // Decompose the duty cycle into and Nhigh and Nlow numbers
    Ntot = freq_find_divisors(Fout, &Prescaler);
    Tclk = (double) Prescaler / FREQ_APB;

    dNhigh = Ntot * Dcyc;       // still floating point
    dNlow  = Ntot - dNhigh;     // still floating point

    // If Ntot is odd, we'd better round to even parts
    // and increment Ntot by one to preserve 50% duty cycle if requested
    Nhigh = (uint32_t)(round(dNhigh));
    Nlow  = (uint32_t)(round(dNlow));
    Ntot  = Nhigh + Nlow;   // May be changed by one unit by rounding
    
    // Recompute the Fout frequency with all that rounding taking place
    // and check the relative  error
    Fout2 = FREQ_APB / (Prescaler * (double)(Ntot));
    Dcyc2 = Nhigh    / (double)(Ntot);
    ErrFreq = (Fout2 - Fout)/Fout;
    ErrDcyc  = (Dcyc2 - Dcyc)/Dcyc; 

    FREQ_CHECK(dNhigh >=1.0, "High state count is < 1", ESP_ERR_INVALID_SIZE);
    FREQ_CHECK(dNlow >=1.0,  "Low state count is < 1",  ESP_ERR_INVALID_SIZE);
 
    // See how many RMT 32-bit items needs this frequency generation
    // How many channes does it take and how many repetitions withon a channel
    // to minimize wraparround jitter (1 Tclk delay is introduced by wraparound)

    Nitems = freq_count_items(Nhigh, Nlow);
    Nchan = (Nitems > 0 ) ? 1 + (Nitems / 64) : 0;
    Nrep = Nchan * 63 / (Nitems-1);
    jitter = 1/((double)(Ntot) * Nrep);

    FREQ_CHECK(Nchan <= 8, "Fout needs more than 8 RMT channels",  ESP_ERR_INVALID_SIZE);
   
    ESP_LOGI(FREQ_TAG,"Ref Clock = %.0f Hz, Prescaler = %d, RMT Clock = %.2f Hz", FREQ_APB, Prescaler, 1/Tclk);    
    ESP_LOGI(FREQ_TAG,"Ntot = %d, Nhigh = %d, Nlow = %d", Ntot, Nhigh, Nlow);
    ESP_LOGI(FREQ_TAG,"Fout = %.3f Hz => %.3f Hz (%.2f%%), Duty Cycle = %.2f%% => %.2f%% (%.2f%%)", Fout, Fout2, ErrFreq*100, Dcyc*100, Dcyc2*100, ErrDcyc*100);
    ESP_LOGI(FREQ_TAG,"Nitems = %d, NChannels = %d", Nitems, Nchan);
    ESP_LOGI(FREQ_TAG,"This sequence can be repeated %d times + final EoTx (0,0,0,0)",Nrep);
    ESP_LOGI(FREQ_TAG,"Jitter due to repetition %.02f%%",jitter*100);

    fp->freq       = Fout2;
    fp->duty_cycle = Dcyc2;
    fp->prescaler  = Prescaler;
    fp->mem_blocks = Nchan;
    fp->nitems     = (Nitems - 1)*Nrep + 1; // include final EoTx
    fp->items      = freq_items_alloc(fp->nitems);
    FREQ_CHECK(fp->items != NULL, "Out of memory allocating RMT items",  ESP_ERR_NO_MEM);
    rmt_item32_t* p = fp->items;
    for(int i = 0 ; i<Nrep; i++) {
        p = freq_fill_items(p, Nhigh, Nlow);
    }
    p->val = 0; // mark end of sequence

    freq_print_items(fp->items, fp->nitems);

    return ESP_OK;
}


/* -------------------------------------------------------------------------- */

/*
 * Initialize the RMT Tx channel
 */
static void rmt_tx_int()
{
   
#if 1
    rmt_config_t config = {
        // Common config
        .channel              = RMT_TX_CHANNEL,
        .rmt_mode             = RMT_MODE_TX,
        .gpio_num             = RMT_TX_GPIO,
        .mem_block_num        = 1,
        .clk_div              = 200,
        // Tx only config
        .tx_config.loop_en    = true,
        .tx_config.carrier_en = false
    };

    ESP_ERROR_CHECK(rmt_config(&config));
    ESP_ERROR_CHECK(rmt_driver_install(config.channel, NO_RX_BUFFER, DEFAULT_ALLOC_FLAGS));
    ESP_ERROR_CHECK(rmt_fill_tx_items(RMT_TX_CHANNEL, items, sizeof(items) / sizeof(items[0]), 0));
#else
    freq_params_t fparams;

    freq_calc_params(&fparams, 5, 0.5);

    rmt_config_t config = {
        // Common config
        .channel              = RMT_TX_CHANNEL,
        .rmt_mode             = RMT_MODE_TX,
        .gpio_num             = RMT_TX_GPIO,
        .mem_block_num        = fparams.mem_blocks,
        .clk_div              = fparams.prescaler,
        // Tx only config
        .tx_config.loop_en    = true,
        .tx_config.carrier_en = false
    };

    printf("-----------------------------------------------------\n");
    for(int i = 0; i < sizeof(items) / sizeof(items[0]); i++ ) {
        printf("(%d,%d,%d,%d)\t", items[i].duration0, items[i].level0, items[i].duration1, items[i].level1);
    }
    printf("-----------------------------------------------------\n");
    ESP_ERROR_CHECK(rmt_config(&config));
    ESP_ERROR_CHECK(rmt_driver_install(config.channel, NO_RX_BUFFER, DEFAULT_ALLOC_FLAGS));
    ESP_ERROR_CHECK(rmt_fill_tx_items(RMT_TX_CHANNEL, fparams.items, fparams.nitems, 0));
#endif


#if 0
    freq_calc_params(0.01,    0.75);
    freq_calc_params(0.01,    0.5);
    freq_calc_params(0.02,    0.5);
    freq_calc_params(0.02,    0.1);
    freq_calc_params(0.025,   0.5);
    freq_calc_params(0.05,    0.75);
    freq_calc_params(0.05,    0.5);
    freq_calc_params(0.1,     0.5);
    freq_calc_params(0.1,     0.63);
    freq_calc_params(0.1,     0.005);
    freq_calc_params(0.1,     1-0.005);
    freq_calc_params(5,       0.5);
    freq_calc_params(5,       1-0.005);
    freq_calc_params(5,       0.5);
    freq_calc_params(509,     0.5);
    freq_calc_params(6473,    0.5);
    freq_calc_params(43247,   0.7);
    freq_calc_params(50000,   0.7);
    freq_calc_params(70729.0,  0.5);
    freq_calc_params(100000.0, 0.5);
    freq_calc_params(500000.0, 0.5);
#endif

}


/* ************************************************************************* */
/*                             MAIN ENTRY POINT                              */
/* ************************************************************************* */

void app_main(void *ignore)
{
    int i = 1;

#if 0
    ESP_LOGI(RMT_TX_TAG, "Configuring transmitter");
    rmt_tx_int();
    int number_of_items = sizeof(items) / sizeof(items[0]);
    ESP_LOGI(RMT_TX_TAG, "Number of items = %d",number_of_items);
    //ESP_ERROR_CHECK(rmt_write_items(RMT_TX_CHANNEL, items, number_of_items, false));
    ESP_ERROR_CHECK(rmt_fill_tx_items(RMT_TX_CHANNEL, items, number_of_items, 0));
    ESP_ERROR_CHECK(rmt_tx_start(RMT_TX_CHANNEL, true));
#else
    ESP_LOGI(RMT_TX_TAG, "Configuring transmitter");
    rmt_tx_int();
    int number_of_items = sizeof(items) / sizeof(items[0]);
    ESP_LOGI(RMT_TX_TAG, "Number of items = %d",number_of_items);
    //ESP_ERROR_CHECK(rmt_write_items(RMT_TX_CHANNEL, items, number_of_items, false));
    ESP_ERROR_CHECK(rmt_fill_tx_items(RMT_TX_CHANNEL, items, number_of_items, 0));
    ESP_ERROR_CHECK(rmt_tx_start(RMT_TX_CHANNEL, true));

#endif    

    while (1) {
        ESP_LOGI(RMT_TX_TAG, "Forever loop (%d)", i++);
        vTaskDelay(120000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}
