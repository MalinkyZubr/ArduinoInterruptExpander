# This dockerfile creates the docker container for building the interface
 
FROM ubuntu:latest
WORKDIR /home/CompilationDir
RUN apt update -y
RUN apt install gcc-avr binutils-avr avr-libc gdb-avr avrdude arduino -y
RUN apt clean
RUN rm -rf /var/lib/apt/lists/*

CMD [ "bash" ]