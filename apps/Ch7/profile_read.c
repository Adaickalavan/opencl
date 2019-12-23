#define _CRT_SECURE_NO_WARNINGS
#define NUM_FILES 1
#define PROGRAM_FILE "./apps/Ch7/profile_read.cl"
#define KERNEL_FUNC "profile_read"

#define NUM_BYTES 131072
#define NUM_ITERATIONS 2000
//#define PROFILE_READ 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
   cl_int i, err, num_vectors;

   /* Data and events */
   char data[NUM_BYTES];
   cl_mem data_buffer;
   cl_event prof_event;
   cl_ulong time_start, time_end, total_time;
   void* mapped_memory;

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

   /* Tell kernel number of char16 vectors */
   num_vectors = NUM_BYTES/16;
   clSetKernelArg(kernel, 1, sizeof(num_vectors), &num_vectors);

   /* Create a command queue */
   queue = clCreateCommandQueue(context, device, 
         CL_QUEUE_PROFILING_ENABLE, &err);
   if(err < 0) {
      perror("Couldn't create a command queue");
      exit(1);   
   };

   total_time = 0.0f;
   for(i=0; i<NUM_ITERATIONS; i++) {

      /* Enqueue kernel */
      err = clEnqueueTask(queue, kernel, 0, NULL, NULL);
      if(err < 0) {
         perror("Couldn't enqueue the kernel");
         exit(1);   
      }

#ifdef PROFILE_READ

      /* Read the buffer */
      err = clEnqueueReadBuffer(queue, data_buffer, CL_TRUE, 0, 
            sizeof(data), data, 0, NULL, &prof_event);
      if(err < 0) {
         perror("Couldn't read the buffer");
         exit(1);
      }

#else

      /* Create memory map */
      mapped_memory = clEnqueueMapBuffer(queue, data_buffer, CL_TRUE,
            CL_MAP_READ, 0, sizeof(data), 0, NULL, &prof_event, &err);
      if(err < 0) {
         perror("Couldn't map the buffer to host memory");
         exit(1);   
      }

#endif

      /* Get profiling information */
      clGetEventProfilingInfo(prof_event, CL_PROFILING_COMMAND_START,
            sizeof(time_start), &time_start, NULL);
      clGetEventProfilingInfo(prof_event, CL_PROFILING_COMMAND_END,
            sizeof(time_end), &time_end, NULL);
      total_time += time_end - time_start;

#ifndef PROFILE_READ

      /* Unmap the buffer */
      err = clEnqueueUnmapMemObject(queue, data_buffer, mapped_memory,
            0, NULL, NULL);
      if(err < 0) {
         perror("Couldn't unmap the buffer");
         exit(1);   
      }

#endif
   }

#ifdef PROFILE_READ
   printf("Average read time: %lu\n", total_time/NUM_ITERATIONS);
#else
   printf("Average map time: %lu\n", total_time/NUM_ITERATIONS);
#endif

   /* Deallocate resources */
   clReleaseEvent(prof_event);
   clReleaseMemObject(data_buffer);
   clReleaseKernel(kernel);
   clReleaseCommandQueue(queue);
   clReleaseProgram(program);
   clReleaseContext(context);
   return 0;
}
