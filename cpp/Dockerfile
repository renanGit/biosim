FROM ubuntu:24.10

WORKDIR /app

COPY ./*.cpp .
COPY CMakeLists.txt .
COPY ./include/* ./include/

RUN apt-get update && apt-get install -y g++ cmake

RUN mkdir build && cd build && cmake .. && cmake --build .

EXPOSE 3000

CMD ["./build/bin/darwin_service"]