#include <stdint.h>
#include <generated/csr.h>
// ... tus defines de registros ...
//#define REG_A   (*(volatile uint32_t *)0xf0000000)
//#define REG_B   (*(volatile uint32_t *)0xf0000004)
//#define REG_RES (*(volatile uint32_t *)0xf0000008)

void uart_flush(void) {
    // Mientras el registro indique que NO está vacío...
    while (uart_rxempty_read() == 0) {
        // Leemos el carácter y lo ignoramos (lo tiramos)
        uart_rxtx_read(); 
    }
}

void putchar(char c) {
    // Usamos las funciones que LiteX ha generado en csr.h
    while (uart_txfull_read()); 
    uart_rxtx_write(c);
}

void print_str(const char *s) {
    while (*s) putchar(*s++);
}

void print_uint32(uint32_t v) {
    char buf[10];
    int i = 0;
    if (v == 0) { putchar('0'); return; }
    while (v > 0) {
        buf[i++] = (v % 10) + '0';
        v /= 10;
    }
    while (--i >= 0) putchar(buf[i]);
}

char getchar(void) {
    while (uart_rxempty_read() == 1); // Bucle mientras esté VACÍO
    return uart_rxtx_read();
}

void read_line(char *buf, int max_len) {
    int i = 0;
    while (i < max_len - 1) {
        char c = getchar(); // Ya bloquea ella sola correctamente

        if (c == 0x03) return; // Control+C por si acaso

        // Manejar el Backspace (por si te equivocas al escribir)
        if (c == '\b' || c == 0x7f) {
            if (i > 0) {
                i--;
                print_str("\b \b");
            }
            continue;
        }

        if (c == '\r' || c == '\n') {
            print_str("\r\n");
            break;
        }

        if (c >= ' ' && c <= '~') { // Solo caracteres imprimibles
            //putchar(c);
            buf[i++] = c;
        }
    }
    buf[i] = '\0';
}

// Una versión ultra-simple de atoi
uint32_t my_atou(const char *s) {
    uint32_t res = 0;
    while (*s >= '0' && *s <= '9') {
        res = res * 10 + (*s - '0');
        s++;
    }
    return res;
}

int main(void) {
    char buffer[32];
    print_str("\n--- Consola de Test del Acelerador ---\n");
    print_str("Escribe 'exit' para volver a la BIOS\n");

    while (1) {
        uart_flush();
        print_str("\nIntroduce operando A: ");
        read_line(buffer, 32);
        if (buffer[0] == 'e') break; // "exit"
        uint32_t a = my_atou(buffer);

        print_str("Introduce operando B: ");
        read_line(buffer, 32);
        uint32_t b = my_atou(buffer);

        // Enviamos al hardware
        mult_hw_params_a_write(a);
        mult_hw_params_b_write(b);

        // Aquí es donde en el futuro esperarás al 'ready'
        // while(!mult_hw_ready_read()); 

        uint32_t res = mult_hw_resultado_read();

        print_str(">>> Resultado HW: ");
        print_uint32(res);
        print_str("\n");
    }

    print_str("Saliendo...\n");
    void (*rom_reset)(void) = (void (*)(void))0x00000000;
    rom_reset();
    return 0;
}