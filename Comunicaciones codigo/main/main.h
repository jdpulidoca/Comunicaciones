#ifndef myMain
#define myMain
typedef struct{
		float temperatura;
		float humedad;
		float humedadSuelo;
		float T_iluminacion;	
}Parameters;
#define ledVerde 16
#define buzzer 14
void app_main(void);
void variablesInit(void);
#endif 