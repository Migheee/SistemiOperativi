# Processi

Un processo è un *programma in esecuzione*
Lo stack contiene dati temporanei:
1. Funzioni
2. Variabili locali
3. Indirizzi
L'Heap contiene memoria dimanicamente allocata durante esecuzione 

I registri sono nella CPU; ma un programma C permette di accedervi

Normalmente si dice che un programma è un *entità passiva*, il processo invece è *attivo*
Fino a pochi anni fa, solo un processo alla volta poteva essere eseguito.
Poi sono arrivate macchine Multiprocessing, e per ogni processore può essere eseguito un processo

Programma ↔ Processo

In UNIX, i processi sono organizzati ad albero. Ad ogni processo viene associato un identificativo (*PID*). <br>
Il padre di tutti i processi è *init* (UNIX-based), ma in LINUX viene chiamato *systemd*.<br>
Lanciando il comando `tree` è possibile vedere tutto l'albero dei processi. <br>
Nell'albero, ad un certo punto, ci sarà un processo `shell`. <br>
<br>
Si immagina di avere un'eseguibile. Se si vuole lanciare tale, la shell farà `fork` e viene generato un processo figlio (un'altra shell con PID differente rispetto da quello della shell e da quelli presenti nell'albero dei processi). Quindi, il processo figlio viene rimpiazzato con l'eseguibile attraverso `exec`.

Il meccanismo, in generale è `fork` + `exec`, ossia due chiamate di sistema.

PROCESSO p ---> INVOCAZIONE FORK --- > GENERAZIONE FIGLIO ---> INVOCAZIONE EXEC 

eg: <br>

```c 
exec("ls"); //il processo viene rimpiazzato con ls
```

#### DIAGRAMMA DI ESEMPIO
                            PROCESSO         TEMPO

                            Genitore         t
                               ↓             t+k
                             fork()
                             /    \
                       Genitore   figlio
                          ↓          ↓
                                   exit() 
                                     ↓
                                   ZOMBIE

- `fork()` crea due rami: uno per il processo genitore, uno per il figlio.<br>
- Il figlio termina (`exit()`), ma il genitore non esegue `wait()`, quindi il processo figlio non viene *ripulito*.<br>
- Questo crea un processo zombie: ha terminato, ma il suo entry nel process table *esiste ancora* fino a quando il genitore lo gestisce


                               
#### UN ULTERIORE ESEMPIO
                             G
                             ↓
                           fork()
                          /     \
                    genitore     figlio
                      ↓             ↓
                      G             G
                      ↓             ↓
                    wait() <---   exit()
                               \    ↓
                                \ zombie 

- Il figlio termina (`exit()`), ma il genitore ha una chiamata `wait()`. <br>
- In questo caso, anche se il processo figlio entra temporaneamente nello stato zombie, il genitore lo gestisce correttamente e lo rimuove dalla process table. <br>
- Il processo zombie è quindi transitorio, non persistente come nel primo caso. <br>

Quando un processo termina con `_exit()`, il valore passato viene memorizzato nel secondo byte del parametro status, che viene poi letto tramite `wait()` o `waitpid()` nel processo genitore.
Nel byte meno significativo di status è invece presente il numero del segnale che ha eventualmente sospeso o terminato il processo (0 se non viene terminato dall'esterno).

#### C PROGRAM FORKING SEPERATE PROCESS
```c
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(){
    pid_t pid;

    pid = fork();
    if(pid < 0){
        fprintf(stderr, "Fork Failed");
        exit(-1);
    }
    else if(pid == 0){  /* figlio */
        execlp("/bin/ls", "ls", NULL);
    }
    else{  /* padre */
        wait(NULL);
        printf("figlio completato");
    }

    return 0;
}
```

### NOTA
Alcuni OS non cosentono ai figli di vivere de il genitore è terimato. Se un processo termina, tutti i processi figli terminano:
- `cascading termination`: La cascading termination si verifica quando la terminazione di un processo padre comporta anche la terminazione forzata dei suoi processi figli
- la cascading termination viene eseguita dall'OS.

## COMUNICAZIONE TRA PROCESSI
I processi devono poter comunicare tra loro. Se i processi si scambiano informazioni, allora si dicono *indipendenti*; altrimenti si dicono cooperanti.
Avere processi cooperanti può aumentare la velocità di esecuzione, soprattutto nelle architetture moderne (con processori in più core).
Processi cooperanti necessitano di comunicazione tra processi.
Esistono due modelli prinicipali per lo scambio di informazione tra processi:
1. Memoria comune. Necessario usare chiamate di Sistema per allocare uno spazio di memoria condivisa. Si necessitano di alcuni meccanismi di sincronizzazione. 
2. Passaggio dei messaggi: Due processi si scambiano informazione scambiandosi messsaggi. 

I secondi sono più pesanti (si necessita di uno strato per la comunicazione, ma come vantaggio non servono meccanismi di sicronizzazione). 
<br>E' sempre presente una memoria comune, ma questa viene gestita dal **kernel**. Lo scambio di messaggi è completamente *asincrono*. 
In UNIX si usa un canale `pipe` per scambiare messaggi tra due processi. che assumeranno ruoli di:
- writer: aggiunge in coda un messaggio
- reader: legge dalla testa della coda

| Memoria Comune  | Scambio Messaggi     | 
|-----------------|----------------------|
| + efficiente    | - efficiente         |  
| - spazio        | + spazio             |  
| - sicuro        | + sicuro             |   

Dal punto di vista teorico, esistono due possibili varianti:
1. buffer illimitato: non è presente una dimensione massima fissata. In questo caso, chi scrive ha sempre la posssibilità di scrivere. Il lettore aspetta se non c'è buffer da consumare
2. buffer limitato: il buffer ha una dimensiona fissa.

### ESEMPIO MEMORIA COMUNE

```c
#define BUFFER_SIZE 10
typedef struct{
    ... // l'esempio più semplice sarebbe usare un byte (un carattere).
        // E' possibile suddivere il byte in campi di bit, ossia assegnare nome a bit. 
        // Non rende possibile la portabilità [Nota Little Endian vs Big Endian]
}item;

item buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

item next_produced;
while(true){
    //produco un item
    while((in + 1) % BUFFER_SIZE == out)
    ;

    buffer[in] = next_produced;
    in = (in +1) % BUFFER_SIZE;
}

int next_consumed;
while(true){
    //produco un item
    while(in == out)
    ;
    next_consumed = buffer[out];
    out = (out +1) % BUFFER_SIZE;
}

```

### RACE CONDITION
Una *race condition* è un evento che si verifica quando due o più processi o thread accedono e modificano una risorsa condivisa nello stesso momento, e l’esito del programma dipende dall’ordine in cui avvengono questi accessi.


### GESTIONE DEI PROCESSI
Le chiamate di sistema relative alla gestione dei processi che vedremo sono tutte *POSIX*. Se necessario, fare comunque riferimento al manuale (man). <br>
Le chiamate principali sono:
1. `getpid()`, `getppid()`: per ottenere PID
2. `exit()`
3. `exec()`
4. `wait()`
- Gestione dei segnali:
    1. `signal()`
    2. `kill()`
    3. `alarm()`
    4. `pause()`

### STATI DI UN PROCESSO
Un processo può assumere uno dei seguenti stati

1. **Running (in esecuzione)**: Il processo sta attualmente utilizzando la CPU per eseguire le sue istruzioni.

2. **Runnable (pronto)**: Il processo è pronto per essere eseguito, ma è in attesa che la CPU sia disponibile.

3. **Sleeping (in attesa o bloccato)**: Il processo è in attesa di un evento esterno, come l'input/output o la disponibilità di una risorsa.

4. **Suspended (sospeso)**: Il processo è stato temporaneamente sospeso, ad esempio per motivi di gestione delle risorse o per decisioni amministrative.

5. **Zombie**: Il processo ha terminato la sua esecuzione. Può essere ucciso solo se si uccide init/systemd.

6. **Idle (inattivo)**: Questo stato non è standard nei sistemi operativi; tuttavia, in alcuni contesti, può riferirsi a un processo che non ha compiti attivi da eseguire e attende nuove istruzioni.


### `_exit()`
- **Header file:** `<unistd.h>`  
- **Prototipo:** `void _exit(int status);`  
- **Nota:** Questa funzione termina immediatamente il processo chiamante. Non esegue la pulizia di buffer, né chiama i destructors di oggetti statici (a differenza di `exit()`).  
- **Caratteristica:** Non fallisce mai.

---

### `getpid()`, `getppid()`
- **Header file:** `<unistd.h>`  
- **Prototipo:** 
  ```c
  pid_t getpid(void);   // Restituisce il PID del processo chiamante
  pid_t getppid(void);  // Restituisce il PID del processo padre
  ```

---

### `exec`
- **Nota importante:** L'unica vera chiamata di sistema è `execve()`. Tutte le altre (`execl`, `execp`, `execvp`, ecc.) sono wrapper (decoratori) che alla fine chiamano `execve()`.

- **Header file:** `<unistd.h>`  
- **Variabili esterne:**
  ```c
  extern char **environ; // Puntatore alle variabili d'ambiente
  /* Environ è un array di stringhe, contiene tutte le variabili d'ambiente */
  ```
- **Prototipo:**
    ```c
    int execl(const char *path, const char *arg, ...);
    int execlp(const char *file, const char *arg, ...);
    int execle(const char *path, const char *arg, ..., char *const envp[]);
    int execv(const char *path, char *const argv[]);
    int execvp(const char *file, char *const argv[]);

    ```

---

### `execve`
- **Nota importante:** L'unica vera chiamata di sistema
- **Header file:** `<unistd.h>`  
- **Prototipo:**
    ```c
    int execve(const char *filename, const  char *argv[], const char *envp[])
    ```


`execve()` esegue il programma a cui fa riferimento il percorso (`pathname`). Restituisce -1 se fallisce
Questo comporta che il programma attualmente in esecuzione nel processo chiamante venga **sostituito** con un nuovo programma, con:

- **stack** inizializzato da zero,  
- **heap** nuovo,  
- e nuovi segmenti **dati** (sia inizializzati che non inizializzati).

Ciò che rimane invariato e viene ereditato sono:
- **PID**
- **PPID**
- **descrittori di file aperti**

In generale, ogni volta che viene effettuta una chiamata di sistema è necessario un controllo degli errori.



#### DIGRESSIONE SUL COMANDO ECHO
Le variabili d'ambiente sono essere gestiti e impostate da shell. Un esempio è `PATH`
Quindi con *echo $PATH*, è possibile vedere il contenuto della variabile d'ambiente PATH


