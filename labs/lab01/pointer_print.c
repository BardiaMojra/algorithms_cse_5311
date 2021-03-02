#include <stdio.h>
#include <stdlib.h>

void pointerFuncA(int* iptr){
/*Print the value pointed to by iptr*/
printf("Value:  %d\n", *iptr );

/*Print the address pointed to by iptr*/
printf("Value:  %p\n", iptr );

/*Print the address of iptr itself*/
printf("Value:  %p\n", &iptr );
}

int main(){
int i = 1234; //Create a variable to get the address of
int* foo = &i; //Get the address of the variable named i and pass it to the integer pointer named foo
pointerFuncA(foo); //Pass foo to the function. See I removed void here because we are not declaring a function, but calling it.

return 0;
}
