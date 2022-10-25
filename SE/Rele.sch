EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 4
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Relay:SANYOU_SRD_Form_C K?
U 1 1 62C03677
P 5350 3300
F 0 "K?" H 5780 3346 50  0000 L CNN
F 1 "SANYOU_SRD_Form_C" H 5780 3255 50  0000 L CNN
F 2 "Relay_THT:Relay_SPDT_SANYOU_SRD_Series_Form_C" H 5800 3250 50  0001 L CNN
F 3 "http://www.sanyourelay.ca/public/products/pdf/SRD.pdf" H 5350 3300 50  0001 C CNN
	1    5350 3300
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 62C0367D
P 4650 3900
F 0 "R?" V 4443 3900 50  0000 C CNN
F 1 "5.6k" V 4534 3900 50  0000 C CNN
F 2 "" V 4580 3900 50  0001 C CNN
F 3 "~" H 4650 3900 50  0001 C CNN
	1    4650 3900
	0    1    1    0   
$EndComp
Wire Wire Line
	5150 3600 5150 3700
Wire Wire Line
	5550 4850 5150 4850
Wire Wire Line
	5150 4850 5150 4100
Wire Wire Line
	5150 3000 5150 2950
Text GLabel 5150 2750 0    50   Input ~ 0
5v
Text GLabel 3850 3900 0    50   Input ~ 0
Crelay
Text GLabel 5550 4850 2    50   Input ~ 0
GND
$Comp
L Diode:1N4004 D?
U 1 1 62C0368A
P 4550 3200
F 0 "D?" V 4504 3280 50  0000 L CNN
F 1 "1N4004" V 4595 3280 50  0000 L CNN
F 2 "Diode_THT:D_DO-41_SOD81_P10.16mm_Horizontal" H 4550 3025 50  0001 C CNN
F 3 "http://www.vishay.com/docs/88503/1n4001.pdf" H 4550 3200 50  0001 C CNN
	1    4550 3200
	0    1    1    0   
$EndComp
Wire Wire Line
	5150 2950 4550 2950
Wire Wire Line
	4550 2950 4550 3000
Connection ~ 5150 2950
Wire Wire Line
	5150 2950 5150 2750
Wire Wire Line
	5150 3600 4550 3600
Wire Wire Line
	4550 3600 4550 3450
Text GLabel 5450 2750 0    50   Input ~ 0
12v
Wire Wire Line
	5450 2750 5450 3000
$Comp
L Device:LED D?
U 1 1 62C03698
P 4300 3900
F 0 "D?" H 4293 4117 50  0000 C CNN
F 1 "LED" H 4293 4026 50  0000 C CNN
F 2 "" H 4300 3900 50  0001 C CNN
F 3 "~" H 4300 3900 50  0001 C CNN
	1    4300 3900
	-1   0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Female J?
U 1 1 62C0369E
P 5750 4150
F 0 "J?" H 5778 4126 50  0000 L CNN
F 1 "Conn_01x02_Female" H 5778 4035 50  0000 L CNN
F 2 "" H 5750 4150 50  0001 C CNN
F 3 "~" H 5750 4150 50  0001 C CNN
	1    5750 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	5550 3600 5550 4150
Wire Wire Line
	5550 4250 5550 4850
Wire Wire Line
	4850 3900 4800 3900
Wire Wire Line
	4450 3900 4500 3900
$Comp
L Device:Buzzer BZ?
U 1 1 62C036A8
P 4050 3200
F 0 "BZ?" H 4055 3490 50  0000 C CNN
F 1 "Buzzer" H 4055 3399 50  0000 C CNN
F 2 "" V 4025 3300 50  0001 C CNN
F 3 "~" V 4025 3300 50  0001 C CNN
	1    4050 3200
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4550 3000 4150 3000
Wire Wire Line
	4150 3000 4150 3100
Connection ~ 4550 3000
Wire Wire Line
	4550 3000 4550 3050
Wire Wire Line
	4550 3450 4250 3450
Wire Wire Line
	4250 3450 4250 3300
Wire Wire Line
	4250 3300 4150 3300
Connection ~ 4550 3450
Wire Wire Line
	4550 3450 4550 3350
Connection ~ 5150 3600
Wire Wire Line
	3850 3900 4150 3900
$Comp
L Transistor_BJT:BC548 Q?
U 1 1 62C036B9
P 5050 3900
F 0 "Q?" H 5241 3946 50  0000 L CNN
F 1 "BC548" H 5241 3855 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 5250 3825 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/BC550-D.pdf" H 5050 3900 50  0001 L CNN
	1    5050 3900
	1    0    0    -1  
$EndComp
$EndSCHEMATC
