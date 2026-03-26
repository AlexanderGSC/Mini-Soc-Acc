# Mini-Soc-Acc
Development of a minimal RISC-V System on Chip with Litex and integration with a computational accelerator written in Verilog. It serves as an example of a minimal architecture and as a foundation for future expansions (real hardware accelerators for scientific computing and AI). 

This project serves as a practical foundation for understanding SoC architecture, memory-mapped I/O (MMIO), and hardware/software co-design.

## 🚀 Features
- **CPU**: VexRiscv (RISC-V 32-bit).
- **Interconnect**: Wishbone Bus.
- **Accelerator**: Custom Verilog Multiplier integrated via CSRs (Control and Status Registers).
- **Simulation**: Verilator-based simulation with UART console support.

## 🛠️ Requirements
- Python 3.x & LiteX
- Verilator (for simulation)
- RISC-V Toolchain (for software headers)

## 📖 How to run
1. Generate the SoC and compile the gateware:
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

 BIOS built on Mar 26 2026 13:29:50
 BIOS CRC passed (4c58ab86)

--=============== SoC ==================--
CPU:            VexRiscv @ 1MHz
BUS:            wishbone 32-bit @ 4GiB
CSR:            32-bit data big ordering
ROM:            32.0KiB
SRAM:           16.0KiB

--============= Console ================--

litex> mem_write 0xf0000000 5
litex> mem_write 0xf0000004 5
litex> mem_read 0xf0000008   
Memory dump:
0xf0000008  19 00 00 00                                      ....
```
