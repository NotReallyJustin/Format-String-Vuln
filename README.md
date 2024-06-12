# Format String Vulnerability Demo
This is just here for me to remember how to exploit a format string sometime in the future. <br />
Also this is a public repo because I shared this with some people during the US Cyber Open CTF and I can't share a private repository

## Reccommended Order:
1. ReadValues 
2. WriteValues
3. WriteValues without Pointers
4. GOT (Global Offset Table)

TLDR of Format Strings - Whenever you see `printf()`, ideally we want to hijack the entire program and have it run what we want
