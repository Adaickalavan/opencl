// #pragma OPENCL EXTENSION cl_khr_global_int32_base_atomics : enable

#define LOCK(a) atomic_cmpxchg(a, 0, 1)
#define UNLOCK(a) atomic_xchg(a, 0)

__kernel void mutex(__global int *mutex, __global int *sum) {

   while(LOCK(mutex));
   *sum += 1;
   UNLOCK(mutex);

   int waiting = 1;
   while(waiting) {
      while(LOCK(mutex));
      if(*sum == get_global_size(0)) {
         waiting = 0;
      }
      UNLOCK(mutex);
   }
}