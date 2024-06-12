#include <stdio.h>
#include <string.h>

// If you're attacking this binary outside of $gdb, disable ASLR

int main()
{
    // Task for you to do: Change this
    int max_chicken = 0;

    // If you want to look at it more - especially since ASLR is a thing
    printf("DEBUG: Address of max_chicken is at %p.\n", &max_chicken);

    puts("Welcome to Chipotle's mobile app.");
    puts("Please insert your name: \n");

    char buf[256];
    fgets(buf, 256, stdin);
    buf[strlen(buf)] = '\0';

    printf("Hi ");
    printf(buf);

    // Btw this is how you're supposed to format string blud
    printf("We're only allowed to give you %d grams of chicken today.\n", max_chicken);

    if (max_chicken > 500)
    {
        puts("\nI don't know how we got here, but I guess this is what happens when you film us making burritos.");
    }
}