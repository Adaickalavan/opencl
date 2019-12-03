# Base image
FROM docker.io/ubuntu AS buildstage

LABEL Author Adaickalavan<adaickalavan@gmail.com>

# Install build tools
# RUN apt-get update -y; apt-get install -y --allow-unauthenticated gpg cmake pkg-config doxygen; \
#     echo "deb http://ppa.launchpad.net/intel-opencl/intel-opencl/ubuntu bionic main" >> /etc/apt/sources.list; \
#     apt-key adv --keyserver keyserver.ubuntu.com --recv-keys B9732172C4830B8F; \
#     apt-get update -y; apt-get install -y --allow-unauthenticated intel-opencl clinfo; \
#     rm -rf /var/lib/apt/lists/*

RUN apt-get update -y && apt-get install -y software-properties-common 
RUN add-apt-repository ppa:intel-opencl/intel-opencl && apt-get update -y
RUN apt-get -y install \
    cmake pkg-config doxygen \
    gpg intel-opencl clinfo

# RUN apt-get update -y && apt-get install -y wget
# RUN mkdir /neo
# RUN cd /neo && wget https://github.com/intel/compute-runtime/releases/download/19.40.14409/intel-gmmlib_19.3.2_amd64.deb \
#     && wget https://github.com/intel/compute-runtime/releases/download/19.40.14409/intel-igc-core_1.0.2597_amd64.deb \
#     && wget https://github.com/intel/compute-runtime/releases/download/19.40.14409/intel-igc-opencl_1.0.2597_amd64.deb \
#     && wget https://github.com/intel/compute-runtime/releases/download/19.40.14409/intel-opencl_19.40.14409_amd64.deb \
#     && wget https://github.com/intel/compute-runtime/releases/download/19.40.14409/intel-ocloc_19.40.14409_amd64.deb
# RUN cd /neo && dpkg -i *.deb

# Change working directory
WORKDIR /src

# Copy the current folder which contains C++ source code to the Docker image
COPY . .

# Use cmake to compile the cpp source file. cmake@v3.10.x .
RUN cmake -E make_directory build && \
    cmake -E chdir ./build cmake -DCMAKE_BUILD_TYPE=Release .. && \
    cmake --build ./build

# # Check if the program has dynamic dependencies
# RUN cd /src/bin && \ 
#     ldd tutorial && \
#     ldd mandel && \
#     ldd linked && \
#     ldd prodCons && \
#     ldd pi_mc

# # Multistage build
# FROM alpine AS runtime

# # Label
# LABEL author Adaickalavan

# # Copy needed source
# COPY --from=buildstage /src/build /src/build/
# COPY --from=buildstage /src/bin /src/bin/

# Command to execute when the image loads
# ENTRYPOINT ["/bin/bash"]