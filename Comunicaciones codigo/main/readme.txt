Han surgido distintos errores a lo largo del desarrollo del software
hay que decir que el ADC implementado no tuvo la funcionalidad esperaba en principio.

Luego a la hora de configurar los estados de la pantalla y actualizar los valores, parece que
hay algo que corrompe la memoria luego de escribir sobre ella un cierto numero de veces.

One of the most common causes of a stack overflow is the recursive function, a type of function that repeatedly calls
itself in an attempt to carry out specific logic. Each time the function calls itself, it uses up more of the stack memory. 
If the function runs too many times, it can eat up all the available memory, resulting in a stack overflow

Luego hubo que buscar la manera de desocupar el stack de buffers.


//uint8_t Ui_Customs(uint8_t *subPantalla, ParamID *paramID, Parameters *BufferForParameters)
//{ }


// subpantalla ayuda a diferenciar place holder, paramID ayuda a diferenciar que parametro se está modificando.
    /*uint8_t siguienteInterfaz = 0;
    char bufferline1[17];
    char bufferline2[5];
    char buffer1[5]; // buffers locales
    char buffer2[5];
    char buffer3[5];
    char buffer4[5];
    // concatenando el string a mostrar
    sprintf(buffer1, "%.1f", BufferForParameters->temperatura);
    sprintf(buffer2, "%.1f", BufferForParameters->humedad);
    sprintf(buffer3, "%.1f", BufferForParameters->humedadSuelo);
    sprintf(buffer4, "%.1f", BufferForParameters->T_iluminacion);

    // configurando subpantallas (un montoooon)
    bool highlight1;
    bool highlight2;
    bool highlight3;
    bool highlight4;
    bool highlight5;

    // Que parametros se va a modificar y mostra en pantalla
    switch (*paramID)
    {
    case TEMPER:
        strcpy(bufferLine1, "-Temperatura(c) ");
        strcpy(bufferLine2, buffer1);
        break;
    case HUMED:
        strcpy(bufferLine1, "-Humdedad(%%)   ");
        strcpy(bufferLine2, buffer2);
        break;
    case HUMED_S:
        strcpy(bufferLine1, "-HumedadSuelo(%%)");
        strcpy(bufferLine2, buffer3);
        break;
    case T_ILUM:
        strcpy(bufferLine1, "-Horas de luz(H)");
        strcpy(bufferLine2, buffer4);
        break;
    default:
        strcpy(bufferLine1, "err");
        break;
    }

    // que se va a resaltar en pantalla
    switch (*subPantalla)
    {
    case 0:
        highlight1 = true;
        highlight2 = false;
        highlight3 = false;
        highlight4 = false;
        highlight5 = false;
        break;
    case 1:
        highlight1 = false;
        highlight2 = true;
        highlight3 = false;
        highlight4 = false;
        highlight5 = false;
        break;
    case 2:
        highlight1 = false;
        highlight2 = false;
        highlight3 = true;
        highlight4 = false;
        highlight5 = false;
        break;
    case 3:
        highlight1 = false;
        highlight2 = false;
        highlight3 = false;
        highlight4 = true;
        highlight5 = false;
    case 4:
        highlight1 = false;
        highlight2 = false;
        highlight3 = false;
        highlight4 = false;
        highlight5 = true;
    default:
        highlightLine1 = true;
        break;
    }

    ssd1306_display_text(&dev, 1, bufferLine1, 16, false);
    ssd1306_display_text(&dev, 5, "             ok ", 16, highlightLine1);
    ssd1306_display_text(&dev, 7, "            back", 16, highlightLine2);
    ssd1306_display_text(&dev, 7, "       ", 6, false);
    ssd1306_display_text_x3(&dev, 5, "-   ", 4, highlightLine3);
    ssd1306_display_text_x3(&dev, 5, " -  ", 4, highlightLine4);
    ssd1306_display_text_x3(&dev, 5, "   -", 4, highlightLine5);
    ssd1306_display_text_x3(&dev, 3, bufferLine2, 4, false);

    // Listen buttons to know what to do next
    while (1)
    {
        buttonPressed = listenButtons(); // función para escuchar botones
        vTaskDelay(pdMS_TO_TICKS(200));  // retraso para que no solo se refleje 1 boton presionado

        // IF para saber cuando se presionó un boton
        if ((buttonPressed == ButtonRIGHT || buttonPressed == ButtonUP || buttonPressed == ButtonDOWN) && *subPantalla == 0)
        {                                     // botones validos pantalla 1
            if (buttonPressed == ButtonRIGHT) // para que se pueda bajar, tiene que estar en la subpantalla 0  ("configurar<-")
            {
                *subPantalla = 1;
                siguienteInterfaz = S_Ui_Customs;
            }
            else if (buttonPressed == ButtonUP)
            {
                *subPantalla = 0;
                // modificando parametros individualmente:
                switch (paramID)
                {
                case TEMPER:
                    BufferForParameters->temperatura += 10;
                    break;
                case HUMED:
                    BufferForParameters->humedad += 10;
                    break;
                case HUMED_S:
                    BufferForParameters->humedadSuelo += 10;
                    break;
                case T_ILUM:
                    BufferForParameters->T_iluminacion += 10;
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
                switch (paramID)
                {
                case TEMPER:
                    BufferForParameters->temperatura -= 10;
                    break;
                case HUMED:
                    BufferForParameters->humedad -= 10;
                    break;
                case HUMED_S:
                    BufferForParameters->humedadSuelo -= 10;
                    break;
                case T_ILUM:
                    BufferForParameters->T_iluminacion -= 10;
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
                switch (paramID)
                {
                case TEMPER:
                    BufferForParameters->temperatura += 1;
                    break;
                case HUMED:
                    BufferForParameters->humedad += 1;
                    break;
                case HUMED_S:
                    BufferForParameters->humedadSuelo += 1;
                    break;
                case T_ILUM:
                    BufferForParameters->T_iluminacion += 1;
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
                switch (paramID)
                {
                case TEMPER:
                    BufferForParameters->temperatura -= 1;
                    break;
                case HUMED:
                    BufferForParameters->humedad -= 1;
                    break;
                case HUMED_S:
                    BufferForParameters->humedadSuelo -= 1;
                    break;
                case T_ILUM:
                    BufferForParameters->T_iluminacion -= 1;
                    break;
                default:
                    break;
                }
                siguienteInterfaz = S_Ui_Customs;
                break;
            }
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
                switch (paramID)
                {
                case TEMPER:
                    BufferForParameters->temperatura += 0.1;
                    break;
                case HUMED:
                    BufferForParameters->humedad += 0.1;
                    break;
                case HUMED_S:
                    BufferForParameters->humedadSuelo += 0.1;
                    break;
                case T_ILUM:
                    BufferForParameters->T_iluminacion += 0.1;
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
                switch (paramID)
                {
                case TEMPER:
                    BufferForParameters->temperatura -= 0.1;
                    break;
                case HUMED:
                    BufferForParameters->humedad -= 0.1;
                    break;
                case HUMED_S:
                    BufferForParameters->humedadSuelo -= 0.1;
                    break;
                case T_ILUM:
                    BufferForParameters->T_iluminacion -= 0.1;
                    break;
                default:
                    break;
                }
                siguienteInterfaz = S_Ui_Customs;
                break;
            }

            else if (buttonPressed == ScreenOffFlag)
            {
                siguienteInterfaz = 255; // opción para cuando el offFlag
                break;
            }
            else
            {
                continue;
            }
        }
        return siguienteInterfaz;
    }*/