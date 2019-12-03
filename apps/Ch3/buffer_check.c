#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#ifdef MAC
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

/* Find a GPU or CPU associated with the first available platform */
cl_device_id create_device() {

   cl_platform_id platform;
   cl_device_id dev;
   char* ext_data;							
   size_t ext_size;  
   int err;

   /* Identify a platform */
   err = clGetPlatformIDs(1, &platform, NULL);
   if(err < 0) {
      perror("Couldn't identify a platform");
      exit(1);
   } 

   /* Access a device */
   err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &dev, NULL);
   if(err == CL_DEVICE_NOT_FOUND) {
      err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &dev, NULL);
   }
   if(err < 0) {
      perror("Couldn't access any devices");
      exit(1);   
   }

   err = clGetPlatformInfo(platform, CL_PLATFORM_VERSION, 0, NULL, &ext_size);
   if(err < 0) {
      perror("Couldn't read platform version.");
      exit(1);
   }
   ext_data = (char*)malloc(ext_size);
   clGetPlatformInfo(platform, CL_PLATFORM_VERSION, ext_size, ext_data, NULL);
   printf("Platform supports version: %s\n", ext_data);

   return dev;
}

int main() {

   /* Host/device data structures */
   cl_device_id device;
   cl_context context;
   cl_int err;
   char name_data[48];

   /* Data and buffers */
   float main_data[100];
   cl_mem main_buffer, sub_buffer;
   void *main_buffer_mem = NULL, *sub_buffer_mem = NULL;
   size_t main_buffer_size, sub_buffer_size;
   cl_buffer_region region;
   
   /* Create device and context */
   device = create_device();
   /* Access device name */
   err = clGetDeviceInfo(device, CL_DEVICE_NAME, 48 * sizeof(char), name_data, NULL);			
   if(err < 0) {		
      perror("Couldn't read device info");
      exit(1);
   }
   printf("NAME: %s\n", name_data);
   context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
   if(err < 0) {
      perror("Couldn't create a context");
      exit(1);   
   }

   /* Create a buffer to hold 100 floating-point values */
   main_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | 
      CL_MEM_USE_HOST_PTR, sizeof(main_data), main_data, &err);
   if(err < 0) {
      perror("Couldn't create a buffer");
      exit(1);   
   }

   /* Create a sub-buffer */
   /* Modified on 2/12/2014 to account for unaligned memory error */
   region.origin = 0x100;
   region.size = 10*sizeof(float);
   sub_buffer = clCreateSubBuffer(main_buffer, CL_MEM_READ_ONLY, 
         CL_BUFFER_CREATE_TYPE_REGION, &region, &err);
   if(err < 0) {
      perror("Couldn't create a sub-buffer");
      exit(1);   
   }

   /* Obtain size information about the buffers */
   clGetMemObjectInfo(main_buffer, CL_MEM_SIZE, 
         sizeof(main_buffer_size), &main_buffer_size, NULL);
   clGetMemObjectInfo(sub_buffer, CL_MEM_SIZE, 
         sizeof(sub_buffer_size), &sub_buffer_size, NULL);
   printf("Main buffer size: %lu\n", main_buffer_size);
   printf("Sub-buffer size:  %lu\n", sub_buffer_size);
   
   /* Obtain the host pointers */
   clGetMemObjectInfo(main_buffer, CL_MEM_HOST_PTR, 
         sizeof(main_buffer_mem), &main_buffer_mem, NULL);
   err = clGetMemObjectInfo(sub_buffer, CL_MEM_HOST_PTR, 
         sizeof(sub_buffer_mem), &sub_buffer_mem, NULL);
   if(err < 0) {
      printf("clGetMemObjectInfo Error %d\n",err);
   }        
   printf("Main buffer memory address: %p\n", main_buffer_mem);
   printf("Sub-buffer memory address:  %p\n", sub_buffer_mem);

   clGetMemObjectInfo(sub_buffer, CL_MEM_OFFSET, 
         sizeof(sub_buffer_size), &sub_buffer_size, NULL);
   printf("Sub-buffer offset address:  %lu\n", sub_buffer_size);

   /* Print the address of the main data */
   printf("Main array address: %p\n", main_data);

   /* Deallocate resources */
   clReleaseMemObject(main_buffer);
   clReleaseMemObject(sub_buffer);
   clReleaseContext(context);

   return 0;
}
