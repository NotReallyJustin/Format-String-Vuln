## Writing Values with Format String (Part 2)
`%n` requires us to feed it a pointer. However, most people don't program like that. <br />
You're far more likely to see something like:
```c
int password = 10;

if (input == password)
{
    do_something();
}
```
Which - granted - is still dumb because we could reverse engineer the password, but that does make it very hard for us to modify `password` since it is not a pointer.
<br /><br />

What we're going to do is craft a format string query - and then **reference ourselves**. <br />
ELI5: We're going to write our own pointer and modify our own pointer
<br /><br />
So if an integer is at `0x00000100`, we're literally going to put `0x00000100` in a pointer we control, and call our own pointer so `%n` can modify that.
<br /><br />

The gist is by doing something like `$(printf <pointer>)        %<size>x        %<offset of our own pointer>$n`, we can write to any address we put into `<pointer>`

## Important Caveats and Concepts before we get started
### Little Endian Form
Most Linux distros I've worked with are little-endian. In that case, your addresses are actually going to be reversed since the Least Significant Bit will be at the Lowest address (memory goes from high --> low in the stack, so technically LSB is the largest but we don't need to get into that)
<br /><br />
Hence, if I want to craft a pointer to `0x12345678`, it's actually be `\x78\x56\x34\x12`

### Pointer Size
On a 32-bit system, this is 4 bytes. On a 64-bit system, this is 8 bytes. When you're writing a pointer address, make sure it's actually 4 and 8 bytes, respectively. If you don't do this, you would very likely segfault as C reads in an invalid address

### Write Size
Believe it or not, pointers also take up bytes! When you do `$(printf "\x12\x23\x34\x45")       %2$n`, the bytes take up 4 bytes! When you do memory calculations for `%<size>x`, take that into account

## Splitting Up Write Sizes
Don't tinker with this if you're new to pwn. <br />
If you want to, you can write in 1 byte intervals to least significant bits. In that case, remember that memory goes from `high --> low`, so if in little endian form your pointer is `0x12345678`, your next byte would be `0x12345679`. Make sure to add up pointer sizes correctly because something like:
<br><br>

`$(printf "\x12\x23\x34\x45.\x13\x23\x34\x45")` would take up 8 bytes. And you would need to deal with the size of your existing `%<size>x`.

## Memory Subtraction
You cannot subtract memory. If I wrote `0xff` to my last LSB and that was way beyond my `0x23` (remember that I need to use that `0xff` because of how pointer sizes and memory work), use overflow instead. Find `0x123` since that extra `1` will either be overwritten, or it wouldn't matter because overflow.

## ⭐ ASLR ⭐
If you take ⭐⭐ one thing away from `notes.md`, let it be this ⭐⭐ <br />
When you write to arbritrary memory (basically, when you write to your own pointers), your exploit heavily relies on the address you're passing in being correct. This is **not** the case. <br /><br />

Because of ASLR, memory addresses
1. Are unpredicatble
2. Changes every time you run the program

What this means for you is:
1. This hack does not work unless you disable ASLR via 
```sh
echo 0 | sudo tee /proc/sys/kernel/randomize_va_space
```
2. You run it in `$gdb` since they do their own thing with addresses. However, this is unrealistic and we're not going to try this approach
3. You find an exploit that bypasses ASLR, which is too complicated for us to cover here

We got away with this in WriteValues and ReadValues because we dealt with **memory offsets** on the stack, which do not change. But when you're specifying a memory location, ASLR fucks you up. 
<br><br>

With that being said, you don't need to worry about other protections like Stack Canary, since we're not doing an overflow attack

# PwnTools (For Writing Hex Data)
You might have noticed that when you're prompted by the terminal to insert something, you actually inserted ASCII characters. <br />
But... memory addresses are written in hex (or binary, if you want to torture yourself). This means we need a way to type hex into input fields. <br /><br />

This is where we'll use `pwntools`. Install it via `$pip install pwntools` and you're good to go. Here's a few functions you might like:
<br><br>
This registers an ELF file.

```python
ELF("insert file path here")
```

This starts the ELF file as a process.
```python
elfObject.process()
```

These converts things to little endian form for you 
```python
p32(0xaabbccdd)           # For 32-bit ELF
p64(0xaabbccdd)           # For 64-bit ELF
```

This sends a payload/writes to stdin
```python
payload = "A%7$s"
process.sendline(payload)
```

This opens up a terminal for you to look at what's being printed
```python
p.interactive()
```

# Step-by-Step Guide to Exploiting Write Values
If it's possible for you to insert hex into `$gdb` and you actively have a copy of the binary to pwn, you could (and should) still exploit it with `$gdb`. But here, we'll assume that we don't have that luxury.
1. Grab the address to write to. You can do this by brute forcing via inserting multiple `%p` until you find the address with the content you want, or you could find a code that leaks the pointer. **Note: If there is an existing pointer you can hijack, don't do this and go to WriteValues (Part 1)**
2. Run `$gdb` and put breakpoints before `printf`
3. Grab the offset of your input (which would happen to be your own pointer)
4. Open up and set up `pwntools` for your ELF
5. Craft the payload: `$(printf <pointer>)        %<size>x        %<offset of our own pointer>$n`
6. Send the payload using `pwntools`