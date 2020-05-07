# renamer
## Usage

*renamer [DIR] ~~[-g GOOD DIR]~~ ~~[-b BAD DIR]~~ [-e or -ef(read file mode) FILE EXTENSIONS] ~~[-i or -if(read file mode) IGNORED FILES]~~ ~~[-wl FORMAT]~~ ~~[-nr]~~*
## Arguments
- Renames files in specified directory by pattern(who you coud not change) and puts it in specified bad folder or specified good folder.
- If good dir not specified puts renamed files at the same folder, if bad dir not specified creates error dir in working directory and puts bad files in it.
- File extensions must starts with dot and be in file with -ef or be splitted by ',' with -e flag.
- -wl Flag enable logging who creates 'logs' directory and puts to it all logs who been writed. You may specify log filename. Format you can see here [strftime](http://www.cplusplus.com/reference/ctime/strftime/).
- -i Specifies the same as -e flag ignored files also may be read from file whos also ignored if extension match such as '.txt'.
- -nr Flag used for debug it disbales rename and leaves only 'preprocessing' filenames.
- Warning!!! I recommend this arguments order and forbid variate it. [-nr] Position doesen't matter.
> Optional parameters _crossed_ out.
> **_Warning!!!_ I recommend this arguments order and forbid variate it. -nr Position doesen't matter.**
## Conditions
#### Preprocessing
> Preprocessing remove all charaters from name exclude '-' and replace associations with it. If filename starts with **_'05'_** inserts in the beginning _'0'_ or if filename starts with **_'5'_** inserts *_'0'_*.

```c++
switch (name[i]) //name is original (const char *) string
{
	case 'b': newname += '6'; break;
	case 'q': case 'g': newname += '9'; break;
	case '+': newname += '4'; break;
	case '!': case 'L': case 'l': case 'I': case 'i': case 'J': case 'j':
	case ']': case '[': case 't': case 'r': case '|' : newname +='1'; break;
	case 'S': case 's': newname += '5'; break;
	case 'O': case 'o': case ')': newname += '0'; break;
	case 'B': newname += '8'; break;
	case '-': newname += '-'; defis++; break;
}
```

#### Good file criteria
> Example _after preprocessing stage_ *005456349-12** or **005456349-1**
- If has exactly one defis chareter.
- If length without ~~extension e.g. '_.txt_'~~ grather than 7 and lower than 14.
- If digits count after defis lower than **3** and after all starts with **'005'**
#### Bad file criteria
> Example _after preprocessing stage_ *0054449-1** or **0054563493**
- If has zero or more one defis chareter.
- If length without ~~extension~~ '_.txt_' lower than 8 and or grather than 13.
- Or if not starts with *'005'*.