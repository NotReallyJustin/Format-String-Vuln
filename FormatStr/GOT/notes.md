## Overwriting the Global Offset Table (GOT)
On our last episode, we learned how to write to any memory address we could possibly imagine. Well... provided you do so properly because Segfaults aren't fun. <br />
In this episode, we will hijack the Global Offset Table so the executable binary can run any code we want.

## What is the GOT?
To keep things short, the GOT is a portion in memory that stores shared library functions (mainly dynamically linked ones). You might also know them as `Dynamic Relocation Records`. It looks something like this:

```
OFFSET   TYPE              VALUE
00003fe8 R_386_GLOB_DAT    __gmon_start__@Base
00003ff0 R_386_GLOB_DAT    _ITM_registerTMCloneTable@Base
00004000 R_386_JUMP_SLOT   __libc_start_main@GLIBC_2.34
00004004 R_386_JUMP_SLOT   printf@GLIBC_2.0
00004008 R_386_JUMP_SLOT   puts@GLIBC_2.0
0000400c R_386_JUMP_SLOT   exit@GLIBC_2.0
00004010 R_386_JUMP_SLOT   strncpy@GLIBC_2.0
```

Think of the GOT as an array of function pointers. So if I go to GOT[3] (with address `0x00004004`), I can grab function `printf` and run it directly from there. 
<br /><br />

See where I'm going with this? If I overwrite GOT[3] (with address `0x00004004`) with my own function pointer, the next time C calls `printf`, it'll actually call my malicious function.

<br /><br />

*But what if there are no shared library functions in a program?* <br />
Don't worry. There's **always** going to be shared library functions. Even if a program somehow doesn't use `printf` or `puts`, they will always use things like `exit`.

## Important Concepts to Keep in Mind
### RELRO
This stands for RELocation Read-Only. This is C's way of protecting memory inside the GOT. <br />
There's two types of RELRO:

1. Partial RELRO - It just moves the GOT around in the process image (usually above `.bss``) so that GOT is immune to buffer overflow. We don't care about that because we're not doing buffer overflow
2. Full RELRO - This is **bad**. If full RELRO is enabled, the GOT is read-only. You cannot modify its value. This whole exploit doesn't work. If this happens, you're gonna need more advanced exploitation methods.

Luckily for you, `$gcc` defaults to Partial RELRO because:
1. Full RELRO is incredibly slow to start up/set up
2. Full RELRO is a bit dumb because if an external shared library (or DLL) changes their location or updates their code, you're fucked

## ASLR and GOT
Remember how we said offsets aren't affected by ASLR? That means that the Offsets (address locations of the GOT "array") will not change regardless of what ASLR does. <br /><br />

This is why you see blogs online say "GOT entries are fixed per binary." <br><br>

However, if you want to replace a function pointer in GOT with a function pointer that exists in the C program (like this demo does), then you'll need to worry about ASLR shuffling that bit of memory around. If you want to launch shellcode, that's going to be a bit weird and depending on how you go about it, you don't really need to worry about ASLR.

## Short Writes
You could theoretically not use these. I actually reccomend not using these if you're just starting out. <br>
However, doing something like `A      $88332211x      %4$m` is going to take a while. If you want to avoid this, you can split your payload into two parts:
<br><br>
```python
>\x06\x40\x00\x00<        \x04\x40\x00\x00        >%22093x%4\$hn<       %2920x%5\$hn
```

`$hn` is a half word. These are two bytes. Wait... this means we can write 2 bytes at a time to craft a 4 byte input!<br />
First, you write to the >upper half< (`0x00004006`). Then, you write to the lower half (`0x00004004`). Remember that because memory in the stack goes from `high --> low`, your "upper half" would be 2 bytes more than your lower half (ie. `0x00004006 = 0x00004004 + 2`).

<br><br>
Just make sure you add up your pointer sizes correctly.

## Format String Vulnerability on the Command Line
*ANY* user input could be susceptible to a format string vulnerability! Programs don't need to specifically read from `stdin` in order for a format string vulernability to happen. <br /><br />

This demo will try exploiting format strings through the command line (via `argv[1]`). Keep these tips in mind when you do this:
1. You still can't write hexes (and hence, memory locations). However, the bash shell allows you to use `$()`. This means you can run python scripts and pass the input into argv. For example: `$(print(b"\x14\x23\x12\x13" + b"%4\$n"))`
2. `$` is a finnicky character on the command line. It's good practice to put an escape character it like this: `\$` 

## Accessing GOT
You can access the Global Offset Table via `objdump -R [binary file path]`

## Accessing Function Locations via GDB
If ASLR is off, `gdb` memory addresses are going to be the same as your normal ELF memory addresses! This means you can run `info functions ^[function name]` to grab their memory address. This - you guessed it - could be written into a GOT "array" entry.

## Steps to Overwriting GOT
1. Use `objdump -R [binary file path]` to dump the GOT table
2. Find a shared library function to hijack. Write down their offset location (basically, memory address on the "array")
3. Run `gdb` on the binary file. Then, run `info functions ^[function name]` to locate the memory of the function you want to run instead of the shared library function. Alternatively, if you're trying to launch shellcode, locate the memory address of `/bin/sh` (probably in your env)
4. Either use `gdb` or experiment with `%x.%x.%x.%x....` to find the offset of your input. Remember that we're doing this to have the program reference our own pointer that we created.
5. Craft a format string vulnerability `\x12\x23\xfa\xbc    %[function pointer address - 4]     %[input offset]$n`. You can get creative with this by writing with bytes or half words.