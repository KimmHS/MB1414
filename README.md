# MB1434
short example of using MB1134 USB-ProxSonar®-EZ3  

http://www.vctec.co.kr/web/product/maxbotix/documents/USB-ProxSonar-EZ_Datasheet.pdf

label|Value 
---|---
Baud |57600
Data bits |8
Parity| 0 / None
Stop Bit |1
Flow Control |0/None 

# Serial Output Format
The sensor output is provided over the COM port (or equivalent) in an ASCII character format. If a target is detected at
8 inches the output appears as follows: “R008 P1<carriage return>”. The output is an ASCII capital “R”, followed by
three ASCII character digits representing the range in inches up to a maximum of 125 inches. This is followed by an
ASCII space and the ASCII character “P”, followed by one ASCII digit “1 or 0” corresponding to the “True or False”
proximity information, followed by a carriage return. A proximity value of “1” signifies that a target is present in the
detection zone. A proximity value of “0” signifies that no target has been detected in the detection zone. 

