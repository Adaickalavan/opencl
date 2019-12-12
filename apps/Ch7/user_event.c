#define _CRT_SECURE_NO_WARNINGS
#define NUM_FILES 1
#define PROGRAM_FILE "./apps/Ch7/user_event.cl"
#define KERNEL_FUNC "user_event"

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

void CL_CALLBACK read_complete(cl_event e, cl_int status, void* data) {

   float *float_data = (float*)data;
   printf("New data: %4.2f, %4.2f, %4.2f, %4.2f\n", 
      float_data[0], float_data[1], float_data[2], float_data[3]);
}

int main() {

   /* OpenCL data structures */
   cl_device_id device;
   cl_context context;
   cl_command_queue queue;
   cl_program program;
   cl_kernel kernel;
   cl_int i, err;

   /* Data and events */
   float data[4];
   cl_mem data_buffer;
   cl_event user_event, kernel_event, read_event;
   
   /* Initialize data */
   for(i=0; i<4; i++)
      data[i] = i * 1.0;

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

   /* Create a command queue */
   queue = clCreateCommandQueueWithProperties(context, device, 
         CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, &err);
   if(err < 0) {
      perror("Couldn't create a command queue");
      exit(1);   
   };

   /* Configure events */
   user_event = clCreateUserEvent(context, &err);
   if(err < 0) {
      perror("Couldn't enqueue the kernel");
      exit(1);   
   }

   /* Enqueue kernel */
   size_t dim = 1;
   size_t global_size = 1;
   size_t* global_offset = NULL;
   size_t* local_size = NULL;
   err = clEnqueueNDRangeKernel(queue, kernel, dim, global_offset,
         &global_size, local_size, 1, &user_event, &kernel_event);      
   // err = clEnqueueTask(queue, kernel, 1, &user_event, &kernel_event);
   if(err < 0) {
      perror("Couldn't enqueue the kernel");
      exit(1);   
   }

   /* Read the buffer */
   err = clEnqueueReadBuffer(queue, data_buffer, CL_TRUE, 0, 
      sizeof(data), data, 1, &kernel_event, &read_event);
   if(err < 0) {
      perror("Couldn't read the buffer");
      exit(1);
   }

   /* Set callback for event */
   err = clSetEventCallback(read_event, CL_COMPLETE, 
         &read_complete, data);
   if(err < 0) {
      perror("Couldn't set callback for event");
      exit(1);   
   }

   /* Sleep for a second to demonstrate the that commands haven't
      started executing. Then prompt user */
   sleep(1);
   printf("Old data: %4.2f, %4.2f, %4.2f, %4.2f\n", 
      data[0], data[1], data[2], data[3]);
   printf("Press ENTER to continue.\n");
   getchar();

   /* Set user event to success */
   clSetUserEventStatus(user_event, CL_SUCCESS);
   
   clFinish(queue);

   /* Deallocate resources */
   clReleaseEvent(read_event);
   clReleaseEvent(kernel_event);
   clReleaseEvent(user_event);
   clReleaseMemObject(data_buffer);
   clReleaseKernel(kernel);
   clReleaseCommandQueue(queue);
   clReleaseProgram(program);
   clReleaseContext(context);
   return 0;
}
