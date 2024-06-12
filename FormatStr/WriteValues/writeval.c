#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

int main()
{
    int* debug = malloc(sizeof(int));
    *debug = FALSE;

    puts("Welcome to the McDonalds! My name is Jalen Hurts, what can I get ya?\n");

    char buf[100];
    // You don't have to use fgets, but it's one of the safer ways of handling input in C. 
    // It'd be dumb to use scanf when we're just exploiting printf down the line
    fgets(buf, 100, stdin);
    buf[strlen(buf)] = '\0';

    // Format string vulnerability
    printf("\nYou ordered: ");
    printf(buf);

    if (*debug == TRUE)
    {
        puts("---------- CLASSIFIED INFORMATION; DO NOT LEAK TO PUBLIC ---------------");
        puts("- The ice cream machine is still broken");
        puts("- We got called out for that $18 Big Mac Meal on CNN and that's bad for our stocks");
        puts("- The Grimace Shake is coming back");
        puts("- Mayor McCheese never existed");
        puts("- Dak Prescott is set to join our franchise after \"shameful\" NFL season");
        puts("-------------------------------------------------------------------------");
    }
    else
    {
        puts("That will be $373894739484786478324.88.");
    }

    free(debug);
}