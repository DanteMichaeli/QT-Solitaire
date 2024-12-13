# Use the Linux GCC Qt image
FROM stateoftheartio/qt6:6.6-gcc-aqt

# Use root for apt-get commands and script installation
USER root

# Fix permissions issue for apt-get and install dependencies
RUN rm -rf /var/lib/apt/lists/* \
    && apt-get clean \
    && apt-get update \
    && apt-get install -y \
        git \
        cmake \
        build-essential \
        libgl1-mesa-dev \
        libx11-dev \
        libxext-dev \
        libxrandr-dev \
        libxi-dev \
    && apt-get clean

# Install Catch2 (if not already in libs folder)
RUN [ ! -d libs/Catch2 ] && git clone --depth 1 https://github.com/catchorg/Catch2.git libs/Catch2 \
    && cmake -S libs/Catch2 -B libs/Catch2/build \
    && cmake --build libs/Catch2/build --target install

# Set environment variables for GUI rendering
ENV DISPLAY=${DISPLAY:-:0}
VOLUME ["/tmp/.X11-unix"]

# Set the working directory
WORKDIR /qt-soliatire

# Copy the entire project into the container
COPY . .

# Install Miniaudio (if not already in libs folder)
RUN [ ! -d libs/miniaudio ] || rm -rf libs/miniaudio && git clone --depth 1 https://github.com/mackron/miniaudio.git libs/miniaudio

# Build the project
RUN cmake -S . -B build && cmake --build build

# Switch back to the default user provided by the base image
USER user

# Default command to run the Solitaire application
CMD ["./build/solitaire"]