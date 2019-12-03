#define _CRT_SECURE_NO_WARNINGS
#define NUM_FILES 1
#define PROGRAM_FILE "./apps/Ch1/matvec.cl"
#define KERNEL_FUNC "matvec_mult"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include "util.h"

#ifdef MAC
#include <OpenCL/cl.h>
#else  
#include <CL/cl.h>
#endif

int main() {

   /* Host/device data structures */
   cl_platform_id platform;
   cl_device_id device;
   cl_context context;
   cl_command_queue queue;
   cl_int i, err;

   /* Program/kernel data structures */
   cl_program program;
   // char *program_buffer[NUM_FILES];
   const char* fileNames[] = {PROGRAM_FILE};
   const char options[] = "-cl-finite-math-only -cl-no-signed-zeros";  
   cl_kernel kernel;
   
   /* Data and buffers */
   float mat[16], vec[4], result[4];
   float correct[4] = {0.0f, 0.0f, 0.0f, 0.0f};
   cl_mem mat_buff, vec_buff, res_buff;
   size_t work_units_per_kernel;

   /* Initialize data to be processed by the kernel */
   for(i=0; i<16; i++) {
      mat[i] = i * 2.0f;
   } 
   for(i=0; i<4; i++) {
      vec[i] = i * 3.0f;
      correct[0] += mat[i]    * vec[i];
      correct[1] += mat[i+4]  * vec[i];
      correct[2] += mat[i+8]  * vec[i];
      correct[3] += mat[i+12] * vec[i];      
   }

   // Find a GPU or CPU associated with the first available platform
   device = create_device();
 
   /* Create the context */
   context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
   if(err < 0) {
      perror("Couldn't create a context");
      exit(1);   
   }

   // Create program from .cl file and compile it
   program = createProgramFromFile(NUM_FILES, fileNames, context, device, options);

   /* Create kernel for the mat_vec_mult function */
   kernel = clCreateKernel(program, KERNEL_FUNC, &err);
   if(err < 0) {
      perror("Couldn't create the kernel");
      exit(1);   
   }

   /* Create CL buffers to hold input and output data */
   mat_buff = clCreateBuffer(context, CL_MEM_READ_ONLY | 
      CL_MEM_COPY_HOST_PTR, sizeof(float)*16, mat, &err);
   if(err < 0) {
      perror("Couldn't create a buffer object");
      exit(1);   
   }      
   vec_buff = clCreateBuffer(context, CL_MEM_READ_ONLY | 
      CL_MEM_COPY_HOST_PTR, sizeof(float)*4, vec, NULL);
   res_buff = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
      sizeof(float)*4, NULL, NULL);

   /* Create kernel arguments from the CL buffers */
   err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &mat_buff);
   if(err < 0) {
      perror("Couldn't set the kernel argument");
      exit(1);   
   }         
   clSetKernelArg(kernel, 1, sizeof(cl_mem), &vec_buff);
   clSetKernelArg(kernel, 2, sizeof(cl_mem), &res_buff);

   /* Create a CL command queue for the device*/
   queue = clCreateCommandQueueWithProperties(context, device, 0, &err);
   if(err < 0) {
      perror("Couldn't create the command queue");
      exit(1);   
   }

   /* Enqueue the command queue to the device */
   work_units_per_kernel = 4; /* 4 work-units per kernel */ 
   err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &work_units_per_kernel, 
      NULL, 0, NULL, NULL);
   if(err < 0) {
      perror("Couldn't enqueue the kernel execution command");
      exit(1);   
   }

   /* Read the result */
   err = clEnqueueReadBuffer(queue, res_buff, CL_TRUE, 0, sizeof(float)*4, 
      result, 0, NULL, NULL);
   if(err < 0) {
      perror("Couldn't enqueue the read buffer command");
      exit(1);   
   }

   /* Test the result */
   if((result[0] == correct[0]) && (result[1] == correct[1])
      && (result[2] == correct[2]) && (result[3] == correct[3])) {
      printf("Matrix-vector multiplication successful.\n");
   }
   else {
      printf("result[0] = %f\n",result[0]);
      printf("result[1] = %f\n",result[1]);
      printf("result[2] = %f\n",result[2]);
      printf("result[3] = %f\n",result[3]);
      printf("Matrix-vector multiplication unsuccessful.\n");
   }

   /* Deallocate resources */
   clReleaseMemObject(mat_buff);
   clReleaseMemObject(vec_buff);
   clReleaseMemObject(res_buff);
   clReleaseKernel(kernel);
   clReleaseCommandQueue(queue);
   clReleaseProgram(program);
   clReleaseContext(context);

   return 0;
}

