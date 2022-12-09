#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"

#include "ssd1306.h"
#include "font8x8_basic.h"
#include "buttonRead.h"
#include "Display.h"
#include "main.h"

#define tag "SSD1306"

SSD1306_t dev;
uint8_t buttonPressed = 0; // Aquí se almacena el boton presionado
bool customFlag = 0;       // En la interfaz de validación para saber si se devuelve a custom o default.
int count = 0;             // para pruebas
bool ledLevel = 1;         // para los gpios de led y de buzzer
bool buzzerLevel = 0;

ParamID paramID = TEMPER;
uint8_t subPantalla = 0; // subpantallas
uint8_t listen = 0;      // aux

AlarmMessage alarmMessageCom;
bool alarmFlagCom = false;
void SetComun(void)
{ // initializing the i2c.
    ESP_LOGI(tag, "INTERFACE is i2c");
    ESP_LOGI(tag, "CONFIG_SDA_GPIO=%d", CONFIG_SDA_GPIO);
    ESP_LOGI(tag, "CONFIG_SCL_GPIO=%d", CONFIG_SCL_GPIO);
    ESP_LOGI(tag, "CONFIG_RESET_GPIO=%d", CONFIG_RESET_GPIO);
    i2c_master_init(&dev, CONFIG_SDA_GPIO, CONFIG_SCL_GPIO, CONFIG_RESET_GPIO);

    // configuring size of the panel
    ESP_LOGI(tag, "Panel is 128x64");
    ssd1306_init(&dev, 128, 64);
    // configurar GPIOS
    gpio_reset_pin(buzzer);
    gpio_reset_pin(ledVerde);
    gpio_set_direction(buzzer, GPIO_MODE_OUTPUT);
    gpio_set_direction(ledVerde, GPIO_MODE_OUTPUT);

    // mensaje de alerta del grupo de comunicaciones
    strcpy(alarmMessageCom.line0, "---------------");
    strcpy(alarmMessageCom.line1, "Se necesita    ");
    strcpy(alarmMessageCom.line2, "registra       ");
    strcpy(alarmMessageCom.line3, "parametros para");
    strcpy(alarmMessageCom.line4, "inicar         ");
    strcpy(alarmMessageCom.line5, "---------------");
    strcpy(alarmMessageCom.line6, "---------------");
    strcpy(alarmMessageCom.line7, "   EN ESPERA   ");
}
void UserInterface(Screens *screenSelected, Parameters *parameters, Parameters *BufferForParameters, Parameters *MainLog, Parameters *defaultPlant1, Parameters *defaultPlant2, Parameters *defaultPlant3, Parameters *RangoMaxUiCustom, Parameters *RangoMinUiCustom)
{

    UiClear();
    alarmFlagCom=false;
    Alarm(alarmMessageCom, alarmFlagCom);//basicamente esto hace que se prenda el led y ya XD

    while (1)
    {
        // verifica que la alarma siga en false, si es true la activa y luego la apaga.
        printf("screenSelected: %d \n", *screenSelected);
        switch (*screenSelected) // tuve que hacer un switch porque con recursividad se corrompia la memoria >stack overflow<
        {
        case S_WellcomeUi:
            *screenSelected = WellcomeUi(); // interfaz de inicio
            break;
        case S_Ui_Init:
            *screenSelected = Ui_Init(&subPantalla);
            break;
        case S_Ui_Config:
            *screenSelected = Ui_Config(&subPantalla);
            break;
        case S_Ui_Monitor:
            *screenSelected = Ui_Monitor(&subPantalla, MainLog);
            break;
        case S_Ui_Defaults:
            *screenSelected = Ui_Defaults(&subPantalla, BufferForParameters, defaultPlant1, defaultPlant2, defaultPlant3);
            break;
        case S_Ui_Customs:
            *screenSelected = Ui_Customs(&subPantalla, &paramID, BufferForParameters, RangoMaxUiCustom, RangoMinUiCustom);
            break;
        case S_Ui_Validation:
            *screenSelected = Ui_Validation(&subPantalla, &paramID, BufferForParameters, parameters);
            break;
        case S_OFF:
            UiClear(); // screen off case.

            if (parameters->temperatura == 0)
            {
                alarmFlagCom = true; // da la alarma de que no hay nada registrado
               
            }else {
                alarmFlagCom = false;
            }
            Alarm(alarmMessageCom, alarmFlagCom);

            listen = listenButtons();

            if (listen != NoPressed && listen != ScreenOffFlag)
            {
                *screenSelected = S_Ui_Init;
            }
            break;
        default:
            printf("Error al mostrar las pantallas");
        }
        // UiParamasProof(16, 45, 45, 20);
    }
}

uint8_t UiParamasProof(float temperatura, float humedad, float humedadSuelo, float T_iluminacion)
{
    ssd1306_clear_screen(&dev, false);

    // Setting the contrast
    ssd1306_contrast(&dev, 0xff);
    // Logo
    ssd1306_display_text(&dev, 1, "->Temperatura(c)", 16, false);
    ssd1306_display_text(&dev, 5, "             ok ", 16, true);
    ssd1306_display_text(&dev, 7, "            back", 16, true);
    ssd1306_display_text(&dev, 7, "       ", 6, false);
    ssd1306_display_text_x3(&dev, 5, "   -", 4, false);
    ssd1306_display_text_x3(&dev, 5, " -", 2, false);
    ssd1306_display_text_x3(&dev, 5, "-", 1, false);

    ssd1306_display_text_x3(&dev, 3, "20.2", 4, false);

    vTaskDelay(3000 / portTICK_PERIOD_MS);

    return 0;
}
void UiClear()
{
    ssd1306_clear_screen(&dev, false);
}
uint8_t WellcomeUi() //
{

    ssd1306_clear_screen(&dev, false);

    // Setting the contrast
    ssd1306_contrast(&dev, 0xff);
    // Logo
    ssd1306_display_text_x3(&dev, 1, "UNAL", 4, false);
    ssd1306_display_text_x3(&dev, 4, "2022", 4, false);
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    // Intro Scroll
    ssd1306_clear_screen(&dev, false);
    ssd1306_contrast(&dev, 0xff);
    ssd1306_display_text(&dev, 1, "   Diseno de    ", 16, false);
    ssd1306_display_text(&dev, 2, "    sistemas    ", 16, false);
    ssd1306_display_text(&dev, 3, "  electronicos  ", 16, false);
    ssd1306_display_text(&dev, 5, "   Gabinete de  ", 16, true);
    ssd1306_display_text(&dev, 6, "   autocultivo  ", 16, true);
    ssd1306_hardware_scroll(&dev, SCROLL_RIGHT);
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    ssd1306_hardware_scroll(&dev, SCROLL_STOP);
    return S_Ui_Init;
}
uint8_t Ui_Init(uint8_t *subPantalla) //
{
    // Interface
    uint8_t siguienteInterfaz = 0;

    char bufferLine1[17];
    char bufferLine2[17];
    bool highlightLine1;
    bool highlightLine2;

    // configurando subpantallas
    if (*subPantalla == 0)
    {
        strcpy(bufferLine1, "Configurar    <-");
        strcpy(bufferLine2, "Monitorear      ");
        highlightLine1 = true;
        highlightLine2 = false;
    }
    else
    {
        strcpy(bufferLine1, "Configurar      ");
        strcpy(bufferLine2, "Monitorear    <-");
        highlightLine1 = false;
        highlightLine2 = true;
    }

    // clear and setting the contrast

    ssd1306_clear_screen(&dev, false);
    ssd1306_contrast(&dev, 0xff);

    // Lo que se imprime
    ssd1306_display_text(&dev, 2, bufferLine1, 16, highlightLine1);
    ssd1306_display_text(&dev, 3, bufferLine2, 16, highlightLine2);
    ssd1306_display_text(&dev, 6, "     para seguir", 16, false);
    ssd1306_display_text(&dev, 6, "<Ok>", 4, true);

    // Escuchar los botones para ir a la siguiente pantalla
    while (1)
    {
        buttonPressed = listenButtons(); // función para escuchar botones
        vTaskDelay(pdMS_TO_TICKS(200));  // retraso para que no solo se refleje 1 boton presionado

        // IF para saber cuando se presionó un boton
        if ((buttonPressed == ButtonDOWN || buttonPressed == ButtonOK) && *subPantalla == 0)
        {                                    // botones validos pantalla 1
            if (buttonPressed == ButtonDOWN) // para que se pueda bajar, tiene que estar en la subpantalla 0  ("configurar<-")
            {
                *subPantalla = 1;
                siguienteInterfaz = S_Ui_Init; // haría que cambie a la misma pantalla , pero con la subpantalla 1.
            }
            else
            {
                siguienteInterfaz = S_Ui_Config;
            }
            break;
        }
        else if ((buttonPressed == ButtonUP || buttonPressed == ButtonOK) && *subPantalla == 1)
        { // botones validos pantalla 1
            if (buttonPressed == ButtonUP)
            {
                *subPantalla = 0;
                siguienteInterfaz = S_Ui_Init; // haría que cambie a la misma pantalla , pero con la subpantalla 0.
            }
            else
            {
                *subPantalla = 0;
                siguienteInterfaz = S_Ui_Monitor;
            }
            break;
        }
        else if (buttonPressed == ScreenOffFlag)
        {
            siguienteInterfaz = S_OFF; // opción para cuando el offFlag
            break;
        }
        else
        {
            continue;
        }
    }
    return siguienteInterfaz;
}
uint8_t Ui_Monitor(uint8_t *subPantalla, Parameters *MainLog)
{
    uint8_t siguienteInterfaz = 0;
    // Como se pueden ocupar hasta 18 bytes, el compilador bota un error si no se pone 18.
    char buffer1[18];
    char buffer2[18];
    char buffer3[18];
    char buffer4[18];
    // concatenando el string a mostrar
    sprintf(buffer1, "%s%.1f%c", "Grados    =", MainLog->temperatura, 'C');
    sprintf(buffer2, "%s%.1f%c", "Humedad   =", MainLog->humedad, '%');
    sprintf(buffer3, "%s%.1f%c", "Humedad.S =", MainLog->humedadSuelo, '%');
    sprintf(buffer4, "%s%.1f%c", "t. de luz =", MainLog->T_iluminacion, 'H');
    // configurando subpantallas.

    bool highlightLine1;
    bool highlightLine2;
    if (*subPantalla == 0)
    {
        highlightLine1 = false;
        highlightLine2 = true;
    }
    else
    {
        highlightLine1 = true;
        highlightLine2 = false;
    }
    ssd1306_clear_screen(&dev, false);
    ssd1306_display_text(&dev, 1, buffer1, 16, false);
    ssd1306_display_text(&dev, 2, buffer2, 16, false);
    ssd1306_display_text(&dev, 3, buffer3, 16, false);
    ssd1306_display_text(&dev, 4, buffer4, 16, false);

    ssd1306_display_text(&dev, 6, "           Back ", 16, highlightLine1);
    ssd1306_display_text(&dev, 6, " Reload ", 8, highlightLine2);

    // Listen buttons to know what to do next
    while (1)
    {
        buttonPressed = listenButtons(); // función para escuchar botones
        vTaskDelay(pdMS_TO_TICKS(200));  // retraso para que no solo se refleje 1 boton presionado

        // IF para saber cuando se presionó un boton
        if ((buttonPressed == ButtonOK || buttonPressed == ButtonRIGHT) && *subPantalla == 0)
        {                                     // botones validos pantalla 1
            if (buttonPressed == ButtonRIGHT) // para que se pueda bajar, tiene que estar en la subpantalla 0  ("configurar<-")
            {
                *subPantalla = 1;
                siguienteInterfaz = S_Ui_Monitor;
            }
            else
            {
                *subPantalla = 0;
                siguienteInterfaz = S_Ui_Monitor;
            }
            break;
        }
        else if ((buttonPressed == ButtonOK || buttonPressed == ButtonLEFT) && *subPantalla == 1)
        { // botones validos pantalla 1
            if (buttonPressed == ButtonLEFT)
            {
                *subPantalla = 0;
                siguienteInterfaz = S_Ui_Monitor; // haría que cambie a la misma pantalla , pero con la subpantalla 1.
            }
            else
            {
                *subPantalla = 0;
                siguienteInterfaz = S_Ui_Init;
            }
            break;
        }
        else if (buttonPressed == ScreenOffFlag)
        {
            siguienteInterfaz = S_OFF; // opción para cuando el offFlag
            break;
        }
        else
        {
            continue;
        }
    }
    return siguienteInterfaz;
}
uint8_t Ui_Config(uint8_t *subPantalla) //
{
    // Interface
    uint8_t siguienteInterfaz = 0;

    char bufferLine1[17];
    char bufferLine2[17];
    bool highlightLine1;
    bool highlightLine2;
    bool highlightLine3;

    // configurando subpantallas
    if (*subPantalla == 0)
    {
        strcpy(bufferLine1, "Predeterminado<-");
        strcpy(bufferLine2, "Personalizado   ");
        highlightLine1 = true;
        highlightLine2 = false;
        highlightLine3 = false;
    }
    else if (*subPantalla == 1)
    {
        strcpy(bufferLine1, "Predeterminado  ");
        strcpy(bufferLine2, "Personalizado <-");
        highlightLine1 = false;
        highlightLine2 = true;
        highlightLine3 = false;
    }
    else
    {
        strcpy(bufferLine1, "Predeterminado  ");
        strcpy(bufferLine2, "Personalizado   ");
        highlightLine1 = false;
        highlightLine2 = false;
        highlightLine3 = true;
    }
    // Displaying
    ssd1306_clear_screen(&dev, false);
    ssd1306_display_text(&dev, 2, bufferLine1, 16, highlightLine1);
    ssd1306_display_text(&dev, 3, bufferLine2, 16, highlightLine2);
    ssd1306_display_text(&dev, 6, "           Back ", 16, highlightLine3);

    // Listen buttons to know what to do next
    while (1)
    {
        buttonPressed = listenButtons(); // función para escuchar botones
        vTaskDelay(pdMS_TO_TICKS(200));  // retraso para que no solo se refleje 1 boton presionado

        // IF para saber cuando se presionó un boton
        if ((buttonPressed == ButtonDOWN || buttonPressed == ButtonOK) && *subPantalla == 0)
        {                                    // botones validos pantalla 0
            if (buttonPressed == ButtonDOWN) // para que se pueda bajar, tiene que estar en la subpantalla 0  ("configurar<-")
            {
                *subPantalla = 1;
                siguienteInterfaz = S_Ui_Config; // haría que cambie a la misma pantalla , pero con la subpantalla 1.
            }
            else
            {
                siguienteInterfaz = S_Ui_Defaults;
            }
            break;
        }
        else if ((buttonPressed == ButtonUP || buttonPressed == ButtonDOWN || buttonPressed == ButtonOK) && *subPantalla == 1)
        { // botones validos pantalla 1
            if (buttonPressed == ButtonUP)
            {
                *subPantalla = 0;
                siguienteInterfaz = S_Ui_Config; // haría que cambie a la misma pantalla , pero con la subpantalla 0.
            }
            else if (buttonPressed == ButtonDOWN)
            {
                *subPantalla = 2;
                siguienteInterfaz = S_Ui_Config;
            }
            else
            {
                *subPantalla = 0;
                siguienteInterfaz = S_Ui_Customs;
            }
            break;
        }
        else if ((buttonPressed == ButtonUP || buttonPressed == ButtonOK) && *subPantalla == 2)
        { // botones validos pantalla2
            if (buttonPressed == ButtonUP)
            {
                *subPantalla = 1;
                siguienteInterfaz = S_Ui_Config; // haría que cambie a la misma pantalla , pero con la subpantalla 0.
            }
            else
            {
                *subPantalla = 0;
                siguienteInterfaz = S_Ui_Init;
            }
            break;
        }
        else if (buttonPressed == ScreenOffFlag)
        {
            siguienteInterfaz = S_OFF; // opción para cuando el offFlag
            break;
        }
        else
        {
            continue;
        }
    }

    return siguienteInterfaz;
}
uint8_t Ui_Defaults(uint8_t *subPantalla, Parameters *BufferForParameters, Parameters *defaultPlant1, Parameters *defaultPlant2, Parameters *defaultPlant3)
{
    {
        customFlag = 0;
        // Interface
        uint8_t siguienteInterfaz = 0;

        char bufferLine1[17];
        char bufferLine2[17];
        char bufferLine3[17];
        bool highlightLine1;
        bool highlightLine2;
        bool highlightLine3;
        bool highlightLine4;

        // configurando subpantallas
        if (*subPantalla == 0)
        {
            strcpy(bufferLine1, "  Planta 1    <-");
            strcpy(bufferLine2, "  Planta 2      ");
            strcpy(bufferLine3, "  Planta 3      ");
            highlightLine1 = true;
            highlightLine2 = false;
            highlightLine3 = false;
            highlightLine4 = false;
        }
        else if (*subPantalla == 1)
        {
            strcpy(bufferLine1, "  Planta 1      ");
            strcpy(bufferLine2, "  Planta 2    <-");
            strcpy(bufferLine3, "  Planta 3      ");
            highlightLine1 = false;
            highlightLine2 = true;
            highlightLine3 = false;
            highlightLine4 = false;
        }
        else if (*subPantalla == 2)
        {
            strcpy(bufferLine1, "  Planta 1      ");
            strcpy(bufferLine2, "  Planta 2      ");
            strcpy(bufferLine3, "  Planta 3    <-");
            highlightLine1 = false;
            highlightLine2 = false;
            highlightLine3 = true;
            highlightLine4 = false;
        }
        else
        {
            strcpy(bufferLine1, "  Planta 1      ");
            strcpy(bufferLine2, "  Planta 2      ");
            strcpy(bufferLine3, "  Planta 3      ");
            highlightLine1 = false;
            highlightLine2 = false;
            highlightLine3 = false;
            highlightLine4 = true;
        }
        // Displaying
        ssd1306_clear_screen(&dev, false);
        ssd1306_display_text(&dev, 2, bufferLine1, 16, highlightLine1);
        ssd1306_display_text(&dev, 3, bufferLine2, 16, highlightLine2);
        ssd1306_display_text(&dev, 4, bufferLine3, 16, highlightLine3);
        ssd1306_display_text(&dev, 6, "           Back ", 16, highlightLine4);

        // Listen buttons to know what to do next
        while (1)
        {
            buttonPressed = listenButtons(); // función para escuchar botones
            vTaskDelay(pdMS_TO_TICKS(200));  // retraso para que no solo se refleje 1 boton presionado

            // IF para saber cuando se presionó un boton
            if ((buttonPressed == ButtonDOWN || buttonPressed == ButtonOK) && *subPantalla == 0)
            {                                    // botones validos pantalla 0
                if (buttonPressed == ButtonDOWN) // para que se pueda bajar, tiene que estar en la subpantalla 0  ("configurar<-")
                {
                    *subPantalla = 1;
                    siguienteInterfaz = S_Ui_Defaults; // haría que cambie a la misma pantalla , pero con la subpantalla 1.
                }
                else
                {
                    *BufferForParameters = *defaultPlant1; // assignación del buffer para la siguiente pantalla
                    siguienteInterfaz = S_Ui_Validation;   // pasa a validar
                }
                break;
            }
            else if ((buttonPressed != ButtonLEFT && buttonPressed != ButtonRIGHT) && *subPantalla == 1)
            { // botones validos pantalla 1
                if (buttonPressed == ButtonUP)
                {
                    *subPantalla = 0;
                    siguienteInterfaz = S_Ui_Defaults; // haría que cambie a la misma pantalla , pero con la subpantalla 0.
                }
                else if (buttonPressed == ButtonDOWN)
                {
                    *subPantalla = 2;
                    siguienteInterfaz = S_Ui_Defaults;
                }
                else
                {
                    *subPantalla = 0;
                    *BufferForParameters = *defaultPlant2; // assignación del buffer para la siguiente pantalla
                    siguienteInterfaz = S_Ui_Validation;   // pasa a validar
                }
                break;
            }
            else if ((buttonPressed != ButtonLEFT && buttonPressed != ButtonRIGHT) && *subPantalla == 2)
            { // botones validos pantalla 2
                if (buttonPressed == ButtonUP)
                {
                    *subPantalla = 1;
                    siguienteInterfaz = S_Ui_Defaults; // haría que cambie a la misma pantalla , pero con la subpantalla 0.
                }
                else if (buttonPressed == ButtonDOWN)
                {
                    *subPantalla = 3;
                    siguienteInterfaz = S_Ui_Defaults;
                }
                else
                {
                    *subPantalla = 0;
                    *BufferForParameters = *defaultPlant3; // assignación del buffer para la siguiente pantalla
                    siguienteInterfaz = S_Ui_Validation;   // pasa a validar
                }
                break;
            }
            else if ((buttonPressed == ButtonUP || buttonPressed == ButtonOK) && *subPantalla == 3)
            { // botones validos pantalla3
                if (buttonPressed == ButtonUP)
                {
                    *subPantalla = 2;
                    siguienteInterfaz = S_Ui_Defaults; // haría que cambie a la misma pantalla , pero con la subpantalla 0.
                }
                else
                {
                    *subPantalla = 0;
                    siguienteInterfaz = S_Ui_Config;
                }
                break;
            }
            else if (buttonPressed == ScreenOffFlag)
            {
                siguienteInterfaz = S_OFF; // opción para cuando el offFlag
                break;
            }
            else
            {
                continue;
            }
        }

        return siguienteInterfaz;
    }
}
uint8_t Ui_Customs(uint8_t *subPantalla, ParamID *paramID, Parameters *BufferForParameters, Parameters *RangoMaxUiCustom, Parameters *RangoMinUiCustom)
{
    customFlag = 1;
    uint8_t siguienteInterfaz = 0;
    char bufferline1[17];
    char bufferline2[5];
    char buffer1[5]; // buffers locales
    char buffer2[5];
    char buffer3[5];
    char buffer4[5];
    // concatenando el string a mostrar
    if (BufferForParameters->temperatura > 10)
    {
        sprintf(buffer1, "%.1f", BufferForParameters->temperatura);
    }
    else
    { // cuando es menor a 10 mostrar 01,02,03...etc
        sprintf(buffer1, "%d%.1f", 0, BufferForParameters->temperatura);
    }

    if (BufferForParameters->humedad > 10)
    {
        sprintf(buffer2, "%.1f", BufferForParameters->humedad);
    }
    else
    { // cuando es menor a 10 mostrar 01,02,03...etc
        sprintf(buffer2, "%d%.1f", 0, BufferForParameters->humedad);
    }
    if (BufferForParameters->humedadSuelo > 10)
    {
        sprintf(buffer3, "%.1f", BufferForParameters->humedadSuelo);
    }
    else
    { // cuando es menor a 10 mostrar 01,02,03...etc
        sprintf(buffer3, "%d%.1f", 0, BufferForParameters->humedadSuelo);
    }
    if (BufferForParameters->T_iluminacion > 10)
    {
        sprintf(buffer4, "%.1f", BufferForParameters->T_iluminacion);
    }
    else
    { // cuando es menor a 10 mostrar 01,02,03...etc
        sprintf(buffer4, "%d%.1f", 0, BufferForParameters->T_iluminacion);
    }

    // configurando subpantallas (un montoooon)

    // Que parametros se va a modificar y mostra en pantalla
    switch (*paramID)
    {
    case TEMPER:
        strcpy(bufferline1, "-Temperatura(c) ");
        strcpy(bufferline2, buffer1);
        break;
    case HUMED:
        strcpy(bufferline1, "-Humdedad(%)   ");
        strcpy(bufferline2, buffer2);
        break;
    case HUMED_S:
        strcpy(bufferline1, "-HumedadSuelo(%)");
        strcpy(bufferline2, buffer3);
        break;
    case T_ILUM:
        strcpy(bufferline1, "-Horas de luz(H)");
        strcpy(bufferline2, buffer4);
        break;
    default:
        strcpy(bufferline1, "err");
        break;
    }

    // que se va a resaltar en pantalla
    bool highlight1 = false;
    bool highlight2 = false;
    char bufferline3[5];
    char bufferline4[5];
    char bufferline5[5];

    switch (*subPantalla)
    {
    case 0:
        highlight1 = false;
        highlight2 = false;
        strcpy(bufferline3, "-   ");
        strcpy(bufferline4, "    ");
        strcpy(bufferline5, "    ");
        break;
    case 1:
        highlight1 = false;
        highlight2 = false;
        strcpy(bufferline3, "    ");
        strcpy(bufferline4, " -  ");
        strcpy(bufferline5, "    ");
        break;
    case 2:
        highlight1 = false;
        highlight2 = false;
        strcpy(bufferline3, "    ");
        strcpy(bufferline4, "    ");
        strcpy(bufferline5, "   -");
        break;
    case 3:
        highlight1 = true;
        highlight2 = false;
        strcpy(bufferline3, "    ");
        strcpy(bufferline4, "    ");
        strcpy(bufferline5, "    ");
        break;
    case 4:
        highlight1 = false;
        highlight2 = true;
        strcpy(bufferline3, "    ");
        strcpy(bufferline4, "    ");
        strcpy(bufferline5, "    ");
    default:
        break;
    }

    // Mostrar en pantalla
    ssd1306_clear_screen(&dev, false);
    ssd1306_display_text(&dev, 1, bufferline1, 16, false);
    ssd1306_display_text(&dev, 5, "             ok ", 16, highlight1);
    ssd1306_display_text(&dev, 7, "            back", 16, highlight2);
    ssd1306_display_text(&dev, 7, "       ", 6, false);
    ssd1306_display_text_x3(&dev, 5, bufferline5, 4, false);
    ssd1306_display_text_x3(&dev, 5, bufferline4, 2, false);
    ssd1306_display_text_x3(&dev, 5, bufferline3, 1, false);
    ssd1306_display_text_x3(&dev, 3, bufferline2, 4, false);
    // Listen buttons to know what to do next
    while (1) // no sale de este ciclo hasta que encuentre un boton presionado valido, para tener en cuenta!
    {
        buttonPressed = listenButtons(); // función para escuchar botones
        vTaskDelay(pdMS_TO_TICKS(50));   // retraso para que no solo se refleje 1 boton presionado

        // IF para saber cuando se presionó un boton
        if ((buttonPressed == ButtonRIGHT || buttonPressed == ButtonUP || buttonPressed == ButtonDOWN) && *subPantalla == 0)
        { // botones validos pantalla 1
            if (buttonPressed == ButtonRIGHT)
            {
                *subPantalla = 1;
                siguienteInterfaz = S_Ui_Customs;
            }
            else if (buttonPressed == ButtonUP)
            {
                *subPantalla = 0;
                // modificando parametros individualmente:
                switch (*paramID)
                {
                case TEMPER:
                    if ((BufferForParameters->temperatura) < (RangoMaxUiCustom->temperatura - 10))
                    {
                        BufferForParameters->temperatura += 10;
                    }
                    break;
                case HUMED:
                    if ((BufferForParameters->humedad) < (RangoMaxUiCustom->humedad - 10))
                    {
                        BufferForParameters->humedad += 10;
                    }
                    break;
                case HUMED_S:
                    if ((BufferForParameters->humedadSuelo) < (RangoMaxUiCustom->humedadSuelo - 10))
                    {
                        BufferForParameters->humedadSuelo += 10;
                    }
                    break;
                case T_ILUM:
                    if ((BufferForParameters->T_iluminacion) < (RangoMaxUiCustom->T_iluminacion - 10))
                    {
                        BufferForParameters->T_iluminacion += 10;
                    }
                    break;
                default:
                    break;
                }
                siguienteInterfaz = S_Ui_Customs;
            }
            else if (buttonPressed == ButtonDOWN)
            {
                *subPantalla = 0;
                // modificando parametros individualmente:
                switch (*paramID)
                {
                case TEMPER:
                    if ((BufferForParameters->temperatura) > (RangoMinUiCustom->temperatura + 10))
                    {
                        printf("Temperatura:%.1f, Temperaturaminima+10 %.1f", BufferForParameters->temperatura, RangoMinUiCustom->T_iluminacion + 10);
                        BufferForParameters->temperatura -= 10;
                    }
                    break;
                case HUMED:
                    if ((BufferForParameters->humedad) > (RangoMinUiCustom->humedad + 10))
                    {
                        BufferForParameters->humedad -= 10;
                    }
                    break;
                case HUMED_S:
                    if ((BufferForParameters->humedadSuelo) > (RangoMinUiCustom->humedadSuelo + 10))
                    {
                        BufferForParameters->humedadSuelo -= 10;
                    }
                    break;
                case T_ILUM:
                    if ((BufferForParameters->T_iluminacion) > (RangoMinUiCustom->T_iluminacion + 10))
                    {
                        BufferForParameters->T_iluminacion -= 10;
                    }
                    break;
                default:
                    break;
                }
                siguienteInterfaz = S_Ui_Customs;
            }
            break;
        }
        else if ((buttonPressed != ButtonOK) && *subPantalla == 1)
        { // botones validos pantalla 1
            if (buttonPressed == ButtonLEFT)
            {
                *subPantalla = 0;
                siguienteInterfaz = S_Ui_Customs; // haría que cambie a la misma pantalla , pero con la subpantalla 1.
            }
            else if (buttonPressed == ButtonRIGHT)
            {
                *subPantalla = 2;
                siguienteInterfaz = S_Ui_Customs;
            }
            else if (buttonPressed == ButtonUP)
            {
                *subPantalla = 1;
                // modificando parametros individualmente:
                switch (*paramID)
                {
                case TEMPER:
                    if ((BufferForParameters->temperatura) < (RangoMaxUiCustom->temperatura - 1))
                    {
                        BufferForParameters->temperatura += 1;
                    }
                    break;
                case HUMED:
                    if ((BufferForParameters->humedad) < (RangoMaxUiCustom->humedad - 1))
                    {
                        BufferForParameters->humedad += 1;
                    }
                    break;
                case HUMED_S:
                    if ((BufferForParameters->humedadSuelo) < (RangoMaxUiCustom->humedadSuelo - 1))
                    {
                        BufferForParameters->humedadSuelo += 1;
                    }
                    break;
                case T_ILUM:
                    if ((BufferForParameters->T_iluminacion) < (RangoMaxUiCustom->T_iluminacion - 1))
                    {
                        BufferForParameters->T_iluminacion += 1;
                    }
                    break;
                default:
                    break;
                }
                siguienteInterfaz = S_Ui_Customs;
            }
            else if (buttonPressed == ButtonDOWN)
            {
                *subPantalla = 1;
                // modificando parametros individualmente:
                switch (*paramID)
                {
                case TEMPER:
                    if ((BufferForParameters->temperatura) > (RangoMinUiCustom->temperatura + 1))
                    {
                        printf("Temperatura:%.1f, Temperaturaminima+1 %.1f", BufferForParameters->temperatura, RangoMinUiCustom->T_iluminacion + 1);
                        BufferForParameters->temperatura -= 1;
                    }
                    break;
                case HUMED:
                    if ((BufferForParameters->humedad) > (RangoMinUiCustom->humedad + 1))
                    {
                        BufferForParameters->humedad -= 1;
                    }
                    break;
                case HUMED_S:
                    if ((BufferForParameters->humedadSuelo) > (RangoMinUiCustom->humedadSuelo + 1))
                    {
                        BufferForParameters->humedadSuelo -= 1;
                    }
                    break;
                case T_ILUM:
                    if ((BufferForParameters->T_iluminacion) > (RangoMinUiCustom->T_iluminacion + 1))
                    {
                        BufferForParameters->T_iluminacion -= 1;
                    }
                    break;
                default:
                    break;
                }
                siguienteInterfaz = S_Ui_Customs;
            }
            break;
        }
        else if ((buttonPressed != ButtonOK) && *subPantalla == 2)
        { // botones validos pantalla 1
            if (buttonPressed == ButtonLEFT)
            {
                *subPantalla = 1;
                siguienteInterfaz = S_Ui_Customs; // haría que cambie a la misma pantalla , pero con la subpantalla 1.
            }
            else if (buttonPressed == ButtonRIGHT)
            {
                *subPantalla = 3;
                siguienteInterfaz = S_Ui_Customs;
            }
            else if (buttonPressed == ButtonUP)
            {
                *subPantalla = 2;
                // modificando parametros individualmente:
                switch (*paramID)
                {
                case TEMPER:
                    if ((BufferForParameters->temperatura) < (RangoMaxUiCustom->temperatura - 0.1))
                    {
                        BufferForParameters->temperatura += 0.1;
                    }
                    break;
                case HUMED:
                    if ((BufferForParameters->humedad) < (RangoMaxUiCustom->humedad - 0.1))
                    {
                        BufferForParameters->humedad += 0.1;
                    }
                    break;
                case HUMED_S:
                    if ((BufferForParameters->humedadSuelo) < (RangoMaxUiCustom->humedadSuelo - 0.1))
                    {
                        BufferForParameters->humedadSuelo += 0.1;
                    }
                    break;
                case T_ILUM:
                    if ((BufferForParameters->T_iluminacion) < (RangoMaxUiCustom->T_iluminacion - 0.1))
                    {
                        BufferForParameters->T_iluminacion += 0.1;
                    }
                    break;
                default:
                    break;
                }
                siguienteInterfaz = S_Ui_Customs;
            }
            else if (buttonPressed == ButtonDOWN)
            {
                *subPantalla = 2;
                // modificando parametros individualmente:
                switch (*paramID)
                {
                case TEMPER:
                    if ((BufferForParameters->temperatura) > (RangoMinUiCustom->temperatura + 0.1))
                    {
                        printf("Temperatura:%.1f, Temperaturaminima+0.1 %.1f", BufferForParameters->temperatura, RangoMinUiCustom->T_iluminacion + 0.1);
                        BufferForParameters->temperatura -= 0.1;
                    }
                    break;
                case HUMED:
                    if ((BufferForParameters->humedad) > (RangoMinUiCustom->humedad + 0.1))
                    {
                        BufferForParameters->humedad -= 0.1;
                    }
                    break;
                case HUMED_S:
                    if ((BufferForParameters->humedadSuelo) > (RangoMinUiCustom->humedadSuelo + 0.1))
                    {
                        BufferForParameters->humedadSuelo -= 0.1;
                    }
                    break;
                case T_ILUM:
                    if ((BufferForParameters->T_iluminacion) > (RangoMinUiCustom->T_iluminacion + 0.1))
                    {
                        BufferForParameters->T_iluminacion -= 0.1;
                    }
                    break;
                default:
                    break;
                }
                siguienteInterfaz = S_Ui_Customs;
            }
            break;
        }
        else if (*subPantalla == 3 && buttonPressed != ButtonRIGHT)
        {
            if (buttonPressed == ButtonLEFT || buttonPressed == ButtonUP)
            {
                *subPantalla = 2;
                siguienteInterfaz = S_Ui_Customs; // haría que cambie a la misma pantalla , pero con la subpantalla 1.
            }
            else if (buttonPressed == ButtonDOWN)
            {
                *subPantalla = 4;
                siguienteInterfaz = S_Ui_Customs;
            }
            else // aquí entra el botton ok
            {
                *subPantalla = 0;
                switch (*paramID)
                {
                case TEMPER:
                    siguienteInterfaz = S_Ui_Customs;
                    *paramID = HUMED; // valor a variar siguiente
                    break;
                case HUMED:
                    siguienteInterfaz = S_Ui_Customs;
                    *paramID = HUMED_S;
                    break;
                case HUMED_S:
                    siguienteInterfaz = S_Ui_Customs;
                    *paramID = T_ILUM;
                    break;
                case T_ILUM:
                    siguienteInterfaz = S_Ui_Validation;
                    break;
                default:
                    break;
                }
            }
            break;
        }
        else if ((buttonPressed != ButtonRIGHT && buttonPressed != ButtonDOWN) && *subPantalla == 4)
        {
            if (buttonPressed == ButtonLEFT || buttonPressed == ButtonUP)
            {
                *subPantalla = 3;
                siguienteInterfaz = S_Ui_Customs; // haría que cambie a la misma pantalla , pero con la subpantalla 1.
            }
            else
            {
                *subPantalla = 0;
                switch (*paramID)
                {
                case TEMPER:
                    siguienteInterfaz = S_Ui_Config;
                    break;
                case HUMED:
                    siguienteInterfaz = S_Ui_Customs;
                    *paramID = TEMPER;
                    break;
                case HUMED_S:
                    siguienteInterfaz = S_Ui_Customs;
                    *paramID = HUMED;
                    break;
                case T_ILUM:
                    siguienteInterfaz = S_Ui_Customs;
                    *paramID = HUMED_S;
                    break;
                default:
                    break;
                }
            }
            break;
        }
        else if (buttonPressed == ScreenOffFlag)
        {
            *paramID = TEMPER;
            siguienteInterfaz = S_OFF; // opción para cuando el offFlag
            break;
        }
        else
        {
            continue;
        }
    } // no sale de este ciclo hasta que encuentre un boton presionado valido, para tener en cuenta!

    return siguienteInterfaz;
}
uint8_t Ui_Validation(uint8_t *subPantalla, ParamID *paramID, Parameters *BufferForParameters, Parameters *parameteres)
{
    // En esta interfaz se decide si volcar o no el buffer a la variable de trabajo y control.
    uint8_t siguienteInterfaz = 0;
    // Como se pueden ocupar hasta 18 bytes, el compilador bota un error si no se pone 18.
    char buffer1[18];
    char buffer2[18];
    char buffer3[18];
    char buffer4[18];
    // concatenando el string a mostrar
    sprintf(buffer1, "%s%.1f%c", "Grados    =", BufferForParameters->temperatura, 'C');
    sprintf(buffer2, "%s%.1f%c", "Humedad   =", BufferForParameters->humedad, '%');
    sprintf(buffer3, "%s%.1f%c", "Humedad.S =", BufferForParameters->humedadSuelo, '%');
    sprintf(buffer4, "%s%.1f%c", "t. de luz =", BufferForParameters->T_iluminacion, 'H');
    // configurando subpantallas.

    bool highlightLine1;
    bool highlightLine2;
    if (*subPantalla == 0)
    {
        highlightLine1 = false;
        highlightLine2 = true;
    }
    else
    {
        highlightLine1 = true;
        highlightLine2 = false;
    }
    ssd1306_clear_screen(&dev, false);
    ssd1306_display_text(&dev, 1, buffer1, 16, false);
    ssd1306_display_text(&dev, 2, buffer2, 16, false);
    ssd1306_display_text(&dev, 3, buffer3, 16, false);
    ssd1306_display_text(&dev, 4, buffer4, 16, false);

    ssd1306_display_text(&dev, 6, "           Back ", 16, highlightLine1);
    ssd1306_display_text(&dev, 6, " Confirm ", 9, highlightLine2);

    // Listen buttons to know what to do next
    while (1)
    {
        buttonPressed = listenButtons(); // función para escuchar botones
        vTaskDelay(pdMS_TO_TICKS(200));  // retraso para que no solo se refleje 1 boton presionado

        // IF para saber cuando se presionó un boton
        if ((buttonPressed == ButtonOK || buttonPressed == ButtonRIGHT) && *subPantalla == 0)
        {                                     // botones validos pantalla 1
            if (buttonPressed == ButtonRIGHT) // para que se pueda bajar, tiene que estar en la subpantalla 0  ("configurar<-")
            {
                *subPantalla = 1;
                siguienteInterfaz = S_Ui_Validation;
            }
            else
            {
                *subPantalla = 0;
                *paramID = TEMPER;
                *parameteres = *BufferForParameters; // Volcando el buffer!!
                validationMesagge();
                siguienteInterfaz = S_OFF;
            }
            break;
        }
        else if ((buttonPressed == ButtonOK || buttonPressed == ButtonLEFT) && *subPantalla == 1)
        { // botones validos pantalla 1
            if (buttonPressed == ButtonLEFT)
            {
                *subPantalla = 0;
                siguienteInterfaz = S_Ui_Validation; // haría que cambie a la misma pantalla , pero con la subpantalla 1.
            }
            else
            {
                *subPantalla = 0;
                if (customFlag) // decidiendo si el back es customs o default
                {
                    siguienteInterfaz = S_Ui_Customs;
                }
                else
                {
                    siguienteInterfaz = S_Ui_Defaults;
                }
            }
            break;
        }
        else if (buttonPressed == ScreenOffFlag)
        {
            siguienteInterfaz = S_OFF; // opción para cuando el offFlag
            break;
        }
        else
        {
            continue;
        }
    }
    return siguienteInterfaz;
}
void validationMesagge()
{
    ssd1306_clear_screen(&dev, true);
    ssd1306_display_text(&dev, 3, " Parametros     ", 16, true);
    ssd1306_display_text(&dev, 4, "  actualizados! ", 16, true);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
}

void Alarm(AlarmMessage alarmMessage, bool alarmFlag)
{ // led verde GPIO 16, buzzer GPIO 14.
    // while(*alarmFlag){ //->si se puede, hacer una interrupcion, usando el alarmFlag para acabar la task
    if (alarmFlag)
    {
        gpio_set_level(ledVerde, 0);
        gpio_set_level(buzzer, 1);
        ssd1306_clear_screen(&dev, true);
        ssd1306_display_text_x3(&dev, 1, "Alarm", 5, true);
        ssd1306_display_text(&dev, 5, "Por favor presta", 16, true);
        ssd1306_display_text(&dev, 6, "    atencion    ", 16, true);
        vTaskDelay(2000 / portTICK_PERIOD_MS);

        ssd1306_clear_screen(&dev, true);
        gpio_set_level(buzzer, 0);
        ssd1306_display_text(&dev, 0, alarmMessage.line0, 16, false);
        ssd1306_display_text(&dev, 1, alarmMessage.line1, 16, false);
        ssd1306_display_text(&dev, 2, alarmMessage.line2, 16, false);
        ssd1306_display_text(&dev, 3, alarmMessage.line3, 16, false);
        ssd1306_display_text(&dev, 4, alarmMessage.line4, 16, false);
        ssd1306_display_text(&dev, 5, alarmMessage.line5, 16, false);
        ssd1306_display_text(&dev, 6, alarmMessage.line6, 16, false);
        ssd1306_display_text(&dev, 7, alarmMessage.line7, 16, false);
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
    else
    {
        gpio_set_level(ledVerde, 1);
        gpio_set_level(buzzer, 0); // cuando todo vaya bien
    }
    //}
}