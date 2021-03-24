#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_LEN 1000000

int cmp(const void *l, const void *r) {
	return *((int *) l) - *((int *) r);
}

int main(int argc, char **argv) {
        int arr[MAX_LEN] = {0};
        int length = 0;
 
        assert(argc >= 2);
     
        FILE* fpread = NULL;
        fpread = fopen(argv[1], "r");
        if (fpread == NULL) {   
            printf("Read error!");    
            return 0;  
        } 
        for (int i = 0; !feof(fpread); i++) {
            fscanf(fpread, "%d ", &arr[i]);
            length++;
        }
        
        qsort(arr, length, sizeof(int), cmp);
        
        printf("The ruslt is:\n");
        for (int i = 0; i < length; i++)
	    printf("%d ", arr[i]); 
        return 0;
}
