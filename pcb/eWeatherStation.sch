EESchema Schematic File Version 4
LIBS:eWeatherStation-cache
EELAYER 29 0
EELAYER END
$Descr USLetter 11000 8500
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Notes 650  900  0    50   ~ 0
MCU selection is going to depend on how many inputs and ouputs needed for RF reception and for LCD\n\nWe can probably just tie SEL0/SEL1 to GND which gives 2.5kbps, still higher then most of the Acurite protocols. \nOr even tie VSEL1 to GND and only toggle VSEL0 for 2.5k/5k rates.
Text Label 8100 1950 0    50   ~ 0
eBUSY
Text Label 8100 2550 0    50   ~ 0
eRST
Text Label 8100 2650 0    50   ~ 0
eDC
Text Label 8100 2750 0    50   ~ 0
eCLK
Text Label 8100 3150 0    50   ~ 0
eDIN
$Comp
L Device:R_Small R9
U 1 1 5C07A008
P 8350 1100
F 0 "R9" H 8291 1054 50  0000 R CNN
F 1 "100k" H 8291 1145 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric" H 8350 1100 50  0001 C CNN
F 3 "~" H 8350 1100 50  0001 C CNN
	1    8350 1100
	-1   0    0    1   
$EndComp
Wire Wire Line
	8000 1750 8650 1750
$Comp
L power:GND #PWR026
U 1 1 5C086020
P 8650 1950
F 0 "#PWR026" H 8650 1700 50  0001 C CNN
F 1 "GND" H 8655 1777 50  0000 C CNN
F 2 "" H 8650 1950 50  0001 C CNN
F 3 "" H 8650 1950 50  0001 C CNN
	1    8650 1950
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R10
U 1 1 5C08607D
P 8650 1850
F 0 "R10" H 8709 1896 50  0000 L CNN
F 1 "100k" H 8709 1805 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" H 8650 1850 50  0001 C CNN
F 3 "~" H 8650 1850 50  0001 C CNN
	1    8650 1850
	1    0    0    -1  
$EndComp
Wire Notes Line
	550  7050 1950 7050
Wire Notes Line
	1950 8000 550  8000
Text Notes 950  7050 0    50   ~ 0
3.3V Regulation
Text Label 8100 1650 0    50   ~ 0
TXD
Text Label 8100 1850 0    50   ~ 0
RXD
$Comp
L Switch:SW_SPST SW2
U 1 1 5C11B0C7
P 8550 1450
F 0 "SW2" H 8550 1650 50  0000 C CNN
F 1 "FLASH" H 8550 1550 50  0000 C CNN
F 2 "Button_Switch_SMD:SW_SPST_PTS810" H 8550 1450 50  0001 C CNN
F 3 "" H 8550 1450 50  0001 C CNN
	1    8550 1450
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR025
U 1 1 5C121F30
P 8350 1000
F 0 "#PWR025" H 8350 850 50  0001 C CNN
F 1 "+3V3" H 8365 1173 50  0000 C CNN
F 2 "" H 8350 1000 50  0001 C CNN
F 3 "" H 8350 1000 50  0001 C CNN
	1    8350 1000
	1    0    0    -1  
$EndComp
Wire Wire Line
	8000 1550 8350 1550
$Comp
L power:GND #PWR027
U 1 1 5C16CDAC
P 8750 1500
F 0 "#PWR027" H 8750 1250 50  0001 C CNN
F 1 "GND" H 8755 1327 50  0000 C CNN
F 2 "" H 8750 1500 50  0001 C CNN
F 3 "" H 8750 1500 50  0001 C CNN
	1    8750 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	8750 1450 8750 1500
Wire Wire Line
	8350 1550 8350 1450
Text Label 750  5150 0    50   ~ 0
TXD
Text Label 750  5350 0    50   ~ 0
RXD
$Comp
L power:GND #PWR03
U 1 1 5C182F1A
P 650 5350
F 0 "#PWR03" H 650 5100 50  0001 C CNN
F 1 "GND" H 655 5177 50  0000 C CNN
F 2 "" H 650 5350 50  0001 C CNN
F 3 "" H 650 5350 50  0001 C CNN
	1    650  5350
	1    0    0    -1  
$EndComp
Text Notes 750  5000 0    50   ~ 0
Programming
$Comp
L Device:R_Small R3
U 1 1 5C1EF12E
P 1750 6150
F 0 "R3" H 1691 6104 50  0000 R CNN
F 1 "470k" H 1691 6195 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric" H 1750 6150 50  0001 C CNN
F 3 "~" H 1750 6150 50  0001 C CNN
	1    1750 6150
	-1   0    0    1   
$EndComp
$Comp
L Device:R_Small R4
U 1 1 5C1EF1C4
P 1750 6450
F 0 "R4" H 1809 6496 50  0000 L CNN
F 1 "150k" H 1809 6405 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" H 1750 6450 50  0001 C CNN
F 3 "~" H 1750 6450 50  0001 C CNN
	1    1750 6450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR07
U 1 1 5C2083D1
P 1750 6600
F 0 "#PWR07" H 1750 6350 50  0001 C CNN
F 1 "GND" H 1755 6427 50  0000 C CNN
F 2 "" H 1750 6600 50  0001 C CNN
F 3 "" H 1750 6600 50  0001 C CNN
	1    1750 6600
	1    0    0    -1  
$EndComp
Wire Wire Line
	1750 6600 1750 6550
Text Notes 1100 5700 0    50   ~ 0
ADC Dividers
Text Notes 600  5800 0    24   ~ 0
4.5V will give about 1.09V - just under the ESP32 1.1V ADC maximum input
Text Label 8100 3550 0    50   ~ 0
ADC_BATT
Text Label 1150 6300 2    50   ~ 0
ADC_BATT
Text Label 8100 3250 0    50   ~ 0
RADIO
$Comp
L power:+3V3 #PWR023
U 1 1 5C18FF49
P 7400 900
F 0 "#PWR023" H 7400 750 50  0001 C CNN
F 1 "+3V3" H 7415 1073 50  0000 C CNN
F 2 "" H 7400 900 50  0001 C CNN
F 3 "" H 7400 900 50  0001 C CNN
	1    7400 900 
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR024
U 1 1 5C1DF24C
P 7400 4200
F 0 "#PWR024" H 7400 3950 50  0001 C CNN
F 1 "GND" H 7405 4027 50  0000 C CNN
F 2 "" H 7400 4200 50  0001 C CNN
F 3 "" H 7400 4200 50  0001 C CNN
	1    7400 4200
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R8
U 1 1 5C2D5915
P 6450 1100
F 0 "R8" H 6391 1054 50  0000 R CNN
F 1 "100k" H 6391 1145 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric" H 6450 1100 50  0001 C CNN
F 3 "~" H 6450 1100 50  0001 C CNN
	1    6450 1100
	1    0    0    1   
$EndComp
$Comp
L Switch:SW_SPST SW1
U 1 1 5C2D591C
P 6250 1550
F 0 "SW1" H 6250 1785 50  0000 C CNN
F 1 "ENABLE" H 6250 1694 50  0000 C CNN
F 2 "Button_Switch_SMD:SW_SPST_PTS810" H 6250 1550 50  0001 C CNN
F 3 "" H 6250 1550 50  0001 C CNN
	1    6250 1550
	-1   0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR020
U 1 1 5C2D5923
P 6450 1000
F 0 "#PWR020" H 6450 850 50  0001 C CNN
F 1 "+3V3" H 6465 1173 50  0000 C CNN
F 2 "" H 6450 1000 50  0001 C CNN
F 3 "" H 6450 1000 50  0001 C CNN
	1    6450 1000
	-1   0    0    -1  
$EndComp
Wire Wire Line
	6800 1550 6450 1550
$Comp
L power:GND #PWR017
U 1 1 5C2D592C
P 6050 1600
F 0 "#PWR017" H 6050 1350 50  0001 C CNN
F 1 "GND" H 6055 1427 50  0000 C CNN
F 2 "" H 6050 1600 50  0001 C CNN
F 3 "" H 6050 1600 50  0001 C CNN
	1    6050 1600
	-1   0    0    -1  
$EndComp
Wire Wire Line
	6050 1550 6050 1600
$Comp
L Device:C_Small C9
U 1 1 5C32A69A
P 7200 1000
F 0 "C9" H 7109 954 50  0000 R CNN
F 1 "10uF" H 7109 1045 50  0000 R CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 7200 1000 50  0001 C CNN
F 3 "~" H 7200 1000 50  0001 C CNN
	1    7200 1000
	1    0    0    1   
$EndComp
$Comp
L power:GND #PWR022
U 1 1 5C34020B
P 7200 1100
F 0 "#PWR022" H 7200 850 50  0001 C CNN
F 1 "GND" H 7205 927 50  0000 C CNN
F 2 "" H 7200 1100 50  0001 C CNN
F 3 "" H 7200 1100 50  0001 C CNN
	1    7200 1100
	1    0    0    -1  
$EndComp
Wire Wire Line
	7400 900  7400 1350
Wire Wire Line
	7200 900  7400 900 
Connection ~ 7400 900 
NoConn ~ 6800 3250
NoConn ~ 6800 3150
NoConn ~ 6800 3050
NoConn ~ 6800 2950
NoConn ~ 6800 2850
NoConn ~ 6800 2750
Text Notes 7400 1350 0    35   ~ 0
(already has small \n  bypass caps)
NoConn ~ 6800 1750
NoConn ~ 6800 1850
Wire Wire Line
	8100 1850 8000 1850
Wire Wire Line
	8100 1650 8000 1650
Wire Wire Line
	8100 3550 8000 3550
Wire Wire Line
	8100 3150 8000 3150
Wire Wire Line
	8100 2950 8000 2950
Wire Wire Line
	8100 2650 8000 2650
Wire Wire Line
	8100 1950 8000 1950
Wire Wire Line
	7400 4150 7400 4200
$Comp
L Battery_Management:MCP73831-2-OT U4
U 1 1 5C0FC083
P 8800 5700
F 0 "U4" H 8550 5950 50  0000 C CNN
F 1 "MCP73831-2-OT" H 9150 5450 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23-5" H 8850 5450 50  0001 L CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/20001984g.pdf" H 8650 5650 50  0001 C CNN
	1    8800 5700
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J4
U 1 1 5C100E3E
P 9800 6450
F 0 "J4" H 9879 6442 50  0000 L CNN
F 1 "1S_BATTERY" H 9879 6351 50  0000 L CNN
F 2 "Connector_JST:JST_PH_S2B-PH-SM4-TB_1x02-1MP_P2.00mm_Horizontal" H 9800 6450 50  0001 C CNN
F 3 "~" H 9800 6450 50  0001 C CNN
	1    9800 6450
	-1   0    0    1   
$EndComp
$Comp
L power:+BATT #PWR037
U 1 1 5C10184F
P 10000 6350
F 0 "#PWR037" H 10000 6200 50  0001 C CNN
F 1 "+BATT" H 10015 6523 50  0000 C CNN
F 2 "" H 10000 6350 50  0001 C CNN
F 3 "" H 10000 6350 50  0001 C CNN
	1    10000 6350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR035
U 1 1 5C101ACA
P 10000 6450
F 0 "#PWR035" H 10000 6200 50  0001 C CNN
F 1 "GND" H 10005 6277 50  0000 C CNN
F 2 "" H 10000 6450 50  0001 C CNN
F 3 "" H 10000 6450 50  0001 C CNN
	1    10000 6450
	1    0    0    -1  
$EndComp
$Comp
L power:+BATT #PWR036
U 1 1 5C10E544
P 9250 5350
F 0 "#PWR036" H 9250 5200 50  0001 C CNN
F 1 "+BATT" H 9265 5523 50  0000 C CNN
F 2 "" H 9250 5350 50  0001 C CNN
F 3 "" H 9250 5350 50  0001 C CNN
	1    9250 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	9200 5600 9250 5600
$Comp
L Connector:USB_B_Micro J2
U 1 1 5C148A17
P 7650 5700
F 0 "J2" H 7650 6150 50  0000 C CNN
F 1 "USB_B_Micro" H 7650 6050 50  0000 C CNN
F 2 "Connector_USB:USB_Micro-B_Amphenol_10103594-0001LF_Horizontal" H 7800 5650 50  0001 C CNN
F 3 "~" H 7800 5650 50  0001 C CNN
	1    7650 5700
	1    0    0    -1  
$EndComp
NoConn ~ 7950 5700
NoConn ~ 7950 5800
NoConn ~ 7950 5900
$Comp
L power:VBUS #PWR031
U 1 1 5C16DB9C
P 8000 5400
F 0 "#PWR031" H 8000 5250 50  0001 C CNN
F 1 "VBUS" H 8015 5573 50  0000 C CNN
F 2 "" H 8000 5400 50  0001 C CNN
F 3 "" H 8000 5400 50  0001 C CNN
	1    8000 5400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR030
U 1 1 5C17A466
P 7650 6100
F 0 "#PWR030" H 7650 5850 50  0001 C CNN
F 1 "GND" H 7655 5927 50  0000 C CNN
F 2 "" H 7650 6100 50  0001 C CNN
F 3 "" H 7650 6100 50  0001 C CNN
	1    7650 6100
	1    0    0    -1  
$EndComp
NoConn ~ 7550 6100
$Comp
L power:VBUS #PWR033
U 1 1 5C186C6B
P 8800 5400
F 0 "#PWR033" H 8800 5250 50  0001 C CNN
F 1 "VBUS" H 8815 5573 50  0000 C CNN
F 2 "" H 8800 5400 50  0001 C CNN
F 3 "" H 8800 5400 50  0001 C CNN
	1    8800 5400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR034
U 1 1 5C186E1A
P 8800 6000
F 0 "#PWR034" H 8800 5750 50  0001 C CNN
F 1 "GND" H 8805 5827 50  0000 C CNN
F 2 "" H 8800 6000 50  0001 C CNN
F 3 "" H 8800 6000 50  0001 C CNN
	1    8800 6000
	1    0    0    -1  
$EndComp
Text Label 9650 5800 0    50   ~ 0
CHG_STAT
Wire Wire Line
	8000 3250 8100 3250
$Comp
L Device:R_Small R_CHG1
U 1 1 5C1A0AAF
P 8400 5900
F 0 "R_CHG1" H 8342 5854 50  0000 R CNN
F 1 "2k2" H 8342 5945 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric" H 8400 5900 50  0001 C CNN
F 3 "~" H 8400 5900 50  0001 C CNN
	1    8400 5900
	1    0    0    1   
$EndComp
Wire Wire Line
	8800 6000 8400 6000
Connection ~ 8800 6000
Text Notes 8500 6350 0    35   ~ 0
1000/R_CHG = 450mA
$Comp
L Device:C_Small C10
U 1 1 5C1BAF95
P 10100 5450
F 0 "C10" H 10009 5404 50  0000 R CNN
F 1 "4.7uF" H 10009 5495 50  0000 R CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 10100 5450 50  0001 C CNN
F 3 "~" H 10100 5450 50  0001 C CNN
	1    10100 5450
	-1   0    0    1   
$EndComp
$Comp
L Device:R_Small R14
U 1 1 5C1D534F
P 9400 5700
F 0 "R14" H 9341 5654 50  0000 R CNN
F 1 "100k" H 9341 5745 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric" H 9400 5700 50  0001 C CNN
F 3 "~" H 9400 5700 50  0001 C CNN
	1    9400 5700
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR039
U 1 1 5C1FD05B
P 10100 5550
F 0 "#PWR039" H 10100 5300 50  0001 C CNN
F 1 "GND" H 10105 5377 50  0000 C CNN
F 2 "" H 10100 5550 50  0001 C CNN
F 3 "" H 10100 5550 50  0001 C CNN
	1    10100 5550
	1    0    0    -1  
$EndComp
Wire Notes Line
	7350 5100 7350 6700
Wire Notes Line
	10450 6700 10450 5100
Text Notes 8650 5100 0    50   ~ 0
Charging
$Comp
L power:+BATT #PWR06
U 1 1 5C12AD2A
P 1750 6050
F 0 "#PWR06" H 1750 5900 50  0001 C CNN
F 1 "+BATT" H 1765 6223 50  0000 C CNN
F 2 "" H 1750 6050 50  0001 C CNN
F 3 "" H 1750 6050 50  0001 C CNN
	1    1750 6050
	1    0    0    -1  
$EndComp
Wire Wire Line
	8000 2550 8100 2550
Wire Wire Line
	8100 2750 8000 2750
NoConn ~ 8000 3850
NoConn ~ 8000 3750
Text Label 8100 2350 0    50   ~ 0
CHG_STAT
Text Notes 8050 3850 0    35   ~ 0
34/35 are\ninput only
$Comp
L Connector:Conn_01x08_Female J3
U 1 1 5C1A6CF7
P 5400 7200
F 0 "J3" H 5427 7176 50  0000 L CNN
F 1 "CASTELLATION" H 5427 7085 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x08_P2.54mm_Vertical" H 5400 7200 50  0001 C CNN
F 3 "~" H 5400 7200 50  0001 C CNN
	1    5400 7200
	1    0    0    -1  
$EndComp
Text Label 5000 7300 0    50   ~ 0
eCS
Text Label 5000 7400 0    50   ~ 0
eDC
Text Label 4900 7600 0    50   ~ 0
eBUSY
Text Label 4950 7500 0    50   ~ 0
eRST
Text Label 4950 7200 0    50   ~ 0
eCLK
Text Label 4950 7100 0    50   ~ 0
eDIN
Wire Wire Line
	5200 7100 4950 7100
Wire Wire Line
	4950 7200 5200 7200
Wire Wire Line
	5200 7300 5000 7300
Wire Wire Line
	5200 7400 5000 7400
Wire Wire Line
	5200 7500 4950 7500
Wire Wire Line
	5200 7600 4900 7600
Wire Wire Line
	8100 3050 8000 3050
Connection ~ 6450 1550
Wire Wire Line
	6450 1200 6450 1550
Wire Wire Line
	8350 1450 8350 1200
Connection ~ 8350 1450
$Comp
L Sensor_Humidity:Si7021-A20 U5
U 1 1 5C3D9EFA
P 5150 5050
F 0 "U5" H 5591 5096 50  0000 L CNN
F 1 "Si7021-A20" H 5591 5005 50  0000 L CNN
F 2 "Package_DFN_QFN:DFN-6-1EP_3x3mm_P1mm_EP1.5x2.4mm" H 5150 4650 50  0001 C CNN
F 3 "https://www.silabs.com/documents/public/data-sheets/Si7021-A20.pdf" H 4950 5350 50  0001 C CNN
	1    5150 5050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 5C3DA5DF
P 5050 5350
F 0 "#PWR0102" H 5050 5100 50  0001 C CNN
F 1 "GND" H 5055 5177 50  0000 C CNN
F 2 "" H 5050 5350 50  0001 C CNN
F 3 "" H 5050 5350 50  0001 C CNN
	1    5050 5350
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR0103
U 1 1 5C3DA812
P 5150 4700
F 0 "#PWR0103" H 5150 4550 50  0001 C CNN
F 1 "+3V3" H 5165 4873 50  0000 C CNN
F 2 "" H 5150 4700 50  0001 C CNN
F 3 "" H 5150 4700 50  0001 C CNN
	1    5150 4700
	1    0    0    -1  
$EndComp
Text Label 4000 4900 2    50   ~ 0
SDA
Text Label 4000 5150 2    50   ~ 0
SCL
Text Label 8100 2950 0    50   ~ 0
SDA
Text Label 8100 3050 0    50   ~ 0
SCL
Wire Wire Line
	8000 2350 8100 2350
$Comp
L Device:R_Small R15
U 1 1 5C401CE9
P 4400 4800
F 0 "R15" H 4459 4846 50  0000 L CNN
F 1 "10k" H 4459 4755 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" H 4400 4800 50  0001 C CNN
F 3 "~" H 4400 4800 50  0001 C CNN
	1    4400 4800
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R11
U 1 1 5C401E8D
P 4250 5050
F 0 "R11" H 4309 5096 50  0000 L CNN
F 1 "10k" H 4309 5005 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" H 4250 5050 50  0001 C CNN
F 3 "~" H 4250 5050 50  0001 C CNN
	1    4250 5050
	1    0    0    -1  
$EndComp
Connection ~ 4250 5150
Wire Wire Line
	4250 5150 4650 5150
Wire Wire Line
	4400 4950 4400 4900
Wire Wire Line
	4400 4950 4650 4950
Wire Wire Line
	4400 4900 4000 4900
Connection ~ 4400 4900
Wire Wire Line
	4000 5150 4250 5150
Wire Wire Line
	4250 4950 4250 4700
Wire Wire Line
	4250 4700 4400 4700
Wire Wire Line
	4400 4700 5150 4700
Connection ~ 4400 4700
Wire Wire Line
	5150 4750 5150 4700
Connection ~ 5150 4700
Connection ~ 9400 5800
Wire Wire Line
	9250 5600 9250 5350
Wire Wire Line
	9200 5800 9400 5800
Connection ~ 9250 5350
$Comp
L power:+3V3 #PWR08
U 1 1 5C6E1ADB
P 9400 5600
F 0 "#PWR08" H 9400 5450 50  0001 C CNN
F 1 "+3V3" H 9415 5773 50  0000 C CNN
F 2 "" H 9400 5600 50  0001 C CNN
F 3 "" H 9400 5600 50  0001 C CNN
	1    9400 5600
	1    0    0    -1  
$EndComp
Wire Wire Line
	9400 5800 9650 5800
Wire Wire Line
	9250 5350 10100 5350
Wire Notes Line
	10450 5100 7350 5100
Wire Notes Line
	7350 6700 10450 6700
$Comp
L Device:C_Small C2
U 1 1 5C70EFAE
P 1600 6450
F 0 "C2" H 1509 6496 50  0000 R CNN
F 1 "0.1uF" H 1509 6405 50  0000 R CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 1600 6450 50  0001 C CNN
F 3 "~" H 1600 6450 50  0001 C CNN
	1    1600 6450
	1    0    0    -1  
$EndComp
Wire Wire Line
	1750 6250 1750 6300
Wire Wire Line
	1350 6300 1600 6300
Connection ~ 1750 6300
Wire Wire Line
	1750 6300 1750 6350
Wire Wire Line
	1600 6350 1600 6300
Connection ~ 1600 6300
Wire Wire Line
	1600 6300 1750 6300
Wire Wire Line
	1600 6550 1600 6600
Wire Wire Line
	1600 6600 1750 6600
Connection ~ 1750 6600
Wire Notes Line
	550  5700 550  6900
Wire Notes Line
	550  6900 2100 6900
Wire Notes Line
	2100 6900 2100 5700
Wire Notes Line
	2100 5700 550  5700
NoConn ~ 8000 2150
Text Notes 7100 2350 0    39   ~ 0
Bootstrap GPIO's\n0, 2, 5, 12, 15
$Comp
L Connector_Generic:Conn_01x04 J5
U 1 1 5C6E341A
P 2400 3400
F 0 "J5" H 2479 3392 50  0000 L CNN
F 1 "RECEIVER" H 2479 3301 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x04_P2.54mm_Vertical" H 2400 3400 50  0001 C CNN
F 3 "~" H 2400 3400 50  0001 C CNN
	1    2400 3400
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR05
U 1 1 5C6F2571
P 2050 3250
F 0 "#PWR05" H 2050 3100 50  0001 C CNN
F 1 "+3V3" H 2065 3423 50  0000 C CNN
F 2 "" H 2050 3250 50  0001 C CNN
F 3 "" H 2050 3250 50  0001 C CNN
	1    2050 3250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR01
U 1 1 5C6F25B2
P 2000 4250
F 0 "#PWR01" H 2000 4000 50  0001 C CNN
F 1 "GND" H 2005 4077 50  0000 C CNN
F 2 "" H 2000 4250 50  0001 C CNN
F 3 "" H 2000 4250 50  0001 C CNN
	1    2000 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	2200 3400 2200 3500
Connection ~ 2200 3400
Text Label 2000 3400 2    50   ~ 0
RADIO
Wire Wire Line
	2200 3400 2000 3400
NoConn ~ 8000 3650
Text Notes 2200 3200 0    50   ~ 0
4.8mA - 5.6mA during use
Wire Wire Line
	2200 3700 2200 3600
Text Label 1650 3900 2    50   ~ 0
RADIO_ENABLE
Text Label 8100 3350 0    50   ~ 0
RADIO_ENABLE
Wire Wire Line
	8000 3350 8100 3350
$Comp
L Device:R_Small R1
U 1 1 5C717910
P 1800 4100
F 0 "R1" H 1859 4146 50  0000 L CNN
F 1 "100k" H 1859 4055 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric" H 1800 4100 50  0001 C CNN
F 3 "~" H 1800 4100 50  0001 C CNN
	1    1800 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	1900 3900 1800 3900
Wire Wire Line
	1800 4000 1800 3900
Connection ~ 1800 3900
Wire Wire Line
	1800 3900 1650 3900
Wire Wire Line
	2200 3300 2050 3300
Wire Wire Line
	2050 3300 2050 3250
Wire Wire Line
	2200 4100 2200 4250
Wire Wire Line
	2200 4250 2000 4250
Wire Wire Line
	2000 4250 1800 4250
Wire Wire Line
	1800 4250 1800 4200
Connection ~ 2000 4250
$Comp
L Device:Q_NMOS_GDS Q1
U 1 1 5C7315C9
P 2100 3900
F 0 "Q1" H 2305 3946 50  0000 L CNN
F 1 "ZXMN3A01Z" H 2305 3855 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-89-3_Handsoldering" H 2300 4000 50  0001 C CNN
F 3 "~" H 2100 3900 50  0001 C CNN
	1    2100 3900
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J1
U 1 1 5C732064
P 1100 5250
F 0 "J1" H 1179 5292 50  0000 L CNN
F 1 "SERIAL" H 1179 5201 50  0000 L CNN
F 2 "!Custom:SAMTEC-TSM-103-01-X-SH" H 1100 5250 50  0001 C CNN
F 3 "~" H 1100 5250 50  0001 C CNN
	1    1100 5250
	1    0    0    -1  
$EndComp
Wire Wire Line
	900  5150 750  5150
Wire Wire Line
	900  5250 650  5250
Wire Wire Line
	650  5250 650  5350
Wire Wire Line
	750  5350 900  5350
Wire Notes Line
	550  5050 1450 5050
Wire Notes Line
	1450 5050 1450 5600
Wire Notes Line
	1450 5600 550  5600
Wire Notes Line
	550  5600 550  5050
$Comp
L power:GNDA #PWR09
U 1 1 5C70200F
P 5250 5500
F 0 "#PWR09" H 5250 5250 50  0001 C CNN
F 1 "GNDA" H 5255 5327 50  0000 C CNN
F 2 "" H 5250 5500 50  0001 C CNN
F 3 "" H 5250 5500 50  0001 C CNN
	1    5250 5500
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 5350 5250 5500
NoConn ~ 8000 3450
NoConn ~ 8000 2450
$Comp
L Device:R_Small R2
U 1 1 5C916603
P 1250 6300
F 0 "R2" V 1054 6300 50  0000 C CNN
F 1 "10k" V 1145 6300 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric" H 1250 6300 50  0001 C CNN
F 3 "~" H 1250 6300 50  0001 C CNN
	1    1250 6300
	0    1    1    0   
$EndComp
$Comp
L Device:C_Small C4
U 1 1 5C917B24
P 8400 5400
F 0 "C4" H 8500 5450 50  0000 L CNN
F 1 "4.7uF" H 8500 5350 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 8400 5400 50  0001 C CNN
F 3 "~" H 8400 5400 50  0001 C CNN
	1    8400 5400
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR011
U 1 1 5C920878
P 8400 5500
F 0 "#PWR011" H 8400 5250 50  0001 C CNN
F 1 "GND" H 8405 5327 50  0000 C CNN
F 2 "" H 8400 5500 50  0001 C CNN
F 3 "" H 8400 5500 50  0001 C CNN
	1    8400 5500
	1    0    0    -1  
$EndComp
Wire Wire Line
	8000 5400 8000 5500
Wire Wire Line
	8000 5500 7950 5500
Wire Wire Line
	8400 5300 8650 5300
Wire Wire Line
	8650 5300 8650 5400
Wire Wire Line
	8650 5400 8800 5400
Connection ~ 8800 5400
$Comp
L Regulator_Linear:TLV75533PDBV U1
U 1 1 5C92A3E3
P 1350 7450
F 0 "U1" H 1350 7792 50  0000 C CNN
F 1 "TLV75533PDBV" H 1350 7701 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23-5" H 1350 7750 50  0001 C CIN
F 3 "http://www.ti.com/lit/ds/symlink/tlv755p.pdf" H 1350 7450 50  0001 C CNN
	1    1350 7450
	1    0    0    -1  
$EndComp
Wire Wire Line
	1050 7350 1050 7450
$Comp
L power:+BATT #PWR02
U 1 1 5C92E623
P 850 7350
F 0 "#PWR02" H 850 7200 50  0001 C CNN
F 1 "+BATT" H 865 7523 50  0000 C CNN
F 2 "" H 850 7350 50  0001 C CNN
F 3 "" H 850 7350 50  0001 C CNN
	1    850  7350
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C1
U 1 1 5C938205
P 850 7450
F 0 "C1" H 950 7400 50  0000 L CNN
F 1 "1uF" H 950 7500 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 850 7450 50  0001 C CNN
F 3 "~" H 850 7450 50  0001 C CNN
	1    850  7450
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR04
U 1 1 5C939382
P 1350 7750
F 0 "#PWR04" H 1350 7500 50  0001 C CNN
F 1 "GND" H 1355 7577 50  0000 C CNN
F 2 "" H 1350 7750 50  0001 C CNN
F 3 "" H 1350 7750 50  0001 C CNN
	1    1350 7750
	1    0    0    -1  
$EndComp
Wire Wire Line
	1350 7750 850  7750
Wire Wire Line
	850  7750 850  7550
Connection ~ 1350 7750
$Comp
L Device:C_Small C3
U 1 1 5C93CE31
P 1650 7450
F 0 "C3" H 1750 7500 50  0000 L CNN
F 1 "1uF" H 1750 7400 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 1650 7450 50  0001 C CNN
F 3 "~" H 1650 7450 50  0001 C CNN
	1    1650 7450
	1    0    0    -1  
$EndComp
Wire Wire Line
	1050 7350 850  7350
Connection ~ 1050 7350
Connection ~ 850  7350
Wire Wire Line
	1650 7550 1650 7750
Wire Wire Line
	1650 7750 1350 7750
Text Label 1900 7350 2    50   ~ 0
3V3
Wire Wire Line
	1900 7350 1650 7350
Connection ~ 1650 7350
Wire Notes Line
	1950 7050 1950 8000
Wire Notes Line
	550  7050 550  8000
$Comp
L RF_Module:ESP32-WROOM-32D U3
U 1 1 5C0E1BD0
P 7400 2750
F 0 "U3" H 6950 4100 50  0000 C CNN
F 1 "ESP32-WROOM-32D" H 7800 4100 50  0000 C CNN
F 2 "RF_Module:ESP32-WROOM-32" H 7400 1250 50  0001 C CNN
F 3 "https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32d_esp32-wroom-32u_datasheet_en.pdf" H 7100 2800 50  0001 C CNN
	1    7400 2750
	1    0    0    -1  
$EndComp
Text Label 8100 2050 0    50   ~ 0
eCS
Wire Wire Line
	8000 2050 8100 2050
NoConn ~ 8000 2850
Text Label 4950 7000 0    50   ~ 0
GND
Wire Wire Line
	5200 7000 4950 7000
Text Label 8100 2250 0    50   ~ 0
DISPLAY_ENABLE
Wire Wire Line
	8000 2250 8100 2250
Text Label 3500 7100 2    50   ~ 0
DISPLAY_ENABLE
$Comp
L Device:R_Small R5
U 1 1 5C9F2C09
P 4000 6600
F 0 "R5" H 3941 6646 50  0000 R CNN
F 1 "10k" H 3941 6555 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric" H 4000 6600 50  0001 C CNN
F 3 "~" H 4000 6600 50  0001 C CNN
	1    4000 6600
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R6
U 1 1 5C99F85D
P 3700 7200
F 0 "R6" H 3642 7246 50  0000 R CNN
F 1 "100k" H 3642 7155 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric" H 3700 7200 50  0001 C CNN
F 3 "~" H 3700 7200 50  0001 C CNN
	1    3700 7200
	1    0    0    -1  
$EndComp
$Comp
L Transistor_FET:DMC2053UVT Q2
U 1 1 5C9C61EE
P 4000 7100
F 0 "Q2" H 4106 7146 50  0000 L CNN
F 1 "DMC2053UVT" H 4106 7055 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:TSOT-23-6" H 3950 6625 50  0001 C CNN
F 3 "https://www.diodes.com/assets/Datasheets/DMC2053UVT.pdf" H 3200 6300 50  0001 C CNN
	1    4000 7100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 5C9CCC60
P 4000 7300
F 0 "#PWR0101" H 4000 7050 50  0001 C CNN
F 1 "GND" H 4005 7127 50  0000 C CNN
F 2 "" H 4000 7300 50  0001 C CNN
F 3 "" H 4000 7300 50  0001 C CNN
	1    4000 7300
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 7300 4000 7300
Connection ~ 4000 7300
Connection ~ 3700 7100
Wire Wire Line
	3500 7100 3700 7100
$Comp
L power:+BATT #PWR0104
U 1 1 5CA176A5
P 4300 6500
F 0 "#PWR0104" H 4300 6350 50  0001 C CNN
F 1 "+BATT" H 4315 6673 50  0000 C CNN
F 2 "" H 4300 6500 50  0001 C CNN
F 3 "" H 4300 6500 50  0001 C CNN
	1    4300 6500
	1    0    0    -1  
$EndComp
$Comp
L Transistor_FET:DMC2053UVT Q2
U 2 1 5C9C88B6
P 4300 6700
F 0 "Q2" H 4406 6746 50  0000 L CNN
F 1 "DMC2053UVT" H 4406 6655 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:TSOT-23-6" H 4250 6225 50  0001 C CNN
F 3 "https://www.diodes.com/assets/Datasheets/DMC2053UVT.pdf" H 3500 5900 50  0001 C CNN
	2    4300 6700
	1    0    0    1   
$EndComp
Wire Wire Line
	4000 6500 4300 6500
Connection ~ 4300 6500
Wire Wire Line
	4000 6700 4000 6900
Connection ~ 4000 6700
Wire Wire Line
	4300 6900 5200 6900
$Comp
L power:+3V3 #PWR010
U 1 1 5C073DCF
P 2100 7650
F 0 "#PWR010" H 2100 7500 50  0001 C CNN
F 1 "+3V3" H 2115 7823 50  0000 C CNN
F 2 "" H 2100 7650 50  0001 C CNN
F 3 "" H 2100 7650 50  0001 C CNN
	1    2100 7650
	1    0    0    -1  
$EndComp
Text Label 2300 7650 2    50   ~ 0
3V3
Wire Wire Line
	2100 7650 2300 7650
$Comp
L power:GND #PWR013
U 1 1 5C1A775D
P 2100 7800
F 0 "#PWR013" H 2100 7550 50  0001 C CNN
F 1 "GND" H 2105 7627 50  0000 C CNN
F 2 "" H 2100 7800 50  0001 C CNN
F 3 "" H 2100 7800 50  0001 C CNN
	1    2100 7800
	1    0    0    -1  
$EndComp
Text Label 2200 7800 0    50   ~ 0
GND
Wire Wire Line
	2100 7800 2200 7800
$EndSCHEMATC