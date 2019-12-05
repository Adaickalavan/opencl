__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | 
      CLK_ADDRESS_MIRRORED_REPEAT | CLK_FILTER_NEAREST; 

__kernel void simple_image(__read_only image2d_t src_image,
                        __write_only image2d_t dst_image,
                        __global float *output) {

   /* Compute value to be subtracted from each pixel */
//    uint offset = get_global_id(1) * 0x4000 + get_global_id(0) * 0x1000;
   uint offset = get_global_id(1) + get_global_id(0);

   /* Read pixel value */
   int2 coord = (int2)(get_global_id(0), get_global_id(1));
   uint4 pixel = read_imageui(src_image, sampler, coord);

   /* Subtract offset from pixel */
//    pixel.x = offset;

   /* Write new pixel value to output */
   write_imageui(dst_image, coord, pixel);

   size_t global_size_0 = get_global_size(0);
   size_t global_size_1 = get_global_size(1);
   size_t global_id_0 = get_global_id(0);
   size_t global_id_1 = get_global_id(1);
   size_t offset_0 = get_global_offset(0);
   size_t offset_1 = get_global_offset(1);
   int index_0 = global_id_0 - offset_0;
   int index_1 = global_id_1 - offset_1;
   int index = index_1 * global_size_0 + index_0;
   output[index] = (float)(pixel.x);
}
