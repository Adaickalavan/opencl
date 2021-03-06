#define CL_HPP_ENABLE_EXCEPTIONS 
#define CL_HPP_TARGET_OPENCL_VERSION 200
// #define __NO_STD_VECTOR
#define PROGRAM_FILE "./apps/Ch8/buffer_test.cl"
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

int main(void) {
   
   cl::vector<cl::Platform> platforms;
   cl::vector<cl::Device> devices;

   // Data and rectangle geometry
   float fullMatrix[80], zeroMatrix[80];
   // cl::size_t<3> bufferOrigin, hostOrigin, region;
   cl::array<cl::size_type, 3> bufferOrigin;
   cl::array<cl::size_type, 3> hostOrigin;
   cl::array<cl::size_type, 3> region;

   try {
      // Initialize data
      for(int i=0; i<80; i++) {
         fullMatrix[i] = i*1.0f;
         zeroMatrix[i] = 0.0f;
      }

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

      // Create matrix buffer and make it the kernel's first argument
      cl::Buffer matrixBuffer(context, 
         CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, 
         sizeof(zeroMatrix), zeroMatrix); 
      kernel.setArg(0, matrixBuffer);

      // Create queue and enqueue kernel-execution command
      cl::CommandQueue queue(context, devices[0]);
      // queue.enqueueTask(kernel);
      cl::NDRange global_offset(0);
      cl::NDRange global_size(1);
      cl::NDRange local_size = cl::NullRange;
      queue.enqueueNDRangeKernel(kernel, global_offset, 
         global_size, local_size);

      // Update the content of the buffer
      queue.enqueueWriteBuffer(matrixBuffer, CL_TRUE, 0, 
            sizeof(fullMatrix), fullMatrix);

      // Read a rectangle of data from the buffer
      // bufferOrigin.push_back(5*sizeof(float)); 
      // bufferOrigin.push_back(3);
      // bufferOrigin.push_back(0);
      bufferOrigin = {5*sizeof(float),3,0};
      // hostOrigin.push_back(1*sizeof(float)); 
      // hostOrigin.push_back(1);
      // hostOrigin.push_back(0);
      hostOrigin = {1*sizeof(float),1,0};
      // region.push_back(4*sizeof(float)); 
      // region.push_back(4);
      // region.push_back(1);
      region = {4*sizeof(float),4,1};
      queue.enqueueReadBufferRect(matrixBuffer, CL_TRUE, 
         bufferOrigin, hostOrigin, region, 
         10*sizeof(float), 0, 
         10*sizeof(float), 0, zeroMatrix);

      /* Display updated buffer */
      for(int i=0; i<8; i++) {
         for(int j=0; j<10; j++) {
            printf("%6.1f", zeroMatrix[j+i*10]);
         }
         printf("\n");
      }
   }
   catch(cl::Error e) {
      std::cout << e.what() << ": Error code " << e.err() << std::endl;   
   }

   return 0;
}
