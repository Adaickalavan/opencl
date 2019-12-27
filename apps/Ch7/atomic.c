#define _CRT_SECURE_NO_WARNINGS
#define NUM_FILES 1
#define PROGRAM_FILE "./apps/Ch7/atomic.cl"
#define KERNEL_FUNC "atomic"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "util.h"

#ifdef MAC
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

int main() {

   /* OpenCL data structures */
   cl_device_id device;
   cl_context context;
   cl_command_queue queue;
   cl_program program;
   cl_kernel kernel;
   cl_int err;
   size_t offset, global_size, local_size;

   /* Data and events */
   int data[2];
   cl_mem data_buffer;

   /* Create a device and context */
   device = create_device();
   context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
   if(err < 0) {
      perror("Couldn't create a context");
      exit(1);   
   }     

   /* Build the program and create a kernel */
   // program = build_program(context, device, PROGRAM_FILE);
   const char* fileNames[] = {PROGRAM_FILE};
   const char options[] = "";  
   program = createProgramFromFile(NUM_FILES, fileNames, context, device, options);
   kernel = clCreateKernel(program, KERNEL_FUNC, &err);
   if(err < 0) {
      perror("Couldn't create a kernel");
      exit(1);   
   };

   /* Create a buffer to hold data */
   data_buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
      sizeof(data), NULL, &err);
   if(err < 0) {
      perror("Couldn't create a buffer");
      exit(1);   
   };         

   /* Create kernel argument */
   err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &data_buffer);
   if(err < 0) {
      perror("Couldn't set a kernel argument");
      exit(1);   
   };

   /* Create a command queue */
   queue = clCreateCommandQueue(context, device, 0, &err);
   if(err < 0) {
      perror("Couldn't create a command queue");
      exit(1);   
   };

   /* Enqueue kernel */
   offset = 0;
   global_size = 8;
   local_size = 4;
   err = clEnqueueNDRangeKernel(queue, kernel, 1, &offset, &global_size, &local_size, 0, NULL, NULL);
   if(err < 0) {
      perror("Couldn't enqueue the kernel");
      exit(1);   
   }

   /* Read the buffer */
   err = clEnqueueReadBuffer(queue, data_buffer, CL_TRUE, 0, 
      sizeof(data), data, 0, NULL, NULL);
   if(err < 0) {
      perror("Couldn't read the buffer");
      exit(1);
   }

   printf("Increment: %d\n", data[0]);
   printf("Atomic increment: %d\n", data[1]);

   /* Deallocate resources */
   clReleaseMemObject(data_buffer);
   clReleaseKernel(kernel);
   clReleaseCommandQueue(queue);
   clReleaseProgram(program);
   clReleaseContext(context);
   return 0;
}
