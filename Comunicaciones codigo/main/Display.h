#ifndef Display
#define Display
#include "ssd1306.h"
#include "main.h"

typedef enum
{
    S_WellcomeUi = 0,
    S_Ui_Init,
    S_Ui_Config,
    S_Ui_Monitor,
    S_Ui_Defaults,
    S_Ui_Customs,
    S_Ui_Validation,
    S_OFF
} Screens;

typedef enum
{
    TEMPER = 0,
    HUMED_S,
    HUMED,
    T_ILUM
} ParamID;

typedef struct
{
    char line0[16];
    char line1[16];
    char line2[16];
    char line3[16];
    char line4[16];
    char line5[16];
    char line6[16];
    char line7[16];
} AlarmMessage;

uint8_t WellcomeUi(void);
void UiClear(void);
void validationMesagge(void);

uint8_t Ui_Init(uint8_t *subPantalla);
void UserInterface(Screens *screenSelected, Parameters *parameters, Parameters *BufferForParameters, Parameters *MainLog, Parameters *defaultPlant1, Parameters *defaultPlant2, Parameters *defaultPlant3, Parameters *RangoMaxUiCustom, Parameters *RangoMinUiCustom);

uint8_t Ui_Monitor(uint8_t *subPantalla, Parameters *MainLog);
uint8_t Ui_Config(uint8_t *subPantalla);
uint8_t Ui_Customs(uint8_t *subPantalla, ParamID *paramID, Parameters *BufferForParameters, Parameters *RangoMaxUiCustom, Parameters *RangoMinUiCustom);
uint8_t Ui_Defaults(uint8_t *subPantalla, Parameters *BufferForParameters, Parameters *defaultPlant1, Parameters *defaultPlant2, Parameters *defaultPlant3);
uint8_t Ui_Validation(uint8_t *subPantalla, ParamID *paramID, Parameters *BufferForParameters, Parameters *parameteres);

void Alarm(AlarmMessage alarmMessage, bool alarmFlag);

uint8_t UiParamasProof(float temperatura, float humedad, float humedadSuelo, float T_iluminacion);

void SetComun(void);

#endif

/*uint8_t Ui_Init_OnMonitor(void);
uint8_t Ui_Monitor_OnReload(uint8_t temperatura, uint8_t humedad, uint8_t humedadSuelo, uint8_t T_iluminacion);
uint8_t Ui_Monitor_OnBack(void);
uint8_t Ui_Config_OnPredeterminado(void);
uint8_t Ui_Config_OnPersonalizado(void);
uint8_t Ui_Config_OnBack(void);
uint8_t Ui_Predeterminado_onPlant1(void); */