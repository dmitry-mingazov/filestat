# filestat

### descrizione

Sviluppare una applicazione di sistema Unix/Linux chiamata `filestat` che sia in grado di monitorare un insieme di file raccogliendo informazioni su\:
- dimensione (in byte);
- permessi;
- data ultimo accesso al file;
- data ultima modifica del file;
- data ultima modifica dei permessi;
- numero di link al file;
- proprietario;
- gruppo

Queste informazioni sono recuperabili usando le funzioni `stat()`, `lstat()` e `fstat()`. 

La sinossi del programma è\:   
`filestat [options] [input] [output]`
Dove (NB: le opzioni lunghe iniziano con il doppio `-`)\:  
- `input` è il file di input dove vengono definiti i parametri di esecuzione del programma, se omesso viene usato il file `filestat.in`;
- `output` è il file di output dove vengono collezionati i dati raccolti, se omesso viene usato il file `filestat.db`;
- `options` è la lista di opzioni\:
    - `–verbose|-v`\: durante l'esecuzione il programma mostra a video le informazioni sui file elaborati, ed i dati raccolti;
    - `–stat|-s`\: vengono mostrate sullo standard output le seguenti statistiche\:
        1. numero di file monitorati;
        2. numero di link;
        3. numero di directory;
        4. dimensione totale;
        5. dimensione media;
        6. dimensione massima;
        7. dimensione minima (in byte).
    - `–report|-r`\: al termine dell'esecuzione vengono mostrati sullo standard output le informazioni riguardanti numero di file elaborati, tempo di elaborazione, dimensione massima del file;
    - `–history|-h <filepah>`\: stampa sullo standard output la cronologia delle informazioni riguardanti il file `<filepah>`;
    - `–user|-u <userId>`\: stampa sullo standard output le informazioni di tutti i file di proprietà di `<userId>`
    - `–group|-g <groupId>`\: stampa sullo standard output le informazioni di tutti i file di proprietà di `<groupId>`
    - `–length|-l <min>:<max>`\: stampa sullo schermo le informazioni di tutti i file di dimensione (in byte) compresa tra `<min>` e `<max>` (`:<max>`indica ogni file di dimensione al più `<max>`, `<min>:` e `<min>` indicano ogni file di dimensione almeno `<min>`) 

### Formato file di input

I parametri di esecuzione di un programma vengono definiti in un file di testo costituito da una sequenza di righe della seguente forma\:  
`<path> [r] [l]`  
Dove `r` indica che occorre leggere ricorsivamente i file nelle directory sottostanti (applicando le stesse opzioni) mentre `l` indica che i link devono essere trattati come file/directory regolari, in questo caso le informazioni collezionate fanno riferimento al file riferito dal link e non a link stesso. 

### Formato file di output

I dati raccolti vengono salvati usando il seguente formato\:  
```
 # <path1> <data1> <uid1> <gid1> <dim1> <perm1> <acc1> <change1> <mod1> <nlink1> … <data_n> <uid_n> <gid_n> <dim_n> <perm_n> <acc_n> <change_n> <mod_n> <nlink_n>

# <path2> <data1> <uid1> <gid1> <dim1> <perm1> <acc1> <change1> <mod1> <nlink1> … <data_n> <uid_n> <gid_n> <dim_n> <perm_n> <acc_n> <change_n> <mod_n> <nlink_n>

… # <pathm> <data1> <uid1> <gid1> <dim1> <perm1> <acc1> <change1> <mod1> <nlink1> … <data_n> <uid_n> <gid_n> <dim_n> <perm_n> <acc_n> <change_n> <mod_n> <nlink_n> ### ### 
```
Le informazioni associate al file/directory `<path>` iniziano con la riga\:  
`# <path>`  
Successivamente si trovano una sequenza di righe (una per ogni analisi svolta) della forma\:  
`<data> <uid> <gid> <dim> <perm> <acc> <change> <mod> <nlink>`  
Dove\:  
    - `<data>` indica ora-data in cui sono recuperate le informazioni;
    - `<uid>` è l'id dell'utente proprietario del file;
    - `<gid>` è l'id del gruppo del file;
    - `<perm>` è la stringa con i diritti di accesso al file;
    - `<acc>` data dell'ultimo accesso;
    - `<change>` data dell'ultimo cambiamento
    - `<mod>` data dell'ultima modifica dei permessi;
    - `<nlink>` numero di link verso il file.
    
Le informazioni terminano con la riga\:   
`###`  
Il file termina con una riga\:  
`###`  

