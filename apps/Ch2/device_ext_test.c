#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef MAC
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

int main() {

   /* Host/device data structures */
   cl_platform_id platform;
   cl_device_id dev;
   cl_uint addr_data;
   cl_int err;

   /* Extension data */
   char name_data[48], ext_data[4096];

   /* Identify a platform */
   err = clGetPlatformIDs(1, &platform, NULL);			
   if(err < 0) {			
      perror("Couldn't find any platforms");
      exit(1);
   }

   /* Access a device, preferably a GPU */
   /* Changed on 2/12 to fix the CL_INVALID_VALUE error */
   err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &dev, NULL);
   if(err == CL_DEVICE_NOT_FOUND) {
      err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &dev, NULL);
   }
   if(err < 0) {
      perror("Couldn't access any devices");
      exit(1);   
   }

   /* Access device name */
   err = clGetDeviceInfo(dev, CL_DEVICE_NAME, 		
      48 * sizeof(char), name_data, NULL);			
   if(err < 0) {		
      perror("Couldn't read extension data");
      exit(1);
   }

   /* Access device address size */
   clGetDeviceInfo(dev, CL_DEVICE_ADDRESS_BITS, 	
      sizeof(addr_data), &addr_data, NULL);			

   /* Access device extensions */
   clGetDeviceInfo(dev, CL_DEVICE_EXTENSIONS, 		
      4096 * sizeof(char), ext_data, NULL);			

   printf("NAME: %s\nADDRESS_WIDTH: %u\nEXTENSIONS: %s\n", 
      name_data, addr_data, ext_data);

   return 0;
}
