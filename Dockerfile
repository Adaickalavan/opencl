# FROM docker.io/ubuntu AS buildstage

# LABEL Author Adaickalavan<adaickalavan@gmail.com>

# Install build tools
# RUN apt-get update -y && apt-get install -y software-properties-common 
# RUN add-apt-repository ppa:intel-opencl/intel-opencl && apt-get update -y
# RUN apt-get -y install \
#     cmake pkg-config doxygen \
#     gpg intel-opencl clinfo

# RUN apt-get update -y && apt-get install -y wget
# RUN mkdir /neo
# RUN cd /neo && wget https://github.com/intel/compute-runtime/releases/download/19.40.14409/intel-gmmlib_19.3.2_amd64.deb \
#     && wget https://github.com/intel/compute-runtime/releases/download/19.40.14409/intel-igc-core_1.0.2597_amd64.deb \
#     && wget https://github.com/intel/compute-runtime/releases/download/19.40.14409/intel-igc-opencl_1.0.2597_amd64.deb \
#     && wget https://github.com/intel/compute-runtime/releases/download/19.40.14409/intel-opencl_19.40.14409_amd64.deb \
#     && wget https://github.com/intel/compute-runtime/releases/download/19.40.14409/intel-ocloc_19.40.14409_amd64.deb
# RUN cd /neo && dpkg -i *.deb

# Change working directory
# WORKDIR /src

# Copy the current folder which contains C++ source code to the Docker image
# COPY . .

# Use cmake to compile the cpp source file. cmake@v3.10.x .
# RUN cmake -E make_directory build && \
#     cmake -E chdir ./build cmake -DCMAKE_BUILD_TYPE=Release .. && \
#     cmake --build ./build