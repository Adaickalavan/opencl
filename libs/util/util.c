#include <stdio.h>
#include <stdlib.h>

#ifdef MAC
#include <OpenCL/cl.h>
#else  
#include <CL/cl.h>
#endif

// Find a GPU or CPU associated with the first available platform
cl_device_id create_device(){
   cl_platform_id platform;
   cl_device_id device;
   char* platform_data;							
   size_t platform_size; 
   char device_data[48];
   int err;

   // Identify a platform
   err = clGetPlatformIDs(1, &platform, NULL);
   if(err < 0) {
      perror("Couldn't find any platforms");
      exit(1);
   }

   // Access a device, preferably a GPU
   err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
   if(err == CL_DEVICE_NOT_FOUND) {
      err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &device, NULL);
   }
   if(err < 0) {
      perror("Couldn't find any devices");
      exit(1);   
   }

   // Access platform version
   err = clGetPlatformInfo(platform, CL_PLATFORM_VERSION, 0, NULL, &platform_size);
   if(err < 0) {
      perror("Couldn't read platform version");
      exit(1);
   }
   platform_data = (char*)malloc(platform_size);
   clGetPlatformInfo(platform, CL_PLATFORM_VERSION, platform_size, platform_data, NULL);
   printf("Platform version: %s\n", platform_data);

   // Access device name
   err = clGetDeviceInfo(device, CL_DEVICE_NAME, 48 * sizeof(char), device_data, NULL);			
   if(err < 0) {		
      perror("Couldn't read device info");
      exit(1);
   }
   printf("Device name: %s\n", device_data);

   return device;
}

/* Create program from a file and compile it */
cl_program createProgramFromFile(int numFiles, const char* fileNames[numFiles], cl_context context, cl_device_id device, const char* options)
{
   FILE* program_handle;
   char* program_buffer[numFiles]; 
   char* program_log;
   cl_int err;
   size_t program_size[numFiles];
   size_t log_size;

   /* Read each program file and place content into buffer array */
   for (int ii = 0; ii < numFiles; ii++)
   {
      // Determine size of source file
      program_handle = fopen(fileNames[ii], "r");
      if (program_handle == NULL)
      {
         perror("Couldn't find the program file");
         exit(1);
      }
      fseek(program_handle, 0, SEEK_END);
      program_size[ii] = ftell(program_handle);
      rewind(program_handle);

      // Read file content into buffer
      program_buffer[ii] = (char *)malloc(program_size[ii] + 1);
      program_buffer[ii][program_size[ii]] = '\0';
      fread(program_buffer[ii], sizeof(char), program_size[ii], program_handle);
      fclose(program_handle);
   }

   /* Create a program containing all program content */
   cl_program program = clCreateProgramWithSource(context, numFiles, 			
         (const char**)program_buffer, program_size, &err);				
   if(err < 0) {
      perror("Couldn't create the program");
      exit(1);   
   }

   /* Deallocate resources */
   for(int ii=0; ii < numFiles; ii++) {
      free(program_buffer[ii]);
   }

   /* Build program */
   err = clBuildProgram(program, 0, NULL, options, NULL, NULL);
   if(err < 0) {
      /* Find size of log and print to std output */
      clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 
            0, NULL, &log_size);
      program_log = (char*) malloc(log_size + 1);
      program_log[log_size] = '\0';
      clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 
            log_size + 1, program_log, NULL);
      printf("%s\n", program_log);
      free(program_log);
      exit(1);
   }

   return program;
}
