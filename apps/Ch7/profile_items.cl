__kernel void profile_items(__global int4 *x, int num_ints) {

   int num_vectors = num_ints/(4 * get_global_size(0));

   x += get_global_id(0) * num_vectors;
   for(int i=0; i<num_vectors; i++) {
      // x[i].s0 += 1;
      // x[i].s1 += 2;
      // x[i].s2 += 3;
      x[i] += 1;
   }
}
