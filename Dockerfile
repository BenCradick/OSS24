FROM ubuntu/focal
COPY . ~/Workspace/OSS24
WORKDIR ~/Workspace/OSS24
RUN make
CMD ["./oss"]