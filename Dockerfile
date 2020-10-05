# Prepare an Ubuntu image to build the servers.
FROM ubuntu:focal AS builder

# We don't want to have to interact when installing dependencies
ARG DEBIAN_FRONTEND=noninteractive

# Install the dependencies
RUN apt-get update
RUN apt-get install --yes   \
    build-essential         \
    cmake                   \
    libgoogle-glog-dev      \
    libboost-all-dev        \
    libcrypto++-dev         \
    protobuf-compiler       \
    protobuf-compiler-grpc  \
    libgrpc++-dev           \
    libpqxx-dev             \
    libtbb-dev

# Copy the source files
COPY . /edensrc/

# Build the servers
RUN mkdir build
WORKDIR /edensrc/build
RUN cmake .. -DCMAKE_BUILD_TYPE=Release
RUN make

# Start with a clean image
FROM ubuntu:focal
ENV GLOG_logtostderr=1

# Install the runtime dependencies
RUN apt-get update
RUN apt-get install --yes   \
    openssl                 \
    libcrypto++6            \
    libgoogle-glog0v5       \
    libpqxx-6.4             \
    libprotobuf17           \
    libgrpc++1              \
    libtbb2
    
# Create a new working directory
WORKDIR /eden/

# Copy the run script
COPY --from=builder /edensrc/run.sh .

# Copy the data directory
COPY --from=builder /edensrc/data ./data

# Copy the game and login server executable
COPY --from=builder /edensrc/build/eden/gameserver/gameserver   .
COPY --from=builder /edensrc/build/eden/loginserver/loginserver .

# Run the servers
ENTRYPOINT ["./run.sh"]