#include <stdio.h>
#include <string.h>

int main() {
    char buff[10];

    printf("Enter a string: ");
    fgets(buff, 100, stdin);  // 🚨 Buffer is only 10 bytes, but we read up to 100!
    
    printf("You entered: %s\n", buff);
    return 0;
}
