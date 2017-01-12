#include <stdio.h>
static void func(char *p[])
{
    p[0] = "Hello";
    p[1] = "World";
}
int main(int argc, char *argv[])
{
    char *strings[2];
    func(strings);
    printf("%s %s\n", strings[0], strings[1]);
    return 0;
}
