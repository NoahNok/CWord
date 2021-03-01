# CWord
A Basic File Editor made in C.

This was the result of many hours worth of effort.

## Compilation
Make sure you have NCurses installed, then run the following command:
```bash
gcc *.c -o CWord -lm -lncurses
```

## Features
### Files
- Create Files
- Copy Files
- Delete Files
- Show Files

### Lines
- Append Lines
- Delete Lines
- Insert Lines
- Show Lines
- Show Line Count (+ If file can be R/W)

### Version Control
- Show Changelog 
- Rollback:
  - Append Line
  - Insert Line
  - Delete Line
  - Delete File
  - Create File
- Timestamped (can record deletion and creation of same file multiple times in a row)
- Track any file editied by CWord

### Full Editor
My take on a simplified version of **Nano**
Combines all the previously listed line operations into a TUI editor.

You can navigate using the up and down arrow keys, type out lines next to the prompt and add then by hitting enter.

#### Useful Notes
I recommend 21 lines as the editor size!

Use ```!h ``` for help
**CTRL + D** to delete the current line
**CTRL + E** to leave the editor

#### Requirements
You must have NCurses installed for the editor to work (the program won't compile without it!)



## Known Caveats
- Some menus require a double ENTER press
- When using the Full Editor and leaving, if you then force close the program via **CTRL + C** it may cause your terminal to look weird.
  - Can be fixed by running your terminals respective **clear** command
- Manually editing any **changelog** files will result in unexpected behaviour if done incorrectly. (In most cases Version Control wont work)
