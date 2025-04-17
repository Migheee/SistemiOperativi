# API: Application Program Interface  

## API POSIX  

### Architetture HW  
Esistono due architetture hardware principali:  
- **Little Endian**  
- **Big Endian**  

### Big Endian vs Little Endian  

> **Big Endian**: Da sinistra verso destra (bit più significativo nell'indirizzo più piccolo).  
> **Little Endian**: Da destra verso sinistra (bit più significativo nell'indirizzo più grande).  

---

## La funzione `main` in C  

La funzione `main` può avere due argomenti:  

```c
int main(int argc, char *argv[])
```
- `argc`: lunghezza delle stringhe passate come argomenti.  
- `argv[0]`: nome dell'eseguibile.  

---

## Gestione degli errori nelle API POSIX  

Ogni funzione in C può fallire o meno.  
Le chiamate di sistema POSIX, in caso di fallimento, restituiscono `-1`.  
Quando si verifica un errore, il motivo viene memorizzato nella variabile esterna `errno`.  

Alcune chiamate non falliscono mai, come `perror()`, che serve per stampare il messaggio di errore.  

### Esempio di gestione degli errori  

```c
if(sys_call(....) == -1) {
    perror("Errore nel punto X");
    exit(-1); // solitamente un numero negativo
}
```

C'è un'altra variabile, `extern char **environ`, che contiene le variabili d'ambiente.  

---

# Apertura, Creazione & Chiusura di un file  

## Differenza tra OR Logico e Bitwise  

Esempio di operazione **bitwise OR** tra due byte:  

```
1101 1010  
0111 1011  
------------
1111 1011  (risultato bitwise)
```

---

## Funzioni principali per la gestione dei file  

Le principali funzioni per la gestione dei file in POSIX sono:  

1. `open`  
2. `close`  
3. `read`  
4. `write`  
5. `lseek`  
6. `chmod`  

### Dichiarazioni delle funzioni per creazione e apertura

```c
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int open(const char *pathname, int flag);
int open(const char *pathname, int flag, mode_t mode);
int creat(const char *pathname, mode_t mode);
```

- **Valore restituito**:  
  - Descrittore file (`n >= 0`) in caso di successo.  
  - `-1` in caso di errore, con `errno` impostato.  

- **Parametri**:  
  - `pathname`: percorso assoluto o relativo.  
  - `flags`: modalità di apertura.  
  - `mode`: protezione di accesso.  

---

## Modalità di apertura dei file  

- `O_RDONLY` : solo lettura  
- `O_WRONLY` : solo scrittura  
- `O_CREAT` : solo scrittura 
- `O_TRUNC` : solo scrittura 
---

## Descrittori di file  

I file risiedono in memoria secondaria e possono essere di vari tipi.  
Quando un file viene aperto in un programma, viene gestito tramite **descrittori di file** (`unsigned short int`).  

Quando si apre un file, il sistema operativo assegna il primo descrittore libero.  

### I primi 3 descrittori predefiniti:  

| Descrittore | Nome     | Funzione        |  
|-------------|----------|-----------------|  
| `0`         | `STDIN`  | Input standard  |  
| `1`         | `STDOUT` | Output standard |  
| `2`         | `STDERR` | Errore standard |  


