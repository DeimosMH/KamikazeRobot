# Unity Boids Simulation

*_UnityBoidsMAS*
Unity 2020.3.12f1 ver compatible simulation of Multi Agent System based on BOIDS algorithm.

# Robot Test C++

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