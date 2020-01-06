#define CL_HPP_ENABLE_EXCEPTIONS 
#define CL_HPP_TARGET_OPENCL_VERSION 200

#include <fstream>
#include <iostream>
#include <iterator>

#ifdef MAC
   #include <OpenCL/cl2.hpp>
#else
   #include <CL/cl2.hpp>
#endif

// #define CL_HPP_NO_STD_VECTOR

int main(void) {
   
   cl::vector<cl::Platform> platforms;
   cl::vector<cl::Device> devices;
   std::string kernelName;

   try {
      // Place the GPU devices of the first platform into a context
      cl::Platform::get(&platforms);
      platforms[0].getDevices(CL_DEVICE_TYPE_CPU, &devices);
      cl::Context context(devices);
      
      // Create and build program
      std::ifstream programFile("./apps/Ch8/create_kernels.cl");
      std::string programString(std::istreambuf_iterator<char>(programFile),
            (std::istreambuf_iterator<char>()));

      // Old method
      // cl::Program::Sources source(1, std::make_pair(programString.c_str(), programString.length()+1));

      // New simpler string interface style
      cl::Program::Sources source;
      source.push_back(programString.c_str());      

      // Build the program
      cl::Program program(context, source);
      const char* options = "-cl-std=CL2.0";
      try {
         program.build(devices, options = options);
      }
      catch (...) {
        // Print build info for all devices
        cl_int buildErr;
        cl::BuildLogType buildInfo = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(&buildErr);
        for (auto &pair : buildInfo) {
            fprintf(stderr, "%s %s", pair.first.getInfo<CL_DEVICE_NAME>().c_str(), pair.second.c_str());
        }
        return 1;
      }

      // Create individual kernels
      cl::Kernel addKernel(program, "add");
      cl::Kernel subKernel(program, "subtract");
      cl::Kernel multKernel(program, "multiply");

      // Create all kernels in program
      cl::vector<cl::Kernel> allKernels;
      program.createKernels(&allKernels);
      for(unsigned int i=0; i<allKernels.size(); i++) {
         kernelName = allKernels[i].getInfo<CL_KERNEL_FUNCTION_NAME>();
         std::cout << "Kernel: " << kernelName << std::endl;
      }
   }
   catch(cl::Error e) {
      std::cout << e.what() << ": Error code " << e.err() << std::endl;   
   }

   return 0;
}
