#ifndef UTIL_H
#define UTIL_H

#ifdef MAC
#include <OpenCL/cl.h>
#else  
#include <CL/cl.h>
#endif

cl_program createProgramFromFile(int numFiles, const char* fileNames[numFiles], cl_context context, cl_device_id device, const char* options);
cl_device_id create_device();
size_t getDeviceTimerResolution(cl_device_id device);

#endif