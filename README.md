# Arduino-LTC2357-Library

This library is for using the LTC2357-18 in "mode 7" i.e. ±10.24V input. Uses SPI single SDO. the ADC supports multiple SDO, but this library does not. This library is a derivitive of the LTC library with major changes.

Should work with the following ADCs, with modifications to the defines:

LTC2348-18: Octal, 18-Bit, 200ksps Differential ±10.24V Input SoftSpan ADC with Wide Input Common Mode Range.
LTC2348-16: Octal, 16-Bit, 200ksps Differential ±10.24V Input SoftSpan ADC with Wide Input Common Mode Range.
LTC2344-18: Quad, 18-Bit, 400ksps/ch Differential SoftSpan ADC with Wide Input Common Mode Range.
LTC2344-16: Quad, 16-Bit, 400ksps/ch Differential SoftSpan ADC with Wide Input Common Mode Range.
LTC2333-18: Buffered 8-Channel, 18-Bit, 800ksps Differential ±10.24V ADC with 30VP-P Common Mode Range.
LTC2333-16: Buffered 8-Channel, 16-Bit, 800ksps Differential ±10.24V ADC with 30VP-P Common Mode Range.
LTC2357-18: Buffered Quad, 18-Bit, 350ksps/Ch Differential ±10.24V ADC with 30VP-P Common Mode Range.
LTC2357-16: Buffered Quad, 16-Bit, 350ksps/Ch Differential ±10.24V ADC with 30VP-P Common Mode Range.
LTC2358-18: Buffered Octal, 18-Bit, 200ksps/Ch Differential ±10.24V ADC with 30VP-P Common Mode Range.
LTC2358-16: Buffered Octal, 16-Bit, 200ksps/Ch Differential ±10.24V ADC with 30VP-P Common Mode Range.
LTC2353-18: Buffered Dual, 18-Bit, 550ksps/Ch Differential ±10.24V ADC with 30VP-P Common Mode Range.
LTC2353-16: Buffered Dual, 16-Bit, 550ksps/Ch Differential ±10.24V ADC with 30VP-P Common Mode Range.

Has a possible issue with voltage refrence scaling. Unsure. Tested only with the LTC2357-18 with a SAMD51J19, but should work with any arduino board.

Im not a software engineer. i neither know or care about liscences. Do whatever you want with this code, i could not care less. I will try to keep this library up to date if anyone has problems.
