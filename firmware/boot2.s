# test_csr.S
.section .text
.global _start

_start:
    # 1. Escribir un valor en tu acelerador (suponiendo que está en 0xf0001000)
    li t0, 0xf0000000  # Carga la dirección del CSR de tu hardware
    li t1, 0x2a        # El valor que quieras (42 en decimal)
    sw t1, 0(t0)       # Almacena el valor en el CSR 0xf0000000
    li t0, 0xf0000004  # Segundo registro para el calculo
    li t1, 0x10        # 16 En hexadecimal
    sw t1, 0(t0)       # Almacena el valor en el CSR 0xf0000004

    # 2. Devolver el control a la BIOS
    # Simplemente hacemos un soft reset
    li   t2, 0x00000000
    jr   t2