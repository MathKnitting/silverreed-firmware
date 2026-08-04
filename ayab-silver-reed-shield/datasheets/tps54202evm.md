<!-- Source: tps54202evm.pdf; method: pymupdf4llm -->

<!-- Page 1 -->

_Table of Contents_ 

www.ti.com 

## _User’s Guide_ 

# **_TPS54202 Step-Down Converter Evaluation Module User's Guide_** 



##### **ABSTRACT** 

This user's guide contains background information for the TPS54202 as well as support documentation for the TPS54202EVM-716 evaluation module (PWR716-001). Also included are the performance specifications, the schematic, and the bill of materials for the TPS54202EVM-716. 

### **Table of Contents** 

|**1 Introduction**.............................................................................................................................................................................2|
|---|
|1.1 Background........................................................................................................................................................................2|
|1.2 Performance Specification Summary.................................................................................................................................2|
|1.3 Modifications......................................................................................................................................................................3|
|**2 Test Setup and Results**..........................................................................................................................................................4|
|2.1 Input/Output Connections..................................................................................................................................................4|
|2.2 Efficiency............................................................................................................................................................................5|
|2.3 Output Voltage Load Regulation........................................................................................................................................6|
|2.4 Output Voltage Line Regulation.........................................................................................................................................6|
|2.5 Load Transients..................................................................................................................................................................7|
|2.6 Output Voltage Ripple........................................................................................................................................................8|
|2.7 Input Voltage Ripple.........................................................................................................................................................10|
|2.8 Powering Up.....................................................................................................................................................................11|
|2.9 Powering Down................................................................................................................................................................12|
|**3 Board Layout**.........................................................................................................................................................................13|
|3.1 Layout..............................................................................................................................................................................13|
|**4 Schematic and Bill of Materials**...........................................................................................................................................15|
|4.1 Schematic........................................................................................................................................................................15|
|4.2 Bill of Materials.................................................................................................................................................................15|
|**5 Revision History**...................................................................................................................................................................17|



### **List of Figures** 

|Figure 2-1. TPS54202EVM-716 Efficiency..................................................................................................................................5|
|---|
|Figure 2-2. TPS54202EVM-716 Low Current Efficiency..............................................................................................................5|
|Figure 2-3. TPS54202EVM-716 Load Regulation.......................................................................................................................6|
|Figure 2-4. TPS54202EVM-716 Line Regulation........................................................................................................................6|
|Figure 2-5. TPS54202EVM-716 Transient Response.................................................................................................................7|
|Figure 2-6. TPS54202EVM-716 Output Ripple, IOUT= 2 A.........................................................................................................8|
|Figure 2-7. TPS54202EVM-716 Output Ripple, IOUT= 100 mA..................................................................................................8|
|Figure 2-8. TPS54202EVM-716 Output Ripple, IOUT= 10 mA....................................................................................................9|
|Figure 2-9. TPS54202EVM-716 Output Ripple, IOUT= 0 A.........................................................................................................9|
|Figure 2-10. TPS54202EVM-716 Input Ripple..........................................................................................................................10|
|Figure 2-11. TPS54202EVM-716 Startup Relative to VIN.........................................................................................................11|
|Figure 2-12. TPS54202EVM-716 Startup Relative to Enable....................................................................................................11|
|Figure 2-13. TPS54202EVM-716 Shutdown Relative to VIN....................................................................................................12|
|Figure 2-14. TPS54202EVM-716 Shutdown Relative to EN.....................................................................................................12|
|Figure 3-1. TPS54202EVM-716 Top-Side Assembly.................................................................................................................13|
|Figure 3-2. TPS54202EVM-716 Bottom-Side Layout................................................................................................................14|
|Figure 4-1. TPS54202EVM-716 Schematic...............................................................................................................................15|



SLVUAP3A – APRIL 2016 – REVISED OCTOBER 2021 _Submit Document Feedback_ 

_TPS54202 Step-Down Converter Evaluation Module User's Guide_ 

1 

Copyright © 2021 Texas Instruments Incorporated

<!-- Page 2 -->

_Trademarks_ 

www.ti.com 

### **List of Tables** 

Table 1-1. Input Voltage and Output Current Summary...............................................................................................................2 Table 1-2. TPS54202EVM-716 Performance Specification Summary.........................................................................................2 Table 1-3. Recommended Component Values............................................................................................................................ 3 Table 2-1. EVM Connectors and Test Points............................................................................................................................... 4 Table 4-1. TPS54202EVM-716 Bill of Materials.........................................................................................................................15 

### **Trademarks** 

All trademarks are the property of their respective owners. 

### **1 Introduction** 

This user's guide contains background information for the TPS54202 as well as support documentation for the TPS54202EVM-716 evaluation module (PWR716-001). Also included are the performance specifications, the schematic, and the bill of materials for the TPS54202EVM-716. 

#### **1.1 Background** 

The TPS54202 dc/dc converter is designed to provide up to a 2-A output from an input voltage source of 8 V to 28 V. Rated input voltage and output current range for the evaluation module are given in Table 1-1. In order to reduce EMI, the TPS54202 introduces frequency spread spectrum. The jittering span is ±6% of the switching frequency with 1/512 swing frequency. This evaluation module is designed to demonstrate the small printed-circuit-board areas that may be achieved when designing with the TPS54202 regulator. The switching frequency is internally set at a nominal 500 kHz. The high-side and low-side MOSFETs are incorporated inside the TPS54202 package along with the gate-drive circuitry. The low drain-to-source on resistance of the MOSFETs allow the TPS54202 to achieve high efficiencies and helps keep the junction temperature low at high output currents. The compensation components are integrated to the integrated circuit (IC), and an external divider allows for an adjustable output voltage. Additionally, the TPS54202 provides an adjustable undervoltage lockout input. The absolute maximum input voltage is 30 V for the TPS54202EVM-716. 

**Table 1-1. Input Voltage and Output Current Summary** 

|**EVM**|**Input Voltage Range**|**Output Current Range**|
|---|---|---|
|TPS54202EVM-716|VIN= 8 V to 28 V|0 A to 2 A|



#### **1.2 Performance Specification Summary** 

A summary of the TPS54202EVM-716 performance specifications is provided in Table 1-2. Specifications are given for an input voltage of VIN = 24 V and an output voltage of 5.0 V, unless otherwise specified. The TPS54202EVM-716 is designed and tested for VIN = 8 V to 28 V. The ambient temperature is 25°C for all measurements, unless otherwise noted. 

**Table 1-2. TPS54202EVM-716 Performance Specification Summary** 

|**Specification**|**Test**|**Conditions**|**MIN**|**TYP**|**MAX**|**Unit**|
|---|---|---|---|---|---|---|
|VINoperating voltage range|||8|24|28|V|
|VINstart voltage||||6.74||V|
|VINstop voltage||||5.83||V|
|Output voltage set point||||5||V|
|Output current range|VIN= 8 V to 28 V||0||2|A|
|Line regulation|IO= 1 A, VIN= 8 V to|28 V||±0.5%|||
|Load regulation|VIN= 12 V, IO= 0 A to|2 A||±0.5%|||
|||Voltage change||–150||mV|
||IO= 0.5 A to 1.5 A|Recovery time||150||μs|
|Load transient response||Voltage change||150||mV|
||IO= 1.5 A to 0.5 A|Recovery time||150||μs|
|Input ripple voltage|IO= 2 A|||400||mVPP|
|Output ripple voltage|IO= 2 A|||<30||mVPP|
|Output rise time||||5||ms|



SLVUAP3A – APRIL 2016 – REVISED OCTOBER 2021 _Submit Document Feedback_ 

2 _TPS54202 Step-Down Converter Evaluation Module User's Guide_ 

Copyright © 2021 Texas Instruments Incorporated

<!-- Page 3 -->

www.ti.com 

_Introduction_ 

**Table 1-2. TPS54202EVM-716 Performance Specification Summary** **<u>(continued)</u>** 

|**Specification**|**Test Conditions**|**MIN**|**TYP**|**MAX**|**Unit**|
|---|---|---|---|---|---|
|Center operating frequency|||500||kHz|
|Maximum Efficiency|TPS54202EVM-716, VIN= 12 V, IO= 1 A||94.06%|||



#### **1.3 Modifications** 

These evaluation modules are designed to provide access to the features of the TPS54202. Some modifications can be made to this module. 

##### **1.3.1 Output Voltage Set Point** 

The voltage divider, R2 and R3, is used to set the output voltage. To change the output voltage of the EVM, it is necessary to change the value of resistor R3. Changing the value of R3 can change the output voltage above 0.596 V. The value of R3 for a specific output voltage can be calculated using Equation 1. Use 100 kΩ for R2. 





Table 1-3 lists the R2 and R3 values for some common output voltages. Note that VIN must be in a range so that the minimum on-time is greater than 150 ns. The values in Table 1-3 are standard values, not the exact value calculated using Equation 1. 

**Table 1-3. Recommended Component Values** 

|**VOUT (V)**|**L (µH)**|**COUT (µF)**|**R2 (kΩ)**|**R3 (kΩ)**|**C8 (pF)**|
|---|---|---|---|---|---|
|1.8|5.6|66|100|49.9|47|
|2.5|8.2|44|100|31.6|33|
|3.3|10|44|100|22.1|56|
|5|15|44|100|13.3|75|
|12|22|44|100|5.23|100|



##### **1.3.2 Output Capacitor and Feed-Forward Capacitor** 

Considering the loop stability and the effect of the internal parasitic parameters, choose a crossover frequency less than 40 kHz, without considering the feed-forward capacitor. A simple estimation for the crossover frequency without feed-forward capacitor C8 is shown in Equation 2, assuming COUT has small ESR. 



Depending on VOUT, if the output capacitor, COUT, is dominated by low-ESR (ceramic types) capacitors, a low phase margin could result. To improve the phase boost, an external feed-forward capacitor, C8, can be added in parallel with R2. C8 is chosen such that phase margin is boosted at the crossover frequency. 

C8 is calculated in Equation 3: 



For this design, C8 = 75 pF. C8 is not needed when COUT has high ESR, and C8 calculated from Equation 3 should be reduced with medium ESR. Use Table 1-3 as a starting point. 

SLVUAP3A – APRIL 2016 – REVISED OCTOBER 2021 _Submit Document Feedback_ 

_TPS54202 Step-Down Converter Evaluation Module User's Guide_ 

3 

Copyright © 2021 Texas Instruments Incorporated

<!-- Page 4 -->

_Introduction_ 

www.ti.com 

##### **1.3.3 Adjustable UVLO** 

The under voltage lock out (UVLO) can be adjusted externally using R4 and R5. The EVM is set for a start voltage of 6.74 V and a stop voltage of 5.83 V using R4 = 510 kΩ and R5 = 105 kΩ. Use Equation 4 and Equation 5 to calculate required resistor values for different start and stop voltages. For higher light-load efficiency, consider choosing a larger R4 and R5. Make adjustments to VSTART or VSTOP for a proper R4. 



Ip = 0.7 µA, Ih = 1.55 µA, VENFALLING = 1.19 V and VENRISING = 1.22 V 

### **2 Test Setup and Results** 

This section describes how to properly connect, set up, and use the TPS54202EVM-716 evaluation module. The section also includes test results typical for the evaluation module and covers efficiency, output voltage regulation, load transients, loop response, output ripple, input ripple, and start-up. 

#### **2.1 Input/Output Connections** 

The TPS54202EVM-716 is provided with input/output connectors and test points as shown in Table 2-1. A power supply capable of supplying 2 A must be connected to J1 through a pair of 20-AWG wires. The load must be connected to J2 through a pair of 20-AWG wires. The maximum load current capability must be at least 3 A to use the full capability of this EVM. Wire lengths must be minimized to reduce losses in the wires. Test-point TP1 provides a place to monitor the VIN input voltages with TP2 providing a convenient ground reference. TP6 is used to monitor the output voltage with TP7 as the ground reference. 

**Table 2-1. EVM Connectors and Test Points** 

|**Reference Designator**|**Function**|
|---|---|
|J1|VIN (seeTable 1-1for VINrange)|
|J2|VOUT, 5 V at 2 A maximum|
|JP1|2-pin header for enable. Connect EN to ground to disable, open to enable.|
|TP1|VINtest point at VIN connector|
|TP2|GND test point at VIN|
|TP3|GND test point|
|TP4|SW test point|
|TP5|Test point between voltage divider network and output. Used for loop response measurements.|
|TP6|Output voltage test point at OUT connector|
|TP7|GND test point at VOUT connector|



SLVUAP3A – APRIL 2016 – REVISED OCTOBER 2021 _Submit Document Feedback_ 

_TPS54202 Step-Down Converter Evaluation Module User's Guide_ 

4 

Copyright © 2021 Texas Instruments Incorporated

<!-- Page 5 -->

_Test Setup and Results_ 

www.ti.com 

#### **2.2 Efficiency** 

The efficiency of this EVM peaks at a load current of about 0.5 A – 1 A, and then decreases as the load current increases towards full load. Figure 2-1 shows the efficiency for the TPS54202EVM-716 at an ambient temperature of 25°C. 



<!-- Start of picture text -->
100<br>90<br>80<br>70<br>60<br>50<br>40<br>30<br>20<br>10<br>VIN = 24 V, VOUT = 5 V<br>VIN = 12 V, VOUT = 5 V<br>0<br>0 0.5 1 1.5 2<br>Output Current (A) D001<br>Effieciency (%)<br><!-- End of picture text -->

**Figure 2-1. TPS54202EVM-716 Efficiency** 

Figure 2-2 shows the efficiency for the TPS54202EVM-716 on a semi-log scale to better show light load efficiency. The ambient temperature is 25°C. 



<!-- Start of picture text -->
100<br>90<br>80<br>70<br>60<br>50<br>40<br>30<br>20<br>10<br>VIN = 24 V, VOUT = 5 V<br>VIN = 12 V, VOUT = 5 V<br>0<br>0.001 0.01 0.1 1 3<br>Output Current (A) D002<br>Effieciency (%)<br><!-- End of picture text -->

**Figure 2-2. TPS54202EVM-716 Low Current Efficiency** 

The efficiency may be lower at higher ambient temperatures, due to temperature variation in the drain-to-source resistance of the internal MOSFET. 

SLVUAP3A – APRIL 2016 – REVISED OCTOBER 2021 _Submit Document Feedback_ 

_TPS54202 Step-Down Converter Evaluation Module User's Guide_ 

5 

Copyright © 2021 Texas Instruments Incorporated

<!-- Page 6 -->

_Test Setup and Results_ 

www.ti.com 

#### **2.3 Output Voltage Load Regulation** 

Figure 2-3 shows the load regulation for the TPS54202EVM-716. 



<!-- Start of picture text -->
0.5<br>0.4<br>0.3<br>0.2<br>0.1<br>0<br>-0.1<br>-0.2<br>-0.3<br>-0.4<br>VIN = 24 V<br>VIN = 12 V<br>-0.5<br>0.1 0.6 1.1 1.6 2.1<br>Output Current (A) D003<br>Load Regulation (%)<br><!-- End of picture text -->

**Figure 2-3. TPS54202EVM-716 Load Regulation** 

Measurements are given for an ambient temperature of 25°C. 

#### **2.4 Output Voltage Line Regulation** 

Figure 2-4 shows the line regulation for the TPS54202EVM-716. 



<!-- Start of picture text -->
0.5<br>0.4<br>0.3<br>0.2<br>0.1<br>0<br>-0.1<br>-0.2<br>-0.3<br>-0.4<br>IOUT = 1 A<br>-0.5<br>6 8 10 12 14 16 18 20 22 24 26 28<br>Input Voltage (V) D004<br>Line Regulation (%)<br><!-- End of picture text -->

**Figure 2-4. TPS54202EVM-716 Line Regulation** 

SLVUAP3A – APRIL 2016 – REVISED OCTOBER 2021 _Submit Document Feedback_ 

6 _TPS54202 Step-Down Converter Evaluation Module User's Guide_ 

Copyright © 2021 Texas Instruments Incorporated

<!-- Page 7 -->

wis TEXASINSTRUMENTS 



<!-- Start of picture text -->
Vour = 100 mv / div (ac Coupled)<br>lout = LA/ div<br>cs me mamma: mamemeaammn nanemamamaa anne<br>| Q.5-A to 1.5-A Load Step,<br>Slew Rate =250 mA/ psec<br>Time = 200 ps = 200 ps 200 ps ps / div<br><!-- End of picture text -->

Time = 200 ps = 200 ps 200 ps ps / div

<!-- Page 8 -->

Wid TexasINSTRUMENTS 



<!-- Start of picture text -->
u t<br>nl eed eee PAM Reef<br>|. | t | | | 4 4 |<br>Time= 2 ps / div<br><!-- End of picture text -->



<!-- Start of picture text -->
5 -<br>PH=10V/dv<br>mens | itis | Mit, |i | UNitstaes H eine | | i<br>jm!! |  Rrap!| A)mene! pea |) AA| nee||py |<br>Time= 4 ps / div<br><!-- End of picture text -->

<!-- Page 9 -->

Wip TexasINSTRUMENTS 



<!-- Start of picture text -->
| | | i | |<br>Time = 40 ps/ = 40 ps/ 40 ps/ ps// div<br><!-- End of picture text -->

> Time = 40 ps/ = 40 ps/ 40 ps/ ps// div 



<!-- Start of picture text -->
Time== 4 ms / div<br><!-- End of picture text -->

Time== 4 ms / div

<!-- Page 10 -->

Wid TexasINSTRUMENTS 



<!-- Start of picture text -->
fied a ee ne e cea eee<br>| | | i es |<br>Time== 2 ps / div<br><!-- End of picture text -->

Time== 2 ps / div

<!-- Page 11 -->

wis TEXASINSTRUMENTS 

Time = 2 ms / div 



<!-- Start of picture text -->
Vin =10V/ div t<br>EN=2V/div t<br>Vour =2V/ div<br><!-- End of picture text -->

Time = 2 ms / div

<!-- Page 12 -->

Wid TexasINSTRUMENTS 



<!-- Start of picture text -->
EN=2V/div t<br>ee<br>Vour =2V/ div t i<br>i |<br>Time =4 ms / div<br><!-- End of picture text -->



<!-- Start of picture text -->
Vin = 10V/ div :<br>EN=2V/div t<br>Vout =2V/ div t \\<br>Time= 2 ms / div<br><!-- End of picture text -->

<!-- Page 13 -->

Wip TexasINSTRUMENTS 



<!-- Start of picture text -->
a) For evaluation only; not FCC approved for resale. (2)<br>TP2 oO & . * o ° e . > o & se e e o es e 2 Oo * e e * ese o es<br>GND eoeeeee De eoeeeees<br>= eeseeees ese eeee#s#?es TPZ<br>= oe eee 7 ©<br>Gaara, @O [|]<br>© a oO seseeceo 5<br>‘Sha° oo o8 8<br>aomm, 28 VOUT<br>egaase ©<br>ses PT Looe SH GND<br>INSTRUMENTS<br>WB Texas au<br>oO PUR716 A @<br><!-- End of picture text -->

<!-- Page 14 -->

<!-- Start of picture text -->
oc ea:<br>a o<br>eee i<br>le eesee fe)<br>fe] we<br>oO QO on-:---<br><!-- End of picture text -->

<!-- Page 15 -->

_Schematic and Bill of Materials_ 

www.ti.com 

### **4 Schematic and Bill of Materials** 

This section presents the TPS54202EVM-716 schematic and bill of materials. 

#### **4.1 Schematic** 

Figure 4-1 is the schematic for the TPS54202EVM-716. 



<!-- Start of picture text -->
TP1 U1 L1 TP6<br>J1 VIN =8V - 28V VIN3 VIN SW 2 SW VOUT VOUT = 5V, 2A Max J2<br>15µH<br>C4<br>TP2 C1 C2 R4 EN 5 EN VBST 6 BOOT R6 C5 C6 DN P C7 TP5 R1 TP7<br>0.1uF 10µF 510k 0 TP4 22µF 22µF 22µF 49.9<br>TP3 0.1uF R7<br>JP1 0<br>VFB 4 VSENSE C8 R2100k GND<br>GND GND 1 GND 75pF<br>R5 TPS54202DDCR R3<br>105k 13.3k<br>GND GND GND<br>Copyright © 2016, Texas Instruments Incorporated<br><!-- End of picture text -->

**Figure 4-1. TPS54202EVM-716 Schematic** 

#### **4.2 Bill of Materials** 

Table 4-1 presents the bill of materials for the TPS54202EVM-716. 

**Table 4-1. TPS54202EVM-716 Bill of Materials** 

|**Designator**|**Qty**|**Value**|**Description**|**Package Reference**|**Part Number**|**Manufacturer**|
|---|---|---|---|---|---|---|
|C1, C4|2|0.1 μF|CAP, CERM, 0.1 μF, 25 V, ±10%, X5R, 0603|0603|GRM188R61E104KA01D|Murata|
|C2|1|10 μF|CAP, CERM, 10 µF, 35 V, ±10%, X7R, 1210|1210|GRM32ER7YA106KA12L|Murata|
|C5, C6|2|22 μF|CAP, CERM, 22 µF, 25 V, ±10%, X7R, 1210|1210|GRM32ER71E226KE15L|Murata|
|C8|1|75 pF|CAP, CERM, 75 pF, 50 V, ±5%, C0G/NP0, 0603|0603|GRM1885C1H750JA01D|Murata|
|J1, J2|2||Terminal Block, 6 A, 3.5 mm Pitch, 2-Pos, TH|7.0 × 8.2 × 6.5 mm|ED555/2DS|On-Shore Technology|
|JP1|1||Header, 100 mil, 2 × 1, Gold, TH|2 × 1 Header|TSW-102-07-G-S|Samtec|
|L1|1|15 μH|Inductor, Shielded Drum Core, Ferrite, 15 µH, 3.5 A, 0.036 Ω, SMD|10 × 5 × 10 mm|7447714150|Wurth Elektronik|
|R1|1|49.9|RES, 49.9, 1%, 0.1 W, 0603|0603|CRCW060349R9FKEA|Vishay-Dale|
|R2|1|100 k|RES, 100 k, 1%, 0.1 W, 0603|0603|CRCW0603100KFKEA|Vishay-Dale|
|R3|1|13.3 k|RES, 13.3 k, 1%, 0.1 W, 0603|0603|CRCW060313K3FKEA|Vishay-Dale|
|R4|1|510 k|RES, 510 k, 5%, 0.1 W, 0603|0603|CRCW0603510KJNEA|Vishay-Dale|
|R5|1|105 k|RES, 105 k, 1%, 0.1 W, 0603|0603|CRCW0603105KFKEA|Vishay-Dale|
|R6, R7|2|0|RES, 0 ohm, 5%, 0.1W, 0603|0603|ERJ-3GEY0R00V|Panasonic|
|TP1, TP6|2|Red|Test Point, Miniature, Red, TH|Red Miniature Testpoint|5000|Keystone|
|TP2, TP3, TP7|3|Black|Test Point, Miniature, Black, TH|Black Miniature Testpoint|5001|Keystone|



SLVUAP3A – APRIL 2016 – REVISED OCTOBER 2021 _Submit Document Feedback_ 

_TPS54202 Step-Down Converter Evaluation Module User's Guide_ 

15 

Copyright © 2021 Texas Instruments Incorporated

<!-- Page 16 -->

_Schematic and Bill of Materials_ 

www.ti.com 

**Table 4-1. TPS54202EVM-716 Bill of Materials (continued)** 

|**Designator**|**Qty**|**Value**|**Description**|**Package Reference**|**Part Number**|**Manufacturer**|
|---|---|---|---|---|---|---|
|TP4|1|Yellow|Test Point, Miniature, Yellow, TH|Yellow Miniature Testpoint|5004|Keystone|
|TP5|1|White|Test Point, Miniature, White, TH|White Miniature Testpoint|5002|Keystone|
|U1|1||4.5-V TO 28-V INPUT, 2-A OUTPUT, EMI FRIENDLY SYNCHRONUOUS<br>STEP-DOWN CONVERTOR, DDC0006A|DDC0006A|TPS54202DDCR|Texas Instruments|
|C7|0|22 μF|CAP, CERM, 22 µF, 25 V, ±10%, X7R, 1210|1210|GRM32ER71E226KE15L|Murata|



SLVUAP3A – APRIL 2016 – REVISED OCTOBER 2021 _Submit Document Feedback_ 

_TPS54202 Step-Down Converter Evaluation Module User's Guide_ 

16 

Copyright © 2021 Texas Instruments Incorporated

<!-- Page 17 -->

www.ti.com 

_Revision History_ 

### **5 Revision History** 

NOTE: Page numbers for previous revisions may differ from page numbers in the current version. 

|**Changes from Revision * (April 2016) to Revision A (October 2021)**|**Page**|
|---|---|



- Updated the numbering format for tables, figures, and cross-references throughout the document. ................2 

- • Updated the user's guide title............................................................................................................................. 2 

SLVUAP3A – APRIL 2016 – REVISED OCTOBER 2021 _Submit Document Feedback_ 

_TPS54202 Step-Down Converter Evaluation Module User's Guide_ 

17 

Copyright © 2021 Texas Instruments Incorporated

<!-- Page 18 -->

### **IMPORTANT NOTICE AND DISCLAIMER** 

TI PROVIDES TECHNICAL AND RELIABILITY DATA (INCLUDING DATASHEETS), DESIGN RESOURCES (INCLUDING REFERENCE DESIGNS), APPLICATION OR OTHER DESIGN ADVICE, WEB TOOLS, SAFETY INFORMATION, AND OTHER RESOURCES “AS IS” AND WITH ALL FAULTS, AND DISCLAIMS ALL WARRANTIES, EXPRESS AND IMPLIED, INCLUDING WITHOUT LIMITATION ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY RIGHTS. 

These resources are intended for skilled developers designing with TI products. You are solely responsible for (1) selecting the appropriate TI products for your application, (2) designing, validating and testing your application, and (3) ensuring your application meets applicable standards, and any other safety, security, regulatory or other requirements. 

These resources are subject to change without notice. TI grants you permission to use these resources only for development of an application that uses the TI products described in the resource. Other reproduction and display of these resources is prohibited. No license is granted to any other TI intellectual property right or to any third party intellectual property right. TI disclaims responsibility for, and you fully indemnify TI and its representatives against any claims, damages, costs, losses, and liabilities arising out of your use of these resources. 

TI’s products are provided subject to TI’s Terms of Sale, TI’s General Quality Guidelines, or other applicable terms available either on ti.com or provided in conjunction with such TI products. TI’s provision of these resources does not expand or otherwise alter TI’s applicable warranties or warranty disclaimers for TI products. Unless TI explicitly designates a product as custom or customer-specified, TI products are standard, catalog, general purpose devices. 

TI objects to and rejects any additional or different terms you may propose. 

Copyright © 2026, Texas Instruments Incorporated 

Last updated 10/2025
