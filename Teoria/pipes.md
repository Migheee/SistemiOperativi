### Pipe

Una *pipe* è un **buffer di comunicazione** tra processi.

Dal punto di vista dell’utente, le pipe sono gestite come file: infatti, ogni pipe restituisce **due descrittori di file** (file descriptor):

1. `fd[1]` — **write-end**: usato per scrivere nella pipe
2. `fd[0]` — **read-end**: usato per leggere dalla pipe
<br>
Quando si invoca `pipe(fd)`, viene restituito un array di due elementi contenente questi descrittori, da cui il motivo del doppio file descriptor.

### Comunicazione tra processi<br>
Una pipe consente la comunicazione tra due processi **con un antenato in comune**. Di conseguenza, la pipe deve essere creata **nell’antenato comune**, tipicamente prima di una `fork()`.

### Pipe con nome

Per superare il vincolo della parentela tra processi, si possono usare le **pipe con nome** (named pipes o *FIFO*), che sono veri e propri **file nel filesystem**. Questi permettono una comunicazione più flessibile, anche tra processi non imparentati.

### Comunicazione su macchine diverse

Se i processi si trovano su **macchine diverse**, si utilizzano i **socket**:
I socket sono un meccanismo di comunicazione più potente e flessibile, e possono mettere in comunicazione processi in esecuzione su sistemi diversi attraverso la rete.

E' possibile chiedere al sistema operativo, attraverso la chiamata a funzione, chiedere la creazione di un canale completamente gestito da kernel. La chiamata, restiuisce due descrittori:
1. `fd[1]` — **write-end**: usato per scrivere nella pipe
2. `fd[0]` — **read-end**: usato per leggere dalla pipe

Il lettore, chiude il descrittore di scrittura; mentre il descrittore di lettura viene chiuso dallo scrittore

### `pipe`
- **Header file:** `<unistd.h>`  
- **Prototipo:**
    ```c
    int pipe(int filedes[2])
    ```
- **Parametri**_ filedes: array nel quale vengono salvati i descrittori
- **Return value** -1: errore; 0: successo

Per una pipe, si usa la seguente terminologia:
1. no-wr: no write descriptors are open
2. si-wr: at least one write descriptor is open
3. no-rd: no read descriptors are open
4. si-rd: at least one read descriptor is open

Lettura & Scrittura (da completare leggendo le slide)


### Esempio
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    int rw[2];
    int p;
    pid_t pid;
    int res;
    if((res=pipe(rw))!=-1){
        if((p=fork())==0){
            int num;
            close(rw[1]);
            read(rw[0], &num ,sizeof(stringa));
            printf("%d\n", num);
        }
        else{
            close(rw[0]);
            pid=getpid();
            write(rw[1], &pid, strlen(pid));
        }

    }
    else
        perror("Errore pipe");
        
    return 0;
}
```