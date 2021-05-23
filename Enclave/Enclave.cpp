#include "Enclave_t.h"
#include <sgx_trts.h>

void ecall_matprod(const int* a, const int* b, int* res, int total_len, int len) {
// 	int a_[len * len];
// 	int b_[len * len];
// 	int res_[len * len];
	

// #pragma omp parallel for
// 	for(int i = 0; i < len * len; ++i) {
// 		a_[i] = a[i];
// 		b_[i] = b[i];
// 	}

	int ibl = 16;
	int ib, jb, kb;
	int i, j, k;

#pragma omp parallel for
	for(int i = 0; i < len * len; ++i)
			res[i] = 0;

	start_timer();

#pragma omp parallel for num_threads(6) private(jb, kb, i, j, k)
	for(ib = 0; ib < len; ib += ibl) {
		for(jb = 0; jb  < len; jb += ibl) {
			for(kb = 0; kb  < len; kb += ibl) {
				for (i = ib; i < ib + ibl && i < len; ++i) {
					for (j = jb; j < jb + ibl && j < len; ++j) {
						for (k = kb; k < kb + ibl && k < len; ++k) {
							res[len * i + j] += a[len * i + k] * b[len * k + j];
						}
					}
				}
			}
		}
	}

	stop_timer();

// #pragma omp parallel for
// 	for(int i = 0; i < len * len; ++i) {
// 		res[i] = res_[i];
// 	}
}
