#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MAX_LEN 1000000

void merge_with(int **data, int **tmp, int **tmp_res, int self, int oth, int s, int num) {
	if (oth >= s || oth < 0)
		return;

	MPI_Request req;
	MPI_Isend(*data, num, MPI_INT, oth, 0, MPI_COMM_WORLD, &req);
	MPI_Recv(*tmp, num, MPI_INT, oth, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	int i = 0, j = 0;
	if (self < oth) {
		i = 0;
		j = 0;
		for (int k = 0; k < num; ++k) {
			if ((*data)[i] < (*tmp)[j])
				(*tmp_res)[k] = (*data)[i++];
			else
				(*tmp_res)[k] = (*tmp)[j++];
		}
	} else {
		i = num - 1;
		j = num - 1;
		for (int k = num - 1; k >= 0; --k) {
			if ((*data)[i] > (*tmp)[j])
				(*tmp_res)[k] = (*data)[i--];
			else
				(*tmp_res)[k] = (*tmp)[j--];
		}
	}

	int *t = *data;
	*data = *tmp_res;
	*tmp_res = t;
}

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

	MPI_Init(&argc, &argv);
	int r = 0, s = 0;
	MPI_Comm_rank(MPI_COMM_WORLD, &r);
	MPI_Comm_size(MPI_COMM_WORLD, &s);

	int num = length / s;
	int *data = (int *) calloc(num, sizeof(int));
	int *tmp = (int *) calloc(num, sizeof(int));
	int *tmp_res = (int *) calloc(num, sizeof(int));
	int buf_size = num * sizeof(int) + MPI_BSEND_OVERHEAD;

        MPI_Scatter(arr, num, MPI_INT, data, num, MPI_INT, 0, MPI_COMM_WORLD);
	qsort(data, num, sizeof(int), cmp);

	for (int i = 0; i < s / 2; ++i) {
		if (r % 2 == 0) {
			merge_with(&data, &tmp, &tmp_res, r, r + 1, s, num);
			merge_with(&data, &tmp, &tmp_res, r, r - 1, s, num);
		} else {
			merge_with(&data, &tmp, &tmp_res, r, r - 1, s, num);
			merge_with(&data, &tmp, &tmp_res, r, r + 1, s, num);
		}
	}
        
        int* result = NULL;
	result = (int *) calloc(length, sizeof(int));      
    
        MPI_Gather(data, num, MPI_INT, result, num, MPI_INT, 0, MPI_COMM_WORLD);
	if (r == 0) {
        	for (int i = 0; i < length; i++) {
			printf("%d ", result[i]);
		}
	}
	MPI_Finalize();

	free(result);
	free(data);
	free(tmp);
	free(tmp_res);
}
