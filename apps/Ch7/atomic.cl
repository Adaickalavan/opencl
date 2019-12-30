__kernel void atomic(__global int* x) {

   __local int a; 
   __local int b;

   a = 0; 
   b = 0;
   barrier(CLK_LOCAL_MEM_FENCE);

   /* Increment without atomic add */
   (a)++;

   /* Increment with atomic add */
   atomic_inc(&b);

   x[0] = a;
   x[1] = b;
}
