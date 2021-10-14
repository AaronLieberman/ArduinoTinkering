EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
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
$Comp
L Seeeduino_XIAO:SeeeduinoXIAO U1
U 1 1 6165F8D1
P 6300 3950
F 0 "U1" H 6275 3011 50  0000 C CNN
F 1 "SeeeduinoXIAO" H 6275 2920 50  0000 C CNN
F 2 "Seeeduino_XIAO:Seeeduino XIAO-MOUDLE14P-2.54-21X17.8MM" H 5950 4150 50  0001 C CNN
F 3 "" H 5950 4150 50  0001 C CNN
	1    6300 3950
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR07
U 1 1 61671FEA
P 7650 3800
F 0 "#PWR07" H 7650 3650 50  0001 C CNN
F 1 "+3V3" H 7665 3973 50  0000 C CNN
F 2 "" H 7650 3800 50  0001 C CNN
F 3 "" H 7650 3800 50  0001 C CNN
	1    7650 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	7100 3800 7650 3800
$Comp
L power:GND #PWR06
U 1 1 61672DC1
P 7450 3650
F 0 "#PWR06" H 7450 3400 50  0001 C CNN
F 1 "GND" H 7455 3477 50  0000 C CNN
F 2 "" H 7450 3650 50  0001 C CNN
F 3 "" H 7450 3650 50  0001 C CNN
	1    7450 3650
	-1   0    0    1   
$EndComp
Wire Wire Line
	7100 3650 7450 3650
Text GLabel 7450 4250 2    50   Input ~ 0
INT2
Text GLabel 7450 4100 2    50   Input ~ 0
INT1
Wire Wire Line
	7100 4100 7450 4100
Wire Wire Line
	7450 4250 7100 4250
Text GLabel 5300 4100 0    50   Input ~ 0
SDA
Text GLabel 5300 4250 0    50   Input ~ 0
SCL
Wire Wire Line
	5300 4100 5450 4100
Wire Wire Line
	5450 4250 5300 4250
$Comp
L LED:NeoPixel_THT Pixel1
U 1 1 6169F8F8
P 8900 1600
F 0 "Pixel1" V 8854 1944 50  0000 L CNN
F 1 "NeoPixel_THT" V 8945 1944 50  0000 L CNN
F 2 "LED_SMD:LED_SK6812MINI_PLCC4_3.5x3.5mm_P1.75mm" H 8950 1300 50  0001 L TNN
F 3 "https://www.adafruit.com/product/1938" H 9000 1225 50  0001 L TNN
	1    8900 1600
	0    1    1    0   
$EndComp
$Comp
L Device:R R3
U 1 1 616A1457
P 5100 3250
F 0 "R3" H 5170 3296 50  0000 L CNN
F 1 "400" H 5170 3205 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 5030 3250 50  0001 C CNN
F 3 "~" H 5100 3250 50  0001 C CNN
	1    5100 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 3650 5100 3650
Wire Wire Line
	5100 3650 5100 3400
$Comp
L power:+3V3 #PWR011
U 1 1 616A7416
P 9200 1350
F 0 "#PWR011" H 9200 1200 50  0001 C CNN
F 1 "+3V3" H 9215 1523 50  0000 C CNN
F 2 "" H 9200 1350 50  0001 C CNN
F 3 "" H 9200 1350 50  0001 C CNN
	1    9200 1350
	1    0    0    -1  
$EndComp
$Comp
L LED:NeoPixel_THT PIXEL4
U 1 1 616C9B9C
P 10350 1600
F 0 "PIXEL4" V 10396 1256 50  0000 R CNN
F 1 "NeoPixel_THT" V 10305 1256 50  0000 R CNN
F 2 "LED_SMD:LED_SK6812MINI_PLCC4_3.5x3.5mm_P1.75mm" H 10400 1300 50  0001 L TNN
F 3 "https://www.adafruit.com/product/1938" H 10450 1225 50  0001 L TNN
	1    10350 1600
	0    -1   -1   0   
$EndComp
$Comp
L Jumper:Jumper_2_Open JP2
U 1 1 616CB68A
P 9650 3000
F 0 "JP2" H 9650 3235 50  0000 C CNN
F 1 "Jumper_2_Open" H 9650 3144 50  0000 C CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_RoundedPad1.0x1.5mm" H 9650 3000 50  0001 C CNN
F 3 "~" H 9650 3000 50  0001 C CNN
	1    9650 3000
	-1   0    0    1   
$EndComp
Wire Wire Line
	9850 3000 10050 3000
Wire Wire Line
	10350 3000 10350 3350
Wire Wire Line
	10350 3350 8900 3350
Wire Wire Line
	8900 3350 8900 3000
Wire Wire Line
	8900 1150 8900 1300
$Comp
L power:GND #PWR014
U 1 1 616C9BA8
P 10650 3050
F 0 "#PWR014" H 10650 2800 50  0001 C CNN
F 1 "GND" H 10655 2877 50  0000 C CNN
F 2 "" H 10650 3050 50  0001 C CNN
F 3 "" H 10650 3050 50  0001 C CNN
	1    10650 3050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 616F33F2
P 8600 3050
F 0 "#PWR0101" H 8600 2800 50  0001 C CNN
F 1 "GND" H 8605 2877 50  0000 C CNN
F 2 "" H 8600 3050 50  0001 C CNN
F 3 "" H 8600 3050 50  0001 C CNN
	1    8600 3050
	1    0    0    -1  
$EndComp
$Comp
L BAT-HLD-001-THM:BAT-HLD-001-THM BT2
U 1 1 617077E9
P 8200 4550
F 0 "BT2" V 8154 4680 50  0000 L CNN
F 1 "BAT-HLD-001-THM" V 8245 4680 50  0000 L CNN
F 2 "BAT-HLD-001-THM:BAT_BAT-HLD-001-THM" H 8200 4550 50  0001 L BNN
F 3 "" H 8200 4550 50  0001 L BNN
F 4 "Manufacturer Recommendations" H 8200 4550 50  0001 L BNN "STANDARD"
F 5 "C" H 8200 4550 50  0001 L BNN "PARTREV"
F 6 "Linx Technologies" H 8200 4550 50  0001 L BNN "MANUFACTURER"
F 7 "4.2mm" H 8200 4550 50  0001 L BNN "MAXIMUM_PACKAGE_HEIGHT"
	1    8200 4550
	0    1    1    0   
$EndComp
Wire Wire Line
	8900 2400 8900 1900
Wire Wire Line
	8600 2700 8600 1600
Connection ~ 8600 2700
$Comp
L LED:NeoPixel_THT PIXEL2
U 1 1 616B291A
P 8900 2700
F 0 "PIXEL2" V 8854 3044 50  0000 L CNN
F 1 "NeoPixel_THT" V 8945 3044 50  0000 L CNN
F 2 "LED_SMD:LED_SK6812MINI_PLCC4_3.5x3.5mm_P1.75mm" H 8950 2400 50  0001 L TNN
F 3 "https://www.adafruit.com/product/1938" H 9000 2325 50  0001 L TNN
	1    8900 2700
	0    1    1    0   
$EndComp
Wire Wire Line
	8600 2700 8600 3050
Wire Wire Line
	9200 1350 9200 1600
Wire Wire Line
	9200 1600 9200 2700
Connection ~ 9200 1600
Wire Wire Line
	9200 2700 9200 3000
Wire Wire Line
	9200 3000 9450 3000
Connection ~ 9200 2700
Wire Wire Line
	10050 1600 10050 2700
Wire Wire Line
	10650 1600 10650 2700
$Comp
L LED:NeoPixel_THT PIXEL3
U 1 1 6175018F
P 10350 2700
F 0 "PIXEL3" V 10396 2356 50  0000 R CNN
F 1 "NeoPixel_THT" V 10305 2356 50  0000 R CNN
F 2 "LED_SMD:LED_SK6812MINI_PLCC4_3.5x3.5mm_P1.75mm" H 10400 2400 50  0001 L TNN
F 3 "https://www.adafruit.com/product/1938" H 10450 2325 50  0001 L TNN
	1    10350 2700
	0    -1   -1   0   
$EndComp
Connection ~ 10650 2700
Wire Wire Line
	10650 2700 10650 3050
Connection ~ 10050 2700
Wire Wire Line
	10050 2700 10050 3000
Wire Wire Line
	10350 2400 10350 1900
$Comp
L Device:D D1
U 1 1 61765328
P 8200 3500
F 0 "D1" V 8154 3580 50  0000 L CNN
F 1 "D" V 8245 3580 50  0000 L CNN
F 2 "Diode_SMD:D_SOD-323" H 8200 3500 50  0001 C CNN
F 3 "~" H 8200 3500 50  0001 C CNN
	1    8200 3500
	0    1    1    0   
$EndComp
$Comp
L BAT-HLD-001-THM:BAT-HLD-001-THM BT1
U 1 1 61708CB4
P 8200 3950
F 0 "BT1" V 8154 4080 50  0000 L CNN
F 1 "BAT-HLD-001-THM" V 8245 4080 50  0000 L CNN
F 2 "BAT-HLD-001-THM:BAT_BAT-HLD-001-THM" H 8200 3950 50  0001 L BNN
F 3 "" H 8200 3950 50  0001 L BNN
F 4 "Manufacturer Recommendations" H 8200 3950 50  0001 L BNN "STANDARD"
F 5 "C" H 8200 3950 50  0001 L BNN "PARTREV"
F 6 "Linx Technologies" H 8200 3950 50  0001 L BNN "MANUFACTURER"
F 7 "4.2mm" H 8200 3950 50  0001 L BNN "MAXIMUM_PACKAGE_HEIGHT"
	1    8200 3950
	0    1    1    0   
$EndComp
Wire Wire Line
	8200 3350 7250 3350
Wire Wire Line
	7250 3350 7250 3500
Wire Wire Line
	7250 3500 7100 3500
$Comp
L power:GND #PWR0102
U 1 1 617929E5
P 8200 4850
F 0 "#PWR0102" H 8200 4600 50  0001 C CNN
F 1 "GND" H 8205 4677 50  0000 C CNN
F 2 "" H 8200 4850 50  0001 C CNN
F 3 "" H 8200 4850 50  0001 C CNN
	1    8200 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	5100 3100 5100 2950
Wire Wire Line
	5100 2950 8400 2950
Wire Wire Line
	8400 2950 8400 1150
Wire Wire Line
	8400 1150 8900 1150
Wire Wire Line
	1800 4550 1800 4650
Wire Wire Line
	2000 4550 1800 4550
Wire Wire Line
	2700 4550 2300 4550
$Comp
L Device:R R2
U 1 1 6176EC78
P 2150 4550
F 0 "R2" H 2220 4596 50  0000 L CNN
F 1 "10K" H 2220 4505 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 2080 4550 50  0001 C CNN
F 3 "~" H 2150 4550 50  0001 C CNN
	1    2150 4550
	0    1    1    0   
$EndComp
Wire Wire Line
	1800 4250 1800 4150
Wire Wire Line
	2550 4350 2700 4350
Wire Wire Line
	2700 4450 2550 4450
Text GLabel 2550 4450 0    50   Input ~ 0
SCL
Text GLabel 2550 4350 0    50   Input ~ 0
SDA
Wire Wire Line
	3850 4450 3700 4450
Wire Wire Line
	3700 4350 3850 4350
$Comp
L Sensor_Motion:ADXL343 U2
U 1 1 61662620
P 3200 4350
F 0 "U2" H 3744 4724 50  0000 L CNN
F 1 "ADXL343" H 3744 4646 50  0000 L CNN
F 2 "Package_LGA:LGA-14_3x5mm_P0.8mm_LayoutBorder1x6y" H 3200 4350 50  0001 C CNN
F 3 "https://www.analog.com/media/en/technical-documentation/data-sheets/ADXL343.pdf" H 3200 4350 50  0001 C CNN
	1    3200 4350
	1    0    0    -1  
$EndComp
Text GLabel 3850 4350 2    50   Input ~ 0
INT1
Text GLabel 3850 4450 2    50   Input ~ 0
INT2
Wire Wire Line
	3400 3750 4600 3750
Wire Wire Line
	3200 5000 4600 5000
Wire Wire Line
	4600 3850 4600 3750
Wire Wire Line
	3200 5000 3200 5100
Connection ~ 3200 5000
Wire Wire Line
	4600 5000 4600 4150
$Comp
L Device:C C1
U 1 1 6168F775
P 4600 4000
F 0 "C1" H 4715 4046 50  0000 L CNN
F 1 "0.1uF" H 4715 3955 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 4638 3850 50  0001 C CNN
F 3 "~" H 4600 4000 50  0001 C CNN
	1    4600 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 3750 3400 3650
Connection ~ 3400 3750
Wire Wire Line
	3200 3750 3400 3750
Wire Wire Line
	3200 3850 3200 3750
Wire Wire Line
	3400 3850 3400 3750
$Comp
L power:+3V3 #PWR04
U 1 1 61689627
P 3400 3650
F 0 "#PWR04" H 3400 3500 50  0001 C CNN
F 1 "+3V3" H 3415 3823 50  0000 C CNN
F 2 "" H 3400 3650 50  0001 C CNN
F 3 "" H 3400 3650 50  0001 C CNN
	1    3400 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3200 4850 3200 5000
$Comp
L power:GND #PWR03
U 1 1 616882FF
P 3200 5100
F 0 "#PWR03" H 3200 4850 50  0001 C CNN
F 1 "GND" H 3205 4927 50  0000 C CNN
F 2 "" H 3200 5100 50  0001 C CNN
F 3 "" H 3200 5100 50  0001 C CNN
	1    3200 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	2300 4250 2700 4250
Wire Wire Line
	2000 4250 1800 4250
$Comp
L power:+3V3 #PWR02
U 1 1 6166C856
P 1800 4150
F 0 "#PWR02" H 1800 4000 50  0001 C CNN
F 1 "+3V3" H 1815 4323 50  0000 C CNN
F 2 "" H 1800 4150 50  0001 C CNN
F 3 "" H 1800 4150 50  0001 C CNN
	1    1800 4150
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 6166A991
P 2150 4250
F 0 "R1" H 2220 4296 50  0000 L CNN
F 1 "10K" H 2220 4205 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 2080 4250 50  0001 C CNN
F 3 "~" H 2150 4250 50  0001 C CNN
	1    2150 4250
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR01
U 1 1 616648C6
P 1800 4650
F 0 "#PWR01" H 1800 4400 50  0001 C CNN
F 1 "GND" H 1805 4477 50  0000 C CNN
F 2 "" H 1800 4650 50  0001 C CNN
F 3 "" H 1800 4650 50  0001 C CNN
	1    1800 4650
	1    0    0    -1  
$EndComp
$EndSCHEMATC
