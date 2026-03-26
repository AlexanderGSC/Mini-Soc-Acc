from migen import *
from litex.build.generic_platform import *
from litex.build.sim import SimPlatform
from litex.build.sim.config import SimConfig
from litex.soc.integration.soc_core import SoCCore
from litex.soc.integration.builder import Builder
from litex.soc.interconnect.csr import *

# -----------------------------------------------------------------------------
# 1. IO - UART simulado
# -----------------------------------------------------------------------------
_io = [
    ("sys_clk", 0, Pins(1)),
    ("sys_rst", 0, Pins(1)),
    ("serial", 0,
        Subsignal("source_valid", Pins(1)),
        Subsignal("source_ready", Pins(1)),
        Subsignal("source_data",  Pins(8)),
        Subsignal("sink_valid",   Pins(1)),
        Subsignal("sink_ready",   Pins(1)),
        Subsignal("sink_data",    Pins(8)),
    ),
]

# Plataforma

class Platform(SimPlatform):
    default_clk_name   = "sys_clk"
    default_clk_period = 1000

    def __init__(self):
        SimPlatform.__init__(self, "SIM", _io)


# -----------------------------------------------------------------------------
# 3. Acelerador HW (CSR)
# -----------------------------------------------------------------------------
class Acelerador(Module, AutoCSR):
    def __init__(self):
        self.params_a  = CSRStorage(32)
        self.params_b  = CSRStorage(32)
        self.resultado = CSRStatus(32)

# -----------------------------------------------------------------------------
# 4. SoC principal
# -----------------------------------------------------------------------------

class MiSoC(SoCCore):
    def __init__(self):
        platform = Platform()
        sys_clk_freq = int(100e6)

        # Importante: uart_name coincide con el nombre del IO "serial"
        SoCCore.__init__(self, platform,
            cpu_type             = "vexriscv",
            clk_freq             = 1e6,
            ident                = "LiteX CPU Test SoC",
            uart_name            = "sim", 
            ident_version        = True,
            integrated_rom_size  = 0x8000,
            integrated_sram_size = 0x4000,
            uart_fifo_depth      = 1,
        )
        self.submodules.crg = CRG(platform.request("sys_clk"), platform.request("sys_rst"))
        self.add_constant("UART_POLLING")
        # Acelerador
        self.submodules.mult_hw = Acelerador()
        self.add_csr("mult_hw")

        # Instancia del módulo Verilog
        self.specials += Instance("MultiplicadorVerilog",
            i_clk = ClockSignal("sys"),
            i_a   = self.mult_hw.params_a.storage,
            i_b   = self.mult_hw.params_b.storage,
            o_resultado = self.mult_hw.resultado.status
        )
        platform.add_source("multiplicador.v")

# -----------------------------------------------------------------------------
# 5. Main: build y sim
# -----------------------------------------------------------------------------
if __name__ == "__main__":
    soc = MiSoC()
    sim_config = SimConfig()
    
    # 2. Añadimos un "clocker" (generador de reloj) 
    # El nombre "sys_clk" debe coincidir con el nombre en tu _io
    sim_config.add_module("serial2console", "serial")
    sim_config.add_clocker("sys_clk", freq_hz=int(1e6))

    builder = Builder(soc, output_dir="build_soc", csr_csv="test/csr.csv")
    builder.build(build_name="sim", sim_config=sim_config, trace=True,opt_level   = "O0",)