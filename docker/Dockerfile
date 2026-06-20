FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# ------------------------------------------------------------------------------
# Base packages
# ------------------------------------------------------------------------------
RUN apt-get update && apt-get install -y \
    sudo \
    build-essential \
    cmake \
    git \
    wget \
    curl \
    unzip \
    openjdk-11-jdk \
    software-properties-common \
    pkg-config \
    libssl-dev \
    libasio-dev \
    libtinyxml2-dev \
    protobuf-compiler \
    libprotobuf-dev \
    && rm -rf /var/lib/apt/lists/*

# ------------------------------------------------------------------------------
# Fast CDR v1.0.24
# 
# Note: Fast CDR is a dependency of Fast DDS, so it must be installed before Fast DDS.
# Repository: https://github.com/eProsima/Fast-CDR
# output: /usr/local/lib/libfastcdr.so.1.0.24
# ------------------------------------------------------------------------------
RUN git clone --branch v1.0.24 --depth 1 https://github.com/eProsima/Fast-CDR.git /tmp/Fast-CDR && \
    cmake -S /tmp/Fast-CDR -B /tmp/Fast-CDR/build \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_INSTALL_PREFIX=/usr/local && \
    cmake --build /tmp/Fast-CDR/build --parallel "$(nproc)" && \
    cmake --install /tmp/Fast-CDR/build && \
    ldconfig && \
    rm -rf /tmp/Fast-CDR

# ------------------------------------------------------------------------------
# foonathan_memory_vender
#
# FastDDS 2.x が依存するカスタムメモリアロケータ（foonathan/memory）のeProsimaベンダーラッパー
# aptではfoonathan_memory_vendorが提供されていないため、ソースからビルドする
# Repository: https://github.com/eProsima/foonathan_memory_vendor
# ------------------------------------------------------------------------------
RUN git clone --depth 1 https://github.com/eProsima/foonathan_memory_vendor.git /tmp/foonathan_memory_vendor && \
    cmake -S /tmp/foonathan_memory_vendor -B /tmp/foonathan_memory_vendor/build \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_INSTALL_PREFIX=/usr/local && \
    cmake --build /tmp/foonathan_memory_vendor/build --parallel "$(nproc)" && \
    cmake --install /tmp/foonathan_memory_vendor/build && \
    ldconfig && \
    rm -rf /tmp/foonathan_memory_vendor

# ------------------------------------------------------------------------------
# Fast RTPS (Fast DDS) v2.6.6
#
# DDS通信基盤
# Repository: https://github.com/eProsima/Fast-DDS
# output: /usr/local/lib/libfastrtps.so.2.6.6
# 依存： Fast CDR, foonathan_memory_vendor, OpenSSL, ASIO, tinyxml2
# ------------------------------------------------------------------------------
RUN git clone --branch v2.6.6 --depth 1 https://github.com/eProsima/Fast-DDS.git /tmp/Fast-DDS && \
    cmake -S /tmp/Fast-DDS -B /tmp/Fast-DDS/build \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_INSTALL_PREFIX=/usr/local \
      -DCOMPILE_EXAMPLES=OFF \
      -DEPROSIMA_BUILD_TYPE=OFF && \
    cmake --build /tmp/Fast-DDS/build --parallel "$(nproc)" && \
    cmake --install /tmp/Fast-DDS/build && \
    ldconfig && \
    rm -rf /tmp/Fast-DDS

# ------------------------------------------------------------------------------
# Fast DDS-Gen v2.1.4
# ------------------------------------------------------------------------------
RUN git clone --branch v2.1.4 --depth 1 https://github.com/eProsima/Fast-DDS-Gen.git /opt/Fast-DDS-Gen && \
    cd /opt/Fast-DDS-Gen && \
    chmod +x gradlew && \
    export JAVA_HOME=/usr/lib/jvm/java-11-openjdk-amd64 && \
    export PATH=$JAVA_HOME/bin:$PATH && \
    ./gradlew assemble && \
    printf '#!/usr/bin/env bash\nexec /opt/Fast-DDS-Gen/scripts/fastddsgen "$@"\n' \
      > /usr/local/bin/fastddsgen && \
    chmod +x /usr/local/bin/fastddsgen

WORKDIR /workspaces/FastDDS_practice

CMD ["/bin/bash"]
