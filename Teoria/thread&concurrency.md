### Chapter 4: Threads & Concurrency

La creazione di un processo è *heavy-weight* (pesante), mentre quella di un thread è *light-weight* (leggera).

Lo standard POSIX non specifica come debbano essere implementati i thread, ma solo come deve avvenire l'interfacciamento verso l'utente.

Con l’evoluzione dei processori, essi sono diventati **multiprocessore**, ovvero composti da più **core**, che possono eseguire operazioni in parallelo.
Per il sistema operativo è molto più conveniente assegnare i **thread** ai core piuttosto che gestire interi processi, per motivi di **velocità** (es. accesso alla memoria condivisa) ed **efficienza**.

L’utente, tuttavia, deve poter programmare in modo semplice, senza doversi preoccupare direttamente dell'associazione tra thread e core.
Da qui nascono due possibili scenari:

1. L’utente crea i propri thread, ma l’OS non li vede, poiché gestisce solo i thread a livello di kernel.
2. L’OS è consapevole dei thread dell’utente e può gestirli direttamente, assegnandoli ai core.

In questi casi, la responsabilità della gestione (incluso lo **scheduler**) può ricadere sull’utente o sul sistema operativo, a seconda del modello adottato.

---

### Multithread Server Architecture

```
CLIENT -------- richiesta --------> SERVER -------- crea un thread --------> THREAD
```

---

### Multicore Programming

* **Data Parallelism**: distribuzione di insiemi di dati tra i vari core, eseguendo le stesse operazioni su ciascun core.
* **Task Parallelism**: distribuzione di diversi compiti tra i core. Ogni core esegue un **compito specifico**. I vari thread possono operare su dati differenti o condividere gli stessi dati.

---

### User & Kernel Threads

```
 ______________________________
|                              |
|        User Threads          | 
|_____________↓________________|
|       Kernel Threads         |
|______________________________|
```

Tuttavia, questi devono essere mappati in qualche modo tra loro:

1. **Many-to-One**: adottato, ad esempio, da Linux per i thread GNU (storicamente). Tutti i thread utente vengono mappati su un singolo thread del kernel. Uno svantaggio importante è che se un thread si blocca, bloccano anche tutti gli altri. Questo modello è oggi poco utilizzato.

2. **One-to-One**: per ogni thread utente esiste un thread del kernel corrispondente. Questo modello offre migliori prestazioni e isolamento.

3. **Many-to-Many**: consente a molti thread utente di essere mappati su un numero variabile di thread del kernel. Implementato, ad esempio, in Windows tramite il pacchetto **ThreadFiber**.

> Esiste anche un quarto modello, chiamato **Two-Level Model**, che combina caratteristiche dei modelli precedenti.

In ogni caso, l’utente **non deve conoscere** quale modello viene adottato dal sistema: la complessità viene nascosta dal sistema operativo o dalla libreria di threading utilizzata.

### Thread Libraries
