# Mini-Soc-Acc
Development of a minimal RISC-V System on Chip (SoC) using LiteX, featuring a custom computational accelerator written in Verilog. This project demonstrates the full hardware/software stack, from gateware generation to bare-metal C firmware development.

## 🚀 Features
- **CPU**: VexRiscv (RISC-V 32-bit).
- **Interconnect**: Wishbone Bus.
- **Accelerator**: Custom Verilog Multiplier (just PoC) integrated via CSRs.
- **Simulation**: Verilator-based simulation with UART console support.
- **SRAM**: Stack memory (16KiB).
- **Main RAM**: Firmware execution space at 0x40000000.
- **Software Stack**: * Custom Linker Script + Bare-metal C firmware.

## 🛠️ Requirements
- Python 3.x & LiteX
- Verilator (for simulation)
- RISC-V Toolchain

## 📖 How to run
1. Compile the Firmware
   Navigate to the firmware directory and compile the binary:
   ```bash
   cd firmware && make clan && make   
2. Generate the SoC and compile the gateware:
   ```bash
   python3 mi_soc.py

### Execution Example (Multiplier Test)
```
        __   _ __      _  __
       / /  (_) /____ | |/_/
      / /__/ / __/ -_)>  <
     /____/_/\__/\__/_/|_|
   Build your hardware, easily!

 (c) Copyright 2012-2026 Enjoy-Digital
 (c) Copyright 2007-2015 M-Labs

 BIOS built on Apr  1 2026 16:53:31
 BIOS CRC passed (02a56dc8)

 LiteX git sha1: bd9d490

--=============== SoC ==================--
CPU:            VexRiscv @ 1MHz
BUS:            wishbone 32-bit @ 4GiB
CSR:            32-bit data big ordering
ROM:            32.0KiB
SRAM:           16.0KiB
MAIN-RAM:       64.0KiB

--========== Initialization ============--


--============== Boot ==================--
Booting from serial...
Press Q or ESC to abort boot completely.
sL5DdSMmkekro
Timeout
No boot medium found

--============= Console ================--

litex> boot 0x40000000
Executing booted program at 0x40000000

--============= Liftoff! ===============--
--- Verilog Accelerator Console ---
Command: calc <num1> <num2>
>>>> calc 0xFF 34
Result: 255 * 34 = 8670
>>>> calc 50 50
Result: 50 * 50 = 2500
>>>> calc 0xFA 0x11
Result: 250 * 17 = 4250
>>>> reboot
```
## 🧠 Hardware/Software Co-Design

The accelerator is mapped to the CSR bus. The C firmware interacts with the hardware using the generated csr.h headers, allowing high-level access to low-level Verilog logic:
```
// Example of hardware interaction in C
mult_hw_params_a_write(a);
mult_hw_params_b_write(b);
uint32_t res = mult_hw_resultado_read();
```
## 🛠 Future Work

[ ] Accelerator Expansion: implement an accelerator for a cryptographic or AI primitive.

[ ] Interrupts (IRQ): Implement event-driven completion signals for the accelerator.

[ ] Standalone Boot: Move the firmware to ROM to bypass the LiteX BIOS.

[ ] DMA-driven data transfers.

[ ] Modify the boot process to create a secure memory zone that cannot be accessed without privileges.

