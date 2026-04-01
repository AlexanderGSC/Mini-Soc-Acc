#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <generated/csr.h>
#include <irq.h>
#include <uart.h>
#include <console.h>

// --- PROTOTIPOS (Para que el compilador sepa que existen antes de usarlas) ---
static char *readstr(void);
static char *get_token(char **str);

// --- FUNCIONES AUXILIARES ---

static char *readstr(void) {
    static char s[64];
    static int ptr = 0;
    char c;

    // Eliminamos el 'if(readchar_nonblock())' 
    // Ahora readchar() esperará internamente a que pulses una tecla
    c = readchar(); 

    switch(c) {
        case 0x7f: // Backspace
        case 0x08:
            if(ptr > 0) {
                ptr--;
                putsnonl("\x08 \x08");
            }
            break;
        case '\r':
        case '\n':
            s[ptr] = 0x00;
            putsnonl("\n");
            ptr = 0;
            return s; // Solo devolvemos la cadena cuando pulsas Enter
        default:
            if(ptr < (sizeof(s) - 1)) {
                putchar(c); // Eco inmediato del carácter
                s[ptr++] = c;
            }
            break;
    }
    return NULL; // Seguimos en la misma línea
}

static char *get_token(char **str) {
    char *c, *d;
    c = (char *)strchr(*str, ' ');
    if(c == NULL) {
        d = *str;
        *str = *str+strlen(*str);
        return d;
    }
    *c = 0;
    d = *str;
    *str = c+1;
    return d;
}

// --- PROGRAMA PRINCIPAL ---

int main(void) {
#ifdef CONFIG_CPU_HAS_INTERRUPT
    irq_setmask(0);
    irq_setie(1);
#endif
    //uart_init(); 
    printf("--- Verilog Accelerator Console ---\n");
    printf("Command: calc <num1> <num2>\n");
    putsnonl(">>> ");
    while(1) {
        // En el modo original, el prompt solo sale cuando readstr devuelve algo
        char *str = readstr(); 
        if(str == NULL) continue;

        char *token = get_token(&str);

        if(strcmp(token, "calc") == 0) {
            char *arg1 = get_token(&str);
            char *arg2 = get_token(&str);

            if(arg1 && *arg1 && arg2 && *arg2) {
                // Usamos %lu porque uint32_t suele ser long unsigned en RISC-V 32
                uint32_t a = strtoul(arg1, NULL, 0);
                uint32_t b = strtoul(arg2, NULL, 0);

                mult_hw_params_a_write(a);
                mult_hw_params_b_write(b);

                uint32_t res = mult_hw_resultado_read();
                printf("Resultado: %lu * %lu = %lu\n", a, b, res);
            } else {
                printf("Uso: calc <n1> <n2>\n");
            }
        } 
        else if(strcmp(token, "reboot") == 0) {
            ctrl_reset_write(1);
        }
        else if(strlen(token) > 0) {
            printf("Unknown Command: %s\n", token);
        }
        putsnonl(">>>> ");
    }
    return 0;
}