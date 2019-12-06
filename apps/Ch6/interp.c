#define _CRT_SECURE_NO_WARNINGS
#define NUM_FILES 1
#define PROGRAM_FILE "./apps/Ch6/interp.cl"
#define KERNEL_FUNC "interp"

#define SCALE_FACTOR 3

#define PNG_DEBUG 3
#include <png.h>

#define INPUT_FILE "./apps/Ch6/interp_input.png"
#define OUTPUT_FILE "./apps/Ch6/interp_output.png"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

#ifdef MAC
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

void read_image_data(const char* filename, png_bytep* input, png_bytep* output, size_t* w, size_t* h) {

   int i;

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

   /* Allocate input/output memory and initialize data */
   *input = (png_bytep)malloc(*h * png_get_rowbytes(png_ptr, info_ptr));
   *output = (png_bytep)malloc(*h * png_get_rowbytes(png_ptr, info_ptr) * SCALE_FACTOR  * SCALE_FACTOR );
   for(i=0; i<*h; i++) {
      png_read_row(png_ptr, *input + i * png_get_rowbytes(png_ptr, info_ptr), NULL);
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
         PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
   png_write_info(png_ptr, info_ptr);
   for(int i=0; i<h; i++) {
      png_write_row(png_ptr, data + i * png_get_rowbytes(png_ptr, info_ptr));
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
   cl_kernel kernel;
   cl_int err;

   /* Image data */
   png_bytep input_pixels, output_pixels;
   cl_image_format png_format;
   cl_mem input_image, output_image;
   size_t width, height;

   /* Open input file and read image data */
   read_image_data(INPUT_FILE, &input_pixels, &output_pixels, &width, &height);

   /* Create a device and context */
   device = create_device();
   context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
   if(err < 0) {
      perror("Couldn't create a context");
      exit(1);
   }

   /* Create kernel */
   // program = build_program(context, device, PROGRAM_FILE);
   char options[20];
   sprintf(options, "-D SCALE=%u", SCALE_FACTOR);     
   const char* fileNames[] = {PROGRAM_FILE};
   program = createProgramFromFile(NUM_FILES, fileNames, context, device, options);
 
   kernel = clCreateKernel(program, KERNEL_FUNC, &err);
   if(err < 0) {
      printf("Couldn't create a kernel: %d", err);
      exit(1);
   };

   /* Create input image object */
   png_format.image_channel_order = CL_LUMINANCE;
   png_format.image_channel_data_type = CL_UNORM_INT16;
   input_image = clCreateImage2D(context, 
         CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, 
         &png_format, width, height, 0, (void*)input_pixels, &err);
   if(err < 0) {
      perror("Couldn't create the image object");
      exit(1);
   }; 

   /* Create output image object */
   output_image = clCreateImage2D(context, 
         CL_MEM_WRITE_ONLY, &png_format, SCALE_FACTOR * width, 
         SCALE_FACTOR * height, 0, NULL, &err);
   if(err < 0) {
      perror("Couldn't create the image object");
      exit(1);
   }; 

   /* Create buffer */
   if(err < 0) {
      perror("Couldn't create a buffer");
      exit(1);
   };

   /* Create kernel arguments */
   err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input_image);
   err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &output_image);
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
   size_t global_size[] = {width, height};
   err = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, global_size, 
         NULL, 0, NULL, NULL);  
   if(err < 0) {
      perror("Couldn't enqueue the kernel");
      exit(1);
   }

   /* Read the image object */
   size_t origin[3] = { 0, 0, 0};
   size_t region[3] = {
      SCALE_FACTOR * width,
      SCALE_FACTOR * height, 
      1};
   err = clEnqueueReadImage(queue, output_image, CL_TRUE, origin, 
         region, 0, 0, (void*)output_pixels, 0, NULL, NULL);
   if(err < 0) {
      perror("Couldn't read from the image object");
      exit(1);   
   }

   /* Create output PNG file and write data */
   write_image_data(OUTPUT_FILE, output_pixels, SCALE_FACTOR * width, SCALE_FACTOR * height);

   /* Deallocate resources */
   free(input_pixels);
   free(output_pixels);
   clReleaseMemObject(input_image);
   clReleaseMemObject(output_image);
   clReleaseKernel(kernel);
   clReleaseCommandQueue(queue);
   clReleaseProgram(program);
   clReleaseContext(context);
   return 0;
}
