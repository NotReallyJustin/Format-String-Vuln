#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Objective: Overwrite GOT entry for printf() with GOT entry for crash()
// This assumes you've disabled ASLR. However, if you were to write shellcode, I think you might not need to disable ASLR depending on how you do it.
void crash()
{
    puts("\nAnswer: I don't [REDACTED] know anything; we bought all our training data off Reddit because our last 2 AI products were a complete failure and our stocks aren't looking good.");
    puts("If we put half of the money we dumped into AI into Cybersecurity, this stuff wouldn't be happening: https://www.spiceworks.com/it-security/data-security/news/google-privacy-breaches-2013-2018/");
}

int main(int argc, char* argv[])
{
    // Enable this if you have to
    // But you shouldn't need to though if ASLR is disabled 
    // printf("Btw, the address for crash is at %p.\n", crash);

    puts("----------------------------Google AI--------------------------------");

    if (argc != 2)
    {
        puts("I'm sorry, but you must insert a one word search query.");
    }
    else
    {
        char query[1024];
        strncpy(query, argv[1], 1024);

        printf("Search Query: ");

        // Format String Vulnerability
        printf(query);

        // Overwrite this printf.
        printf("\nAnswer: First, try jumping off the Golden Gate Bridge. Then, try adding glue to pizza. If none of those things work, eat some rocks.\n");
    }

    puts("----------------------------------------------------------------------");

    exit(0);
}
