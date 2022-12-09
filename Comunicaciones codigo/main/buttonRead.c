#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "buttonRead.h"

#define DEFAULT_VREF 1100 // Use adc1_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES 64  // Multisampling

static esp_adc_cal_characteristics_t *adc_chars;

static const adc_channel_t channel = ADC_CHANNEL_6; // GPIO34 if ADC1, GPIO14 if ADC2
static const adc_bits_width_t width = ADC_WIDTH_BIT_12;

static const adc_atten_t atten = ADC_ATTEN_DB_2_5;
static const adc_unit_t unit = ADC_UNIT_1;

uint8_t listenButtons(void) // esta función no retorna hasta que haya algo diferente a NoPressed
{
    // Configure ADC
    uint8_t buttonPressed = 0;
    // Configure ADC aproximaciones sucesivas

    adc1_config_width(width);
    adc1_config_channel_atten(channel, atten);
    // Characterize ADC
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, width, DEFAULT_VREF, adc_chars);

    // Continuously sample ADC1
    uint32_t adc_reading = 0;

    uint32_t TempApagado = 0; // temporizador pantalla apagada

    int raw=adc1_get_raw((adc1_channel_t)channel); // calcular valor inicial de RAW
    // Multisampling
    while (TempApagado <= 3000)                             // Detecte la interacción o cuando han pasado 30s sin interacción
    {                                                       // this way highers values won't affect future adc_readings averages.
        raw=adc1_get_raw((adc1_channel_t)channel);// Calcular continuamente el valor de RAW
        //printf("%d",raw);
        if (raw < 4095)
        {
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(10));
        TempApagado += 2;
    }
    if (TempApagado >= 2999)
    {
        buttonPressed = ScreenOffFlag;
        printf("offFlag\n");
    }
    else
    {
        for (int i = 0; i < NO_OF_SAMPLES; i++)
        {
           adc_reading += adc1_get_raw((adc1_channel_t)channel);
        }
        adc_reading /= NO_OF_SAMPLES;

        printf("adc_reading: %d\n", adc_reading); // filtering each probabilitie.
        if (adc_reading > 4094)
        {
            buttonPressed = NoPressed;
            printf("BotonNoPresionado\n");
        }
        else if (adc_reading > 350 && adc_reading < 950)
        {
            buttonPressed = ButtonUP;
            printf("Button UP Pressed\n");
        }
        else if (adc_reading >= 0 && adc_reading < 20)
        {
            buttonPressed = ButtonRIGHT;
            printf("Button RIGHT Pressed\n");
        }
        else if (adc_reading > 20 && adc_reading < 350)
        {
            buttonPressed = ButtonDOWN;
            printf("Button DOWN Pressed\n");
        }
        else if (adc_reading > 950 && adc_reading < 2500)
        {
            buttonPressed = ButtonLEFT;
            printf("Button LEFT Pressed\n");
        }
        else if (adc_reading > 2500 && adc_reading < 3800)
        {
            buttonPressed = ButtonOK;
            printf("Button Ok Pressed\n");
        }
        else
        {
            printf("something strange is happening XD");
        }
    }
    return buttonPressed;
}

