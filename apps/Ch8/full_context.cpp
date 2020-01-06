#define CL_HPP_ENABLE_EXCEPTIONS 
#define CL_HPP_TARGET_OPENCL_VERSION 200

#include <stdio.h>
#include <exception>

#ifdef MAC
   #include <OpenCL/cl2.hpp>
#else
   #include <CL/cl2.hpp>
#endif

class myexception : public std::exception {
   public:
      virtual const char* what() const throw()
      {
         return "No OpenCL 2.0 platform found.";
      }
} myex;

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

      // Set platform with OpenCL2, as the default platform.
      cl::Platform plat;
      for (auto &p : platforms) {
        std::string platver = p.getInfo<CL_PLATFORM_VERSION>();
        if (platver.find("OpenCL 2.") != std::string::npos) {
            plat = p;
        }
      }
      if (plat() != 0) {
         throw myex;
      }

      // Create a context with devices from the first platform
      cl::Context context(devices[0]);

   }
   catch(cl::Error e) {
      printf("%s : Error code %d\n", e.what(), e.err());
   }
   catch(std::exception& e) {
     printf("%s \n", e.what());
   }

   return 0;
}

