#define _CRT_SECURE_NO_WARNINGS
#define NUM_FILES 1
#define PROGRAM_FILE "./apps/Ch7/profile_items.cl"
#define KERNEL_FUNC "profile_items"

#define NUM_INTS 4096
#define NUM_ITEMS 512
#define NUM_ITERATIONS 2000

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
   size_t num_items;
   cl_int i, err, num_ints;

   /* Data and events */
   int data[NUM_INTS];
   cl_mem data_buffer;
   cl_event prof_event;
   cl_ulong time_start, time_end, total_time;

   /* Initialize data */
   for(i=0; i<NUM_INTS; i++) {
      data[i] = i;
   }

   /* Set number of data points and work-items */
   num_ints = NUM_INTS;
   num_items = NUM_ITEMS;

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
   data_buffer = clCreateBuffer(context, 
         CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, 
         sizeof(data), data, &err);
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
   clSetKernelArg(kernel, 1, sizeof(num_ints), &num_ints);

   /* Create a command queue */
   queue = clCreateCommandQueue(context, device, 
         CL_QUEUE_PROFILING_ENABLE, &err);
   if(err < 0) {
      perror("Couldn't create a command queue");
      exit(1);   
   };

   total_time = 0.0f;
   size_t dim = 1;
   size_t global_size = num_items;
   size_t* global_offset = NULL;
   // size_t local_size[] = {16};
   size_t* local_size = NULL;
   for(i=0; i<NUM_ITERATIONS; i++) {
         
      /* Enqueue kernel */
      // clEnqueueNDRangeKernel(queue, kernel, 1, NULL, 
      //    &num_items, NULL, 0, NULL, &prof_event);
      err = clEnqueueNDRangeKernel(queue, kernel, dim, global_offset,
         &global_size, local_size, 0, NULL, &prof_event);        
      if(err < 0) {
         perror("Couldn't enqueue the kernel");
         exit(1);   
      }

      /* Finish processing the queue and get profiling information */
      clFinish(queue);
      clGetEventProfilingInfo(prof_event, CL_PROFILING_COMMAND_START,
            sizeof(time_start), &time_start, NULL);
      clGetEventProfilingInfo(prof_event, CL_PROFILING_COMMAND_END,
            sizeof(time_end), &time_end, NULL);
      total_time += time_end - time_start;
   }
   printf("Average time = %lu ns\n", total_time/NUM_ITERATIONS);

   /* Create memory map */
   void* mapped_memory = clEnqueueMapBuffer(queue, data_buffer, CL_TRUE,
         CL_MAP_READ, 0, sizeof(data), 0, NULL, NULL, &err);
   if(err < 0) {
      perror("Couldn't map the buffer to host memory");
      exit(1);   
   }
   // Print contents of data
   for(int j=0; j<2; j++){
      for(int i=0; i<7; i++){
         printf("%d ", ((int*)mapped_memory)[j*8 + i]);
      }
      printf("%d\n", ((int*)mapped_memory)[j*8 + 7]);
   }
   /* Unmap the buffer */
   err = clEnqueueUnmapMemObject(queue, data_buffer, mapped_memory,
         0, NULL, NULL);
   if(err < 0) {
      perror("Couldn't unmap the buffer");
      exit(1);   
   }

   /* Deallocate resources */
   clReleaseEvent(prof_event);
   clReleaseKernel(kernel);
   clReleaseMemObject(data_buffer);
   clReleaseCommandQueue(queue);
   clReleaseProgram(program);
   clReleaseContext(context);
   return 0;
}
