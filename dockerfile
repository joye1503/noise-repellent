FROM ubuntu:18.04

ENV PATH=$PATH:/opt/mingw64/bin:/opt/local/bin

WORKDIR /noise-repellent

RUN apt-get update -qq -y && apt-get install software-properties-common -y
RUN add-apt-repository ppa:tobydox/mingw-w64 -y  \
    && apt-get update -qq
RUN apt-get install wget -y

RUN apt-get install -y \
    g++ \
    libssl1.0.0 \
    ninja-build \
    meson \
    zip \
    lv2-dev \
    pkg-config \
    fftw3-dev \
    fftw-mingw-w64 \
    gcc-mingw-w64 \
    mingw-w64-tools

# Apple 
RUN wget -c https://launchpad.net/~flosoft/+archive/ubuntu/cross-apple/+files/apple-uni-sdk-10.5_20110407-0.flosoft1_amd64.deb --no-check-certificate
RUN wget http://zamaudio.com/mbox2/apple-macports-fftw-3_3.3.5-1_all.deb
RUN wget http://zamaudio.com/mbox2/apple-x86-gcc_4.2.1~5646-1kxstudio2_amd64.deb
RUN wget http://zamaudio.com/mbox2/apple-x86-odcctools_758.159-0kxstudio2_amd64.deb 
RUN wget http://zamaudio.com/mbox2/apple-x86-setup_2_amd64.deb

RUN dpkg -i apple-x86-odcctools_758.159-0kxstudio2_amd64.deb && \
	dpkg -i apple-x86-gcc_4.2.1~5646-1kxstudio2_amd64.deb && \
	dpkg -i apple-uni-sdk-10.5_20110407-0.flosoft1_amd64.deb && \
	dpkg -i apple-x86-setup_2_amd64.deb && \
	dpkg -i apple-macports-fftw-3_3.3.5-1_all.deb
