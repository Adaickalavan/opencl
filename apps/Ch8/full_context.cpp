// #define _CRT_SECURE_NO_WARNINGS
#define CL_HPP_TARGET_OPENCL_VERSION 200
// #define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_ENABLE_EXCEPTIONS 
// #define __NO_STD_STRING

#include <iostream>

#ifdef MAC
#include <OpenCL/cl2.hpp>
#else
#include <CL/cl2.hpp>
#endif

int main(void) {
   
   std::vector<cl::Platform> platforms;
   std::vector<cl::Device> platformDevices, ctxDevices;
   cl::string device_name;
   cl_uint i;
   try {
      // Access all devices in first platform
      cl::Platform::get(&platforms);
      platforms[0].getDevices(CL_DEVICE_TYPE_ALL, &platformDevices);
      
      // Create context and access device names
      cl::Context context(platformDevices);
      ctxDevices = context.getInfo<CL_CONTEXT_DEVICES>();
      for(i=0; i<ctxDevices.size(); i++) {
         device_name = ctxDevices[i].getInfo<CL_DEVICE_NAME>();
         std::cout << "Device: " << device_name.c_str() << std::endl;
      }
   }
   catch(cl::Error e) {
      std::cout << e.what() << ": Error code " << e.err() << std::endl;
   }

   return 0;
}
