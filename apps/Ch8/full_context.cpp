#define CL_HPP_ENABLE_EXCEPTIONS 
#define CL_HPP_TARGET_OPENCL_VERSION 200

#include <stdio.h>

#ifdef MAC
#include <OpenCL/cl2.hpp>
#else
#include <CL/cl2.hpp>
#endif

int main(void) {

   try {
      // Retrieve all platforms
      std::vector<cl::Platform> platforms;
      cl::Platform::get(&platforms);

      // Retrieve all devices in all platforms
      cl::string platformParam;
      cl::string deviceName;
      std::vector<std::vector<cl::Device>> devices(platforms.size());
      for(int i=0; i<platforms.size(); i++) {
         platforms[i].getInfo(CL_PLATFORM_VERSION, &platformParam);
         printf("Platform %d, Version: %s\n", i, platformParam.c_str());

         platforms[i].getDevices(CL_DEVICE_TYPE_ALL, &devices[i]);
         for(int j=0; j<devices[i].size(); j++) {
            deviceName = devices[i][j].getInfo<CL_DEVICE_NAME>();
            printf("Platform %d, Device %d: %s \n", i, j, deviceName.c_str());
         }
      }

      // Create context with devices from first platform
      cl::Context context(devices[0]);

   }
   catch(cl::Error e) {
      printf("%s : Error code %d\n", e.what(), e.err());
   }

   return 0;
}

