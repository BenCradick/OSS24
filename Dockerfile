FROM ubuntu/focal
COPY . ./OSS24
RUN make
CMD ["./oss"]