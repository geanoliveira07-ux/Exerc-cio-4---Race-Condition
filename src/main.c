#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#define STACK_SIZE 1024
#define THREAD_PRIORITY 5

K_THREAD_STACK_DEFINE(thread1_stack_area, STACK_SIZE);
K_THREAD_STACK_DEFINE(thread2_stack_area, STACK_SIZE);
struct k_thread thread1_data;
struct k_thread thread2_data;

/* RECURSO COMPARTILHADO */
int saldo_bancario = 2000;

void processar_compra(int valor_compra, const char *nome_terminal) {
    printk("[%s] 1. Iniciando compra de R$ %d,00...\n", nome_terminal, valor_compra);

    /* PASSO 1: CHECK (Leitura) */
    int saldo_no_momento_da_leitura = saldo_bancario;
    printk("[%s] 2. Leu saldo global -> Visto: R$ %d,00\n", nome_terminal, saldo_no_momento_da_leitura);

    if (saldo_no_momento_da_leitura >= valor_compra) {
        printk("[%s] 3. CHECK OK! Saldo suficiente para a compra.\n", nome_terminal);
        printk("[%s] \x1B[33m[|] Aguardando servidor... (Preempcao! Troca de Thread)\x1B[0m\n\n", nome_terminal);

        /* 
         * PONTO CRÍTICO: O k_msleep obriga o microcontrolador a pausar esta 
         * thread e executar a outra AGORA, enquanto a compra já foi validada
         * mas o dinheiro AINDA NÃO FOI DEBITADO.
         */
        k_msleep(250); 

        /* PASSO 2: ACT (Escrita) */
        printk("[%s] \x1B[32m[+] Retomou! Concluindo debito de R$ %d,00...\x1B[0m\n", nome_terminal, valor_compra);
        
        /* 
         * O SEGREDO DO -1200:
         * A dedução é aplicada em cima do saldo global atual, e não do valor lido no passado.
         * Como ambas as threads passaram pelo 'IF' juntas, as duas farão essa dedução em sequência.
         */
        saldo_bancario = saldo_bancario - valor_compra;
        
        printk("[%s] 4. Saldo global atualizado para: R$ %d,00\n\n", nome_terminal, saldo_bancario);
    } else {
        printk("[%s] -> RECUSADO: Saldo insuficiente.\n\n", nome_terminal);
    }
}

void terminal_1_entry(void *p1, void *p2, void *p3) {
    processar_compra(1500, "Terminal 1");
}

void terminal_2_entry(void *p1, void *p2, void *p3) {
    processar_compra(1700, "Terminal 2");
}

int main(void) {
    /* Pequeno atraso inicial para o monitor serial conectar no PlatformIO */
    k_msleep(1000); 

    printk("\n=============================================================\n");
    printk(" [ ! ] DEMONSTRACAO VISUAL DE RACE CONDITION (TOCTOU) [ ! ]\n");
    printk("=============================================================\n");
    printk(" SALDO INICIAL DO CLIENTE: R$ %d,00\n\n", saldo_bancario);

    /* Dispara as threads simultaneamente */
    k_thread_create(&thread1_data, thread1_stack_area,
                    K_THREAD_STACK_SIZEOF(thread1_stack_area),
                    terminal_1_entry, NULL, NULL, NULL,
                    THREAD_PRIORITY, 0, K_NO_WAIT);

    k_thread_create(&thread2_data, thread2_stack_area,
                    K_THREAD_STACK_SIZEOF(thread2_stack_area),
                    terminal_2_entry, NULL, NULL, NULL,
                    THREAD_PRIORITY, 0, K_NO_WAIT);

    /* Aguarda tempo suficiente para todo o desastre acontecer (250ms do sleep + margem) */
    k_msleep(1000);

    printk("=============================================================\n");
    printk(" [ X ] RESULTADO CATASTROFICO (FALHA DE SINCRONISMO)\n");
    printk("=============================================================\n");
    printk(" SALDO FINAL: R$ %d,00\n", saldo_bancario);
    
    if (saldo_bancario < 0) {
        printk(" >> O sistema autorizou duas compras simultaneas sem lock.\n");
        printk(" >> O cliente gastou R$ 3200,00 tendo apenas R$ 2000,00!\n");
    }

    return 0;
}