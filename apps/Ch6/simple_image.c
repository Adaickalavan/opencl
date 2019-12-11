#define _CRT_SECURE_NO_WARNINGS // Disable deprecation
#define NUM_FILES 1
#define PROGRAM_FILE "./apps/Ch6/simple_image.cl"
#define KERNEL_FUNC "simple_image"

#define PNG_DEBUG 3
#include <png.h>

#define INPUT_FILE "./apps/Ch6/simple_image_input.png"
#define OUTPUT_FILE "./apps/Ch6/simple_image_output.png"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

#ifdef MAC
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

void read_image_data(const char* filename, png_bytep* data, size_t* w, size_t* h) {

   /* Open input file */
   FILE *png_input;
   if((png_input = fopen(filename, "rb")) == NULL) {
      perror("Can't read input image file");
      exit(1);
   }

   /* Read image data */
   png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
   png_infop info_ptr = png_create_info_struct(png_ptr);
   png_init_io(png_ptr, png_input);
   png_read_info(png_ptr, info_ptr);
   *w = png_get_image_width(png_ptr, info_ptr);
   *h = png_get_image_height(png_ptr, info_ptr);

   png_size_t sz = png_get_rowbytes(png_ptr, info_ptr);
   png_byte channels = png_get_channels(png_ptr, info_ptr);
   png_byte color_type = png_get_color_type(png_ptr, info_ptr);
   png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
   png_byte filter_type = png_get_filter_type(png_ptr, info_ptr);
   printf("Input image row size = %ld bytes\n",sz);
   printf("Input image channels = %d \n",channels);
   printf("Input image color type = %d \n",color_type);
   printf("Input image bit depth = %d bits\n",(uint8_t)(bit_depth));
   printf("Input image filter type = %d\n",(uint16_t)(filter_type));

   /* Allocate memory and read image data */
   *data = malloc(*h * png_get_rowbytes(png_ptr, info_ptr));   
   for(int ii=0; ii<*h; ii++) {
      png_read_row(png_ptr, *data + ii * sz, NULL);
   }

   // Print values of pixels in input image 
   printf("Input image:\n");
   for(int ii=0; ii<*h; ii++){
      for(int jj=0; jj<*w; jj++){
         printf("%d ",(*data)[*w*ii + jj]);
      }
      printf("\n");
   }

   /* Close input file */
   png_read_end(png_ptr, info_ptr);
   png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
   fclose(png_input);
}

void write_image_data(const char* filename, png_bytep data, size_t w, size_t h) {

   /* Open output file */
   FILE *png_output;
   if((png_output = fopen(filename, "wb")) == NULL) {
      perror("Create output image file");
      exit(1);
   }

   /* Write image data */
   png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
   png_infop info_ptr = png_create_info_struct(png_ptr);
   png_init_io(png_ptr, png_output);
   png_set_IHDR(png_ptr, info_ptr, w, h, 16,
         PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE,
         PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
   png_write_info(png_ptr, info_ptr);

   png_size_t sz = png_get_rowbytes(png_ptr, info_ptr);
   png_byte channels = png_get_channels(png_ptr, info_ptr);
   png_byte color_type = png_get_color_type(png_ptr, info_ptr);
   png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
   png_byte filter_type = png_get_filter_type(png_ptr, info_ptr);
   printf("Output image row size = %ld bytes\n",sz);
   printf("Output image channels = %d \n",channels);
   printf("Output image color type = %d \n",color_type);
   printf("Output image bit depth = %d bits\n",(uint8_t)(bit_depth));
   printf("Output image filter type = %d\n",(uint8_t)(filter_type));

   for(int ii=0; ii<h; ii++) {
      png_write_row(png_ptr, data + ii*sz);
   }

   // Print values of pixels in input image 
   // printf("Output image:\n");
   // for(int ii=0; ii<h; ii++){
   //    for(int jj=0; jj<w; jj++){
   //       printf("%d ",(data)[w*ii + jj]);
   //    }
   //    printf("\n");
   // }
   printf("Output image:\n");
   for(int i=0; i<32; i+=8) {
      printf("0x%X  %X     0x%X  %X     0x%X  %X     0x%X  %X \n", 
         data[i], data[i+1], data[i+2], data[i+3], data[i+4], data[i+5], data[i+6], data[i+7]);
   }

   /* Close file */
   png_write_end(png_ptr, NULL);
   png_destroy_write_struct(&png_ptr, &info_ptr);
   fclose(png_output);
}

int main(int argc, char **argv) {

   /* Host/device data structures */
   cl_device_id device;
   cl_context context;
   cl_command_queue queue;
   cl_program program;
   const char* fileNames[] = {PROGRAM_FILE};
   const char options[] = "-cl-no-signed-zeros";  
   cl_kernel kernel;
   cl_int err;

   /* Image data */
   png_bytep pixels;
   cl_image_format png_format;
   cl_mem input_image, output_image;
   size_t origin[3], region[3];
   size_t width, height;

   /* Data and buffers */
   float test[16];      
   cl_mem test_buffer;

   /* Open input file and read image data */
   read_image_data(INPUT_FILE, &pixels, &width, &height);

   /* Create a device and context */
   device = create_device();
   context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
   if(err < 0) {
      perror("Couldn't create a context");
      exit(1);
   }

   /* Build the program and create a kernel */
   // program = build_program(context, device, PROGRAM_FILE);
   program = createProgramFromFile(NUM_FILES, fileNames, context, device, options);
   kernel = clCreateKernel(program, KERNEL_FUNC, &err);
   if(err < 0) {
      printf("Couldn't create a kernel: %d", err);
      exit(1);
   };

   /* Create image object */
   png_format.image_channel_order = CL_LUMINANCE;
   png_format.image_channel_data_type = CL_HALF_FLOAT;
   input_image = clCreateImage2D(context, 
         CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, 
         &png_format, width, height, 0, (void*)pixels, &err);
   
   // output_image = clCreateImage2D(context, 
   //    CL_MEM_WRITE_ONLY, &png_format, width, height, 0, NULL, &err);
   cl_image_desc image_desc_output;
   image_desc_output.image_type = CL_MEM_OBJECT_IMAGE2D;
   image_desc_output.image_width = width;
   image_desc_output.image_height = height;
   image_desc_output.image_row_pitch = 0;
   output_image = clCreateImage(context, 
         CL_MEM_WRITE_ONLY, 
         &png_format, 
         &image_desc_output,
         NULL, 
         &err);      
   if(err < 0) {
      printf("%d\n",err);
      perror("Couldn't create the image object");
      exit(1);
   }; 

   size_t element_size;
   clGetImageInfo (input_image, CL_IMAGE_ELEMENT_SIZE,
      sizeof(size_t), &element_size, NULL);
   printf("Element size = %ld bytes\n", element_size);

   /* Create a write-only buffer to hold the output data */
   test_buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
         sizeof(test), NULL, &err);
   if(err < 0) {
      perror("Couldn't create a buffer");
      exit(1);   
   };

   /* Create kernel arguments */
   err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input_image);
   err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &output_image);
   err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &test_buffer);
   if(err < 0) {
      printf("Couldn't set a kernel argument");
      exit(1);   
   }; 

   /* Create a command queue */
   queue = clCreateCommandQueueWithProperties(context, device, 0, &err);
   if(err < 0) {
      perror("Couldn't create a command queue");
      exit(1);   
   };

   /* Enqueue kernel */
   size_t dim = 2;
   size_t global_size[] = {width, height};
   size_t* global_offset = NULL;
   size_t* local_size = NULL;
   err = clEnqueueNDRangeKernel(queue, kernel, dim, global_offset,
         global_size, local_size, 0, NULL, NULL);      
   if(err < 0) {
      perror("Couldn't enqueue the kernel");
      exit(1);
   }

   /* Read the image object */
   origin[0] = 0; origin[1] = 0; origin[2] = 0;
   region[0] = width; region[1] = height; region[2] = 1;
   err = clEnqueueReadImage(queue, output_image, CL_TRUE, origin, 
         region, 0, 0, (void*)pixels, 0, NULL, NULL);
   if(err < 0) {
      perror("Couldn't read from the image object");
      exit(1);   
   }

   /* Read and print the result */
   err = clEnqueueReadBuffer(queue, test_buffer, CL_TRUE, 0, 
      sizeof(test), &test, 0, NULL, NULL);
   if(err < 0) {
      perror("Couldn't read the buffer");
      exit(1);   
   }

   printf("Test buffer output:\n");
   for(int i=0; i<16; i+=4) {
      printf("%.2f     %.2f     %.2f     %.2f\n", 
         test[i], test[i+1], test[i+2], test[i+3]);
   }

   printf("Output image:\n");
   for(int i=0; i<32; i+=8) {
      printf("0x%X  %X     0x%X  %X     0x%X  %X     0x%X  %X \n", 
         pixels[i], pixels[i+1], pixels[i+2], pixels[i+3], pixels[i+4], pixels[i+5], pixels[i+6], pixels[i+7]);
   }

   /* Create output PNG file and write data */
   write_image_data(OUTPUT_FILE, pixels, width, height);

   /* Deallocate resources */
   free(pixels);
   clReleaseMemObject(test_buffer);
   clReleaseMemObject(input_image);
   clReleaseMemObject(output_image);
   clReleaseKernel(kernel);
   clReleaseCommandQueue(queue);
   clReleaseProgram(program);
   clReleaseContext(context);
   return 0;
}
