FROM gcc:10.5.0
COPY . ~/Workspace/OSS24
WORKDIR ~/Workspace/OSS24
RUN make
CMD ["./oss"]