#define CL_HPP_ENABLE_EXCEPTIONS 
#define CL_HPP_TARGET_OPENCL_VERSION 200
// #define CL_HPP_NO_STD_VECTOR
#define PROGRAM_FILE "./apps/Ch8/user_event.cl"
#define KERNEL_FUNC "blank"

#include <cstdio>
#include <fstream>
#include <iostream>
#include <iterator>

#ifdef MAC
#include <OpenCL/cl2.hpp>
#else
#include <CL/cl2.hpp>
#endif

void CL_CALLBACK printMessage(cl_event event, cl_int status, void* data) {
   std::cout << "The kernel has executed." << std::endl;
}

int main(void) {
   
   cl::vector<cl::Platform> platforms;
   cl::vector<cl::Device> devices;
   cl::Event callbackEvent;
   int data[10];

   try {
      // Place the GPU devices of the first platform into a context
      cl::Platform::get(&platforms);
      platforms[0].getDevices(CL_DEVICE_TYPE_CPU, &devices);
      cl::Context context(devices);
      
      // Create kernel
      std::ifstream programFile(PROGRAM_FILE);
      std::string programString(std::istreambuf_iterator<char>(programFile),
            (std::istreambuf_iterator<char>()));
      // cl::Program::Sources source(1, std::make_pair(programString.c_str(),
      //       programString.length()+1));
      cl::Program::Sources source;
      source.push_back(programString.c_str());                   
      cl::Program program(context, source);
      program.build(devices);
      cl::Kernel kernel(program, KERNEL_FUNC);

      // Create buffer and make it a kernel argument
      cl::Buffer buffer(context, 
            CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(data), data);
      kernel.setArg(0, buffer);

      // Create queue and enqueue kernel-execution command
      cl::UserEvent userEvent(context);
      cl::vector<cl::Event> waitList;
      waitList.push_back((cl::Event)userEvent);
      cl::CommandQueue queue(context, devices[0]);
      // queue.enqueueTask(kernel, &waitList, &callbackEvent);
      cl::NDRange global_offset(0);
      cl::NDRange global_size(1);
      cl::NDRange local_size = cl::NullRange;
      queue.enqueueNDRangeKernel(kernel, global_offset, 
         global_size, local_size, &waitList, &callbackEvent);

      // Configure event processing
      callbackEvent.setCallback(CL_COMPLETE, &printMessage);
      std::cout << "Press ENTER to execute kernel." << std::endl;
      getchar();
      userEvent.setStatus(CL_COMPLETE);
   }
   catch(cl::Error e) {
      std::cout << e.what() << ": Error code " << e.err() << std::endl;   
   }

   return 0;
}
