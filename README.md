# Robot Test

## Compile
```bash
docker pull ev3dev/debian-stretch-cross
docker run -it -v /home/user/srcdir:/src -w /src ev3dev/debian-stretch-cross
mkdir build
cd build
env CC=arm-linux-gnueabi-gcc CXX=arm-linux-gnueabi-g++ cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```

## Deploy
```bash
scp robot robot@ev3dev.local:~/
```

## Run
```bash
ssh robot@ev3dev.local
brickrun ./robot
```