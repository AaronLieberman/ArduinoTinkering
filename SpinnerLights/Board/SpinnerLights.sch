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
L Device:D D1
U 1 1 61660AAE
P 1700 4950
F 0 "D1" H 1700 5167 50  0000 C CNN
F 1 "D" H 1700 5076 50  0000 C CNN
F 2 "" H 1700 4950 50  0001 C CNN
F 3 "~" H 1700 4950 50  0001 C CNN
	1    1700 4950
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 616648C6
P 1700 6700
F 0 "#PWR?" H 1700 6450 50  0001 C CNN
F 1 "GND" H 1705 6527 50  0000 C CNN
F 2 "" H 1700 6700 50  0001 C CNN
F 3 "" H 1700 6700 50  0001 C CNN
	1    1700 6700
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 6166A991
P 2300 4850
F 0 "R1" H 2370 4896 50  0000 L CNN
F 1 "10K" H 2370 4805 50  0000 L CNN
F 2 "" V 2230 4850 50  0001 C CNN
F 3 "~" H 2300 4850 50  0001 C CNN
	1    2300 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	2700 4550 2300 4550
Wire Wire Line
	2300 4550 2300 4700
$Comp
L power:+3V3 #PWR?
U 1 1 6166C856
P 2300 5200
F 0 "#PWR?" H 2300 5050 50  0001 C CNN
F 1 "+3V3" H 2315 5373 50  0000 C CNN
F 2 "" H 2300 5200 50  0001 C CNN
F 3 "" H 2300 5200 50  0001 C CNN
	1    2300 5200
	-1   0    0    1   
$EndComp
Wire Wire Line
	2300 5000 2300 5200
$Comp
L power:GND #PWR?
U 1 1 6166D36D
P 6550 2750
F 0 "#PWR?" H 6550 2500 50  0001 C CNN
F 1 "GND" H 6555 2577 50  0000 C CNN
F 2 "" H 6550 2750 50  0001 C CNN
F 3 "" H 6550 2750 50  0001 C CNN
	1    6550 2750
	-1   0    0    1   
$EndComp
Wire Wire Line
	6550 3050 6550 2750
$Comp
L Jumper:Jumper_2_Open JP1
U 1 1 61672D55
P 1700 5600
F 0 "JP1" H 1700 5835 50  0000 C CNN
F 1 "Jumper_2_Open" H 1700 5744 50  0000 C CNN
F 2 "" H 1700 5600 50  0001 C CNN
F 3 "~" H 1700 5600 50  0001 C CNN
	1    1700 5600
	0    1    1    0   
$EndComp
Wire Wire Line
	1700 5100 1700 5400
$Comp
L Device:R R2
U 1 1 616788BD
P 1700 6250
F 0 "R2" H 1770 6296 50  0000 L CNN
F 1 "5K" H 1770 6205 50  0000 L CNN
F 2 "" V 1630 6250 50  0001 C CNN
F 3 "~" H 1700 6250 50  0001 C CNN
	1    1700 6250
	1    0    0    -1  
$EndComp
Wire Wire Line
	1700 5800 1700 6100
Wire Wire Line
	1700 6400 1700 6700
$Comp
L power:+3V3 #PWR?
U 1 1 61671FEA
P 7650 3800
F 0 "#PWR?" H 7650 3650 50  0001 C CNN
F 1 "+3V3" H 7665 3973 50  0000 C CNN
F 2 "" H 7650 3800 50  0001 C CNN
F 3 "" H 7650 3800 50  0001 C CNN
	1    7650 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	7100 3800 7650 3800
$Comp
L power:GND #PWR?
U 1 1 61672DC1
P 7450 3650
F 0 "#PWR?" H 7450 3400 50  0001 C CNN
F 1 "GND" H 7455 3477 50  0000 C CNN
F 2 "" H 7450 3650 50  0001 C CNN
F 3 "" H 7450 3650 50  0001 C CNN
	1    7450 3650
	-1   0    0    1   
$EndComp
Wire Wire Line
	7100 3650 7450 3650
Connection ~ 2300 4550
Wire Wire Line
	2300 4550 1700 4550
Wire Wire Line
	1700 4550 1700 4800
Wire Wire Line
	2300 4250 2700 4250
Wire Wire Line
	2300 4250 2300 4550
$Comp
L power:GND #PWR?
U 1 1 616882FF
P 3200 5100
F 0 "#PWR?" H 3200 4850 50  0001 C CNN
F 1 "GND" H 3205 4927 50  0000 C CNN
F 2 "" H 3200 5100 50  0001 C CNN
F 3 "" H 3200 5100 50  0001 C CNN
	1    3200 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	3200 4850 3200 5000
$Comp
L power:+3V3 #PWR?
U 1 1 61689627
P 3400 3650
F 0 "#PWR?" H 3400 3500 50  0001 C CNN
F 1 "+3V3" H 3415 3823 50  0000 C CNN
F 2 "" H 3400 3650 50  0001 C CNN
F 3 "" H 3400 3650 50  0001 C CNN
	1    3400 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 3850 3400 3750
Wire Wire Line
	3200 3850 3200 3750
Wire Wire Line
	3200 3750 3400 3750
Connection ~ 3400 3750
Wire Wire Line
	3400 3750 3400 3650
$Comp
L Device:C C1
U 1 1 6168F775
P 4600 4000
F 0 "C1" H 4715 4046 50  0000 L CNN
F 1 "0.1uF" H 4715 3955 50  0000 L CNN
F 2 "" H 4638 3850 50  0001 C CNN
F 3 "~" H 4600 4000 50  0001 C CNN
	1    4600 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 5000 4600 4150
Connection ~ 3200 5000
Wire Wire Line
	3200 5000 3200 5100
Wire Wire Line
	4600 3850 4600 3750
Text GLabel 7450 4250 2    50   Input ~ 0
INT2
Text GLabel 7450 4100 2    50   Input ~ 0
INT1
Wire Wire Line
	3200 5000 4600 5000
Wire Wire Line
	3400 3750 4600 3750
Text GLabel 3850 4450 2    50   Input ~ 0
INT2
Text GLabel 3850 4350 2    50   Input ~ 0
INT1
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
Wire Wire Line
	3700 4350 3850 4350
Wire Wire Line
	3850 4450 3700 4450
Wire Wire Line
	7100 4100 7450 4100
Wire Wire Line
	7450 4250 7100 4250
Text GLabel 2550 4350 0    50   Input ~ 0
SDA
Text GLabel 2550 4450 0    50   Input ~ 0
SCL
Wire Wire Line
	2700 4450 2550 4450
Wire Wire Line
	2550 4350 2700 4350
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
F 2 "" H 8950 1300 50  0001 L TNN
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
F 2 "" V 5030 3250 50  0001 C CNN
F 3 "~" H 5100 3250 50  0001 C CNN
	1    5100 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 3650 5100 3650
Wire Wire Line
	5100 3650 5100 3400
$Comp
L power:+3V3 #PWR?
U 1 1 616A7416
P 9300 1350
F 0 "#PWR?" H 9300 1200 50  0001 C CNN
F 1 "+3V3" H 9315 1523 50  0000 C CNN
F 2 "" H 9300 1350 50  0001 C CNN
F 3 "" H 9300 1350 50  0001 C CNN
	1    9300 1350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 616A7F93
P 8500 1600
F 0 "#PWR?" H 8500 1350 50  0001 C CNN
F 1 "GND" H 8505 1427 50  0000 C CNN
F 2 "" H 8500 1600 50  0001 C CNN
F 3 "" H 8500 1600 50  0001 C CNN
	1    8500 1600
	0    1    1    0   
$EndComp
Wire Wire Line
	9200 1600 9300 1600
Wire Wire Line
	8600 1600 8500 1600
$Comp
L LED:NeoPixel_THT PIXEL2
U 1 1 616B291A
P 8900 2700
F 0 "PIXEL2" V 8854 3044 50  0000 L CNN
F 1 "NeoPixel_THT" V 8945 3044 50  0000 L CNN
F 2 "" H 8950 2400 50  0001 L TNN
F 3 "https://www.adafruit.com/product/1938" H 9000 2325 50  0001 L TNN
	1    8900 2700
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 616B2926
P 8500 2700
F 0 "#PWR?" H 8500 2450 50  0001 C CNN
F 1 "GND" H 8505 2527 50  0000 C CNN
F 2 "" H 8500 2700 50  0001 C CNN
F 3 "" H 8500 2700 50  0001 C CNN
	1    8500 2700
	0    1    1    0   
$EndComp
Wire Wire Line
	9200 2700 9300 2700
Wire Wire Line
	8600 2700 8500 2700
$Comp
L LED:NeoPixel_THT PIXEL3
U 1 1 616B438B
P 8900 3800
F 0 "PIXEL3" V 8854 4144 50  0000 L CNN
F 1 "NeoPixel_THT" V 8945 4144 50  0000 L CNN
F 2 "" H 8950 3500 50  0001 L TNN
F 3 "https://www.adafruit.com/product/1938" H 9000 3425 50  0001 L TNN
	1    8900 3800
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 616B4397
P 8500 3800
F 0 "#PWR?" H 8500 3550 50  0001 C CNN
F 1 "GND" H 8505 3627 50  0000 C CNN
F 2 "" H 8500 3800 50  0001 C CNN
F 3 "" H 8500 3800 50  0001 C CNN
	1    8500 3800
	0    1    1    0   
$EndComp
Wire Wire Line
	9200 3800 9300 3800
Wire Wire Line
	8600 3800 8500 3800
Wire Wire Line
	5100 1150 8900 1150
Wire Wire Line
	8900 3000 8900 3500
Wire Wire Line
	8900 2400 8900 1900
$Comp
L LED:NeoPixel_THT PIXEL6
U 1 1 616C9B9C
P 10350 3800
F 0 "PIXEL6" V 10396 3456 50  0000 R CNN
F 1 "NeoPixel_THT" V 10305 3456 50  0000 R CNN
F 2 "" H 10400 3500 50  0001 L TNN
F 3 "https://www.adafruit.com/product/1938" H 10450 3425 50  0001 L TNN
	1    10350 3800
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 616C9BA8
P 10750 3800
F 0 "#PWR?" H 10750 3550 50  0001 C CNN
F 1 "GND" H 10755 3627 50  0000 C CNN
F 2 "" H 10750 3800 50  0001 C CNN
F 3 "" H 10750 3800 50  0001 C CNN
	1    10750 3800
	0    -1   -1   0   
$EndComp
Wire Wire Line
	10650 3800 10750 3800
$Comp
L LED:NeoPixel_THT PIXEL5
U 1 1 616C9BB0
P 10350 2700
F 0 "PIXEL5" V 10396 2356 50  0000 R CNN
F 1 "NeoPixel_THT" V 10305 2356 50  0000 R CNN
F 2 "" H 10400 2400 50  0001 L TNN
F 3 "https://www.adafruit.com/product/1938" H 10450 2325 50  0001 L TNN
	1    10350 2700
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 616C9BBC
P 10750 2700
F 0 "#PWR?" H 10750 2450 50  0001 C CNN
F 1 "GND" H 10755 2527 50  0000 C CNN
F 2 "" H 10750 2700 50  0001 C CNN
F 3 "" H 10750 2700 50  0001 C CNN
	1    10750 2700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	10650 2700 10750 2700
$Comp
L LED:NeoPixel_THT PIXEL4
U 1 1 616C9BC4
P 10350 1600
F 0 "PIXEL4" V 10396 1256 50  0000 R CNN
F 1 "NeoPixel_THT" V 10305 1256 50  0000 R CNN
F 2 "" H 10400 1300 50  0001 L TNN
F 3 "https://www.adafruit.com/product/1938" H 10450 1225 50  0001 L TNN
	1    10350 1600
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 616C9BD0
P 10750 1600
F 0 "#PWR?" H 10750 1350 50  0001 C CNN
F 1 "GND" H 10755 1427 50  0000 C CNN
F 2 "" H 10750 1600 50  0001 C CNN
F 3 "" H 10750 1600 50  0001 C CNN
	1    10750 1600
	0    -1   -1   0   
$EndComp
Wire Wire Line
	10650 1600 10750 1600
Wire Wire Line
	10350 2400 10350 1900
Wire Wire Line
	10350 3000 10350 3500
$Comp
L Jumper:Jumper_2_Open JP?
U 1 1 616CB68A
P 9650 4100
F 0 "JP?" H 9650 4335 50  0000 C CNN
F 1 "Jumper_2_Open" H 9650 4244 50  0000 C CNN
F 2 "" H 9650 4100 50  0001 C CNN
F 3 "~" H 9650 4100 50  0001 C CNN
	1    9650 4100
	-1   0    0    1   
$EndComp
Wire Wire Line
	9300 1350 9300 1600
Wire Wire Line
	9300 1600 9300 2700
Connection ~ 9300 1600
Wire Wire Line
	9300 2700 9300 3800
Connection ~ 9300 2700
Wire Wire Line
	9300 3800 9300 4100
Wire Wire Line
	9300 4100 9450 4100
Connection ~ 9300 3800
Wire Wire Line
	9850 4100 10050 4100
Wire Wire Line
	10050 4100 10050 3800
Wire Wire Line
	10050 3800 10050 2700
Connection ~ 10050 3800
Wire Wire Line
	10050 1600 10050 2700
Connection ~ 10050 2700
Wire Wire Line
	10350 4100 10350 4450
Wire Wire Line
	10350 4450 8900 4450
Wire Wire Line
	8900 4450 8900 4100
Wire Wire Line
	5100 1150 5100 3100
Wire Wire Line
	8900 1150 8900 1300
Wire Wire Line
	6400 4800 6400 6100
$Comp
L Device:Battery_Cell BT2
U 1 1 6165CE41
P 5700 5500
F 0 "BT2" H 5818 5596 50  0000 L CNN
F 1 "Battery_Cell" H 5818 5505 50  0000 L CNN
F 2 "" V 5700 5560 50  0001 C CNN
F 3 "~" V 5700 5560 50  0001 C CNN
	1    5700 5500
	1    0    0    -1  
$EndComp
$Comp
L Device:Battery_Cell BT1
U 1 1 616660DA
P 5700 6000
F 0 "BT1" H 5818 6096 50  0000 L CNN
F 1 "Battery_Cell" H 5818 6005 50  0000 L CNN
F 2 "" V 5700 6060 50  0001 C CNN
F 3 "~" V 5700 6060 50  0001 C CNN
	1    5700 6000
	1    0    0    -1  
$EndComp
Wire Wire Line
	5700 5800 5700 5600
Wire Wire Line
	5700 6100 6400 6100
Wire Wire Line
	6200 4800 5700 4800
Wire Wire Line
	5700 4800 5700 5300
$EndSCHEMATC
