#include <stdio.h>
#include <stdlib.h>
int const_tri(int* p, int n) {
    if(n < 3) return *p;
    if(n == 3) return *(p + 2);
    else return const_tri(p, n - 1) + const_tri(p, n - 2) + const_tri(p, n - 3);
}
int main() {
    const int x = 1;
    const int* q = &x;
    int* p = (int*)calloc(3, sizeof(int));
    
    
    *p = *q;
    *(p + 1) = *q; 
    *(p + 2) = 2 * (*q);

    fprintf(stdout, "P0: %p P1: %p P2: %p\n", p, (p + 1), (p + 2));
    fprintf(stdout, "%d", const_tri(p, 6));

    free(p); // Don't forget to free the allocated memory when done

    return 0;
}
