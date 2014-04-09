#define A(i, j) A[(i)*n+(j)]

__kernel                                            
void vmul(
		int	n,
		__global float* A,                        
        __global float* x,                        
        __global float* y)                        
{                                                   
                                                    
	int gid = get_group_id(0);                      
   	int tid = get_local_id(0);
	int tnum = get_local_size(0);
	int gnum = get_num_groups(0);
   	                                                 
	int i, j;
	
	__local float s_res[256];

	for (i = gid; i<n; i+=gnum) { 

		float l_res = 0;

		for (j=tid; j<n; j+=tnum) {
			l_res += A(i, j)*x[j];
		}
		s_res[tid] = l_res;

		for (unsigned int s=1; s<tnum; s *= 2) {
			barrier(CLK_LOCAL_MEM_FENCE);
			if ((tid%(s*2))==0) {
				s_res[tid] += s_res[tid+s];
			}	
		}
		if (tid==0) {
			y[i] = s_res[0];
		}
	}
}
