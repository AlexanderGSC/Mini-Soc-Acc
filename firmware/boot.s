.section .text.init  # Sección especial para el arranque
.global _start       # El punto de entrada que busca el Linker
.extern _fstack      # Avisamos que viene del linker script
_start:
    la sp, _fstack   # 'la' carga la dirección definida en el linker
    call main
loop:
    j loop           # VexRisc
