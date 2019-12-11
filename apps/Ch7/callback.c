#define _CRT_SECURE_NO_WARNINGS
#define NUM_FILES 1
#define PROGRAM_FILE "./apps/Ch7/callback.cl"
#define KERNEL_FUNC "callback"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

#ifdef MAC
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

void CL_CALLBACK kernel_complete(cl_event e, cl_int status, void* data) {
   printf("%s", (char*)data);
}

void CL_CALLBACK read_complete(cl_event e, cl_int status, void* data) {

   int i;
   cl_bool check;
   float *buffer_data;

   buffer_data = (float*)data;
   check = CL_TRUE;
   for(i=0; i<4096; i++) {
      if(buffer_data[i] != 5.0) {
         check = CL_FALSE;
         break;
      }  
   }
   if(check)
      printf("The data has been initialized successfully.\n");
   else
      printf("The data has not been initialized successfully.\n");
}

int main() {

   /* OpenCL data structures */
   cl_device_id device;
   cl_context context;
   cl_command_queue queue;
   cl_program program;
   const char* fileNames[] = {PROGRAM_FILE};
   const char options[] = "";     
   cl_kernel kernel;
   cl_int err;

   /* Data and events */
   char *kernel_msg;
   float data[4096];
   cl_mem data_buffer;
   cl_event kernel_event, read_event;   
   
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
      perror("Couldn't create a kernel");
      exit(1);   
   };

   /* Create a write-only buffer to hold the output data */
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
   queue = clCreateCommandQueueWithProperties(context, device, 0, &err);
   if(err < 0) {
      perror("Couldn't create a command queue");
      exit(1);   
   };

   /* Enqueue kernel */
   size_t dim = 1;
   size_t global_size[1] = {1};
   size_t* global_offset = NULL;
   size_t* local_size = NULL;
   err = clEnqueueNDRangeKernel(queue, kernel, dim, global_offset,
         global_size, local_size, 0, NULL, &kernel_event);      
   // err = clEnqueueTask(queue, kernel, 0, NULL, &kernel_event);
   if(err < 0) {
      perror("Couldn't enqueue the kernel");
      exit(1);   
   }

   /* Read the buffer */
   err = clEnqueueReadBuffer(queue, data_buffer, CL_FALSE, 0, 
      sizeof(data), &data, 0, NULL, &read_event);
   if(err < 0) {
      perror("Couldn't read the buffer");
      exit(1);   
   }
 
   /* Set event handling routines */
   kernel_msg = "The kernel finished successfully.\n\0";
   err = clSetEventCallback(kernel_event, CL_COMPLETE, 
         &kernel_complete, kernel_msg);
   if(err < 0) {
      perror("Couldn't set callback for event");
      exit(1);   
   }
   clSetEventCallback(read_event, CL_COMPLETE, &read_complete, data);

   /* Deallocate resources */
   clReleaseMemObject(data_buffer);
   clReleaseKernel(kernel);
   clReleaseCommandQueue(queue);
   clReleaseProgram(program);
   clReleaseContext(context);
   return 0;
}
