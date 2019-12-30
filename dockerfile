FROM multiarch/crossbuild
RUN git clone -b dockerbuild https://github.com/lucianodato/noise-repellent.git
ENV CROSS_TRIPLE=x86_64-linux-gnu
WORKDIR /workdir/noise-repellent
RUN crossbuild ./compile-all.sh