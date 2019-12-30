#define _CRT_SECURE_NO_WARNINGS
#define NUM_FILES 1
#define PROGRAM_FILE "./apps/Ch7/mutex.cl"
#define KERNEL_FUNC "mutex"

#include <stdio.h>
#include <stdlib.h>
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

   /* Data and events */
   int mutex = 0;
   int sum = 0;

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
   cl_mem mutex_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, 
      sizeof(mutex), &mutex, &err);
   if(err < 0) {
      perror("Couldn't create a buffer");
      exit(1);   
   };         
   cl_mem sum_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, 
      sizeof(sum), &sum, &err);

   /* Create kernel argument */
   err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &mutex_buffer);
   err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &sum_buffer);
   if(err < 0) {
      perror("Couldn't set a kernel argument");
      exit(1);   
   };

   /* Create a command queue */
   queue = clCreateCommandQueueWithProperties(context, device, 0, &err);
   if(err < 0) {
      perror("Couldn't create a command queue");
      exit(1);   
   };

   /* Enqueue kernel */
   size_t dim = 1;
   size_t global_size = 32;
   size_t* global_offset = NULL;
   size_t local_size = 32;
   err = clEnqueueNDRangeKernel(queue, kernel, dim, global_offset,
         &global_size, &local_size, 0, NULL, NULL);   
   if(err < 0) {
      perror("Couldn't enqueue the kernel");
      exit(1);   
   }

   /* Read the buffer */
   err = clEnqueueReadBuffer(queue, mutex_buffer, CL_TRUE, 0, 
      sizeof(mutex), &mutex, 0, NULL, NULL);
   err |= clEnqueueReadBuffer(queue, sum_buffer, CL_TRUE, 0, 
      sizeof(sum), &sum, 0, NULL, NULL);
   if(err < 0) {
      perror("Couldn't read the buffer");
      exit(1);
   }

   printf("Mutex: %d\n", mutex);
   printf("Sum: %d\n", sum);

   /* Deallocate resources */
   clReleaseMemObject(mutex_buffer);
   clReleaseMemObject(sum_buffer);
   clReleaseKernel(kernel);
   clReleaseCommandQueue(queue);
   clReleaseProgram(program);
   clReleaseContext(context);
   return 0;
}
