__kernel void mad_test(__global uint *result_int, __global ulong *result_long) {

   uint a = 0x12E23456;
   uint b = 0x12E12233;
   uint c = 0x111111;
   uint d = 0x0;

   ulong e = 0x12E23456;
   ulong f = 0x12E12233;
   ulong g = 0x111111;
   ulong h = 0x0;
   
   result_int[0] = (a*b);
   result_int[1] = mad_hi(a, b, d);

   result_long[0] = (e*f);
   result_long[1] = mad_hi(e, f, h);
}
