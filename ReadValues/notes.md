## Reading Values with Format String
The main reason we can do this is because C will grab arguments for format parameters if you don't supply them with it.
<br /><br />

1. $gdb debug to pinpoint stack before printf (telescope command works really well here)
2. Count the pointer address offset
3. Use `%p<insert # of addresses to look backwards for>$<format string type>` to extract hidden data
<br /><br />
This bypasses all protections in C including ASLR, Stack Canary, etc (because we're not touching the buffer at all).
Remember that if you do choose to examine a string, you are looking for a pointer (and not the string itself)!!! This means things do get kind of funny when you have a char array.
<br /><br />
Neat little trick, but if that's all you're gonna do, just reverse engineer the binary (it's probably much faster)