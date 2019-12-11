constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE
   | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_LINEAR;

__kernel void interp(__read_only image2d_t src_image,
                     __write_only image2d_t dst_image,
                     __global float *test_buffer) {

   float4 pixel;

   /* Determine input coordinate */
   float2 input_coord = (float2)
      (get_global_id(0) + (1.0f/(SCALE*2)),
       get_global_id(1) + (1.0f/(SCALE*2)));

   /* Determine output coordinate */
   int2 output_coord = (int2)
      (SCALE*get_global_id(0),
       SCALE*get_global_id(1));

   /* Compute interpolation */
   for(int i=0; i<SCALE; i++) {
      for(int j=0; j<SCALE; j++) {
         pixel = read_imagef(src_image, sampler,
           (float2)(input_coord + 
           (float2)((1.0f*i)/SCALE, (1.0f*j)/SCALE)));

         write_imagef(dst_image, output_coord + 
                      (int2)(i, j), pixel);

         int2 ty = output_coord + (int2)(i, j);
         test_buffer[ty.y*132 + ty.x] = pixel.x;
      } 
   }

   // size_t global_size_0 = get_global_size(0);
   // size_t global_size_1 = get_global_size(1);
   // size_t global_id_0 = get_global_id(0);
   // size_t global_id_1 = get_global_id(1);
   // size_t offset_0 = get_global_offset(0);
   // size_t offset_1 = get_global_offset(1);
   // int index_0 = global_id_0 - offset_0;
   // int index_1 = global_id_1 - offset_1;
   // int index = index_1 * SCALE*global_size_0 + index_0;
   // test_buffer[index] = index;

}
