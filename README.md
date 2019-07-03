# filestat

### Descritption

Develop a Unix/Linux system application called `filestat` which is capable of monitoring a set of files collecting info about\:
- size (byte);
- permissions;
- time of last access;
- time of last modification;
- time of last status change;
- number of links at file;
- owner;
- group.

Program synopsis is the following\:   
`filestat [options] [input] [output]`
Where\:  
- `input` is the input file which contains executing arguments of program, if omitted is used `filestat.in`;
- `output` is the output file where collected data is stored, if omitted is used `filestat.db`. Informations contained into output file are updated at any program execution (and not overwrited);

Possible options are the following:
```
 --verbose | -v  
 --stat | -s  
 --report | -r  
 --history | -h <filepath>  
 --user | -u <userID>  
 --group | -g <groupID>   
 --length | -l <min>:<max>   
 --noscan  
 ```

following are the options description:
    - `–verbose|-v`\: during program execution it print informations about elaboreted files, and collected data;
    - `–stat|-s`\: the following stats are shown on standard output\:
        1. number of monitored files;
        2. number of links;
        3. number of directories;
        4. total size;
        5. average size;
        6. maximum size;
        7. minimum size (byte).
    - `–report|-r`\: at the end of execution are shown information about number of monitored files, time of execution, maximum size;
    - `–history|-h <filepah>`\: shows on standard output data history of file `<filepah>`;
    - `–user|-u <userId>`\: shows on standard output info of all files owned by `<userId>`
    - `–group|-g <groupId>`\: shows on standard output info of all files owned by `<groupId>`
    - `–length|-l <min>:<max>`\: shows on standard output info of all files that have size (byte) between`<min>` and `<max>` (`:<max>` stands for any file which is at maximum `<max>` in size, `<min>:` and `<min>` stands for any file which is at least `<min>` in size) 

### Input file format

Executing arguments are defined in a text file made of a sequences of row with the following format\:  
`<path> [r] [l]`  
Where `r` indicates that is necessary read recursively files on subdirectories (applying same options) while `l` indicates that links must be treated as regular file/directory, in this case data collected refers to file reffered by the link and not link itself . 

### Output file format

Collected data is structured as following:  
```
# <path1>  
<data1> <uid1> <gid1> <dim1> <perm1> <acc1> <change1> <mod1> <nlink1>  
…  
<data_n> <uid_n> <gid_n> <dim_n> <perm_n> <acc_n> <change_n> <mod_n> <nlink_n>  
###  
# <path2> 
<data1> <uid1> <gid1> <dim1> <perm1> <acc1> <change1> <mod1> <nlink1> 
…  
<data_n> <uid_n> <gid_n> <dim_n> <perm_n> <acc_n> <change_n> <mod_n> <nlink_n>  
###  
… # <pathm>  
<data1> <uid1> <gid1> <dim1> <perm1> <acc1> <change1> <mod1> <nlink1>  
…  
<data_n> <uid_n> <gid_n> <dim_n> <perm_n> <acc_n> <change_n> <mod_n> <nlink_n>   
###  
### 
```
Info associated at directory/file `<path>` begin with\:  
`# <path>`  
Then there are a sequence of rows (one for every analyze done) with the following format\:  
`<data> <uid> <gid> <dim> <perm> <acc> <change> <mod> <nlink>`  
Where\:  
    - `<data>` indicates time of when info have been collected;
    - `<uid>` is owner's user ID;
    - `<gid>` is owner's group ID;
    - `<perm>` is the permissions string;
    - `<acc>` time of last access;
    - `<change>` time of last modification
    - `<mod>` time of last status change;
    - `<nlink>` number of link forward to file.
    
Info end with the following row\:   
`###`  
File ends with the following row\:  
`###`  

