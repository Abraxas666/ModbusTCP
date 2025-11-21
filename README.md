# Modbus TCP library for Arduino family v1.0.0 
Author: Abraxas666

The library includes the main functionalities to read and write Coil Status, Input Status, Holding registers and Input registers

## Bill of Materials (BOM)

| Qty | Component                                | Description                                                                                                      |
|-----|------------------------------------------|------------------------------------------------------------------------------------------------------------------|
| 1   | Ethernet shield                          | W5100 ethernet shield for arduino                                                                               |
| 1   | Arduino Uno                              |                          |



### ✅ Example

The example transform Arduino in a Modbus-TCP slave device. The following functions code have been implemented

## Modbus Function Codes Summary

| Code | Name                          | Description                                                                 |
|------|------------------------------|-----------------------------------------------------------------------------|
| 1    | Read Coils                  | Reads the ON/OFF status of discrete outputs (coils).                       |
| 2    | Read Discrete Inputs        | Reads the ON/OFF status of discrete inputs.                                |
| 3    | Read Holding Registers      | Reads the contents of holding registers (read/write).                      |
| 4    | Read Input Registers        | Reads the contents of input registers (read-only).                         |
| 5    | Write Single Coil           | Forces a single coil to ON or OFF.                                         |
| 6    | Write Single Register       | Writes a single holding register.                                          |
| 15   | Write Multiple Coils        | Forces multiple coils to ON or OFF.                                        |
| 16   | Write Multiple Registers    | Writes multiple holding registers.                                         |
| 22   | Mask Write Register         | Modifies specific bits in a holding register using AND/OR masks.           |
| 23   | Read/Write Multiple Registers| Reads and writes multiple holding registers in one transaction.            |

---



