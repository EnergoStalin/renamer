# renamer
####Usage:
`renamer [**DIR**] ~~-g~~ [**GOOD DIR**] ~~-b~~ [**BAD DIR**] -e [**FILE EXTENSIONS**] ~~-i~~ [**IGNORED FILES**] ~~-wl~~ [**FORMAT**] -nr`
- Renames files in specified directory by pattern and puts it in specified bad folder or specified good folder.
- If good dir not specified puts renamed files at the same folder, if bad dir not specified creates error dir in working directory and puts bad files in it.
- File extensions must starts with dot and be in file with -ef or be splitted by ',' with -e flag.
- -wl Flag enable logging who creates 'logs' directory and puts to it all logs who been writed. You may specify log filename. Format you can see here [strftime](http://www.cplusplus.com/reference/ctime/strftime/).
- -i Specifies the same as -e flag ignored files also may be read from file whos also ignored if extension match such as '.txt'
- -nr Flag used for debug it disbales rename and leaves only 'preprocessing' filenames.
> **_Warning!!!_ I recommend this arguments order and forbid variate it. -nr Position doesen't matter.**
