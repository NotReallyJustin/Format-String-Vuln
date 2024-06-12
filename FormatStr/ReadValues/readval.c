#include <stdio.h>
#include <string.h>

void never_use_this()
{
}

int main()
{
    // Use %31$s
    char* secretVal = "You prolly can't read this if I compiled the file.";

    printf("Pick a number between 1 - 10.\n");

    char buf[100];
    buf[strcspn(buf, "\n")] = '\0';

    fgets(buf, 100, stdin);

    // Printf vulnerability
    printf(buf);
}