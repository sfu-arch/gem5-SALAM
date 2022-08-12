# gem5-SALAM #

gem5-SALAM (System Architecture for LLVM-based Accelerator Modeling), is a novel system architecture designed to enable LLVM-based modeling and simulation of custom hardware accelerators.

Interactive Demo: https://youtu.be/nVlh0J89Qyw

Presentation: https://youtu.be/IIl5TlbbfHE

# Requirements

**Please note that Ubuntu 20.04 is not currently supported**

- gem5 dependencies
- LLVM-9 or newer
- Frontend LLVM compiler for preferred development language (eg. clang for C)

# gem5-SALAM Setup

## Dependencies for gem5-SALAM

### Required

#### All Required Dependencies

```bash
sudo apt install build-essential m4 scons python-dev libprotobuf-dev python-protobuf protobuf-compiler libgoogle-perftools-dev
```

#### GCC (The Compiler used)

``` bash
sudo apt install build-essential m4
```

#### SCons (Build Environment)

```bash
sudo apt install scons
```

#### Python 2 (Required Libraries)

```bash
sudo apt install python-dev
```

#### Protobuf (Used for trace playback and generation)

```bash
sudo apt-get install libprotobuf-dev python-protobuf protobuf-compiler libgoogle-perftools-dev
```

### Recommended

#### Visualization

gem5 offers the ability to create a system configuration diagram. In order to be able to create these when using gem5-SALAM, the following packages are required:

- python-pydot
- python-pydot-ng
- graphviz

The following command can be run in Ubuntu to install the required dependencies:

``` bash
sudo apt install python-pydot python-pydot-ng graphviz
```

#### ARM GCC Cross Compiler

```bash
sudo apt-get install gcc-arm-none-eabi
```

## LLVM/Clang Setup 

You can install the latest version of LLVM via your system package manager or alternatively build from source found at https://github.com/llvm/llvm-project.

# Building gem5-SALAM

Once you have successfully installed all of the necessary dependencies, you can go ahead and clone the gem5-SALAM repository to begin working with it.

```bash
git clone https://github.com/TeCSAR-UNCC/gem5-SALAM
```

When building gem5-SALAM, there are multiple different binary types that can be created. Just like in gem5 the options are debug, opt, fast, prof, and perf. 

Below are the bash commands you would use to build the opt or debug binary. 

```bash
scons build/ARM/gem5.opt -j`nproc`
```

```bash
scons build/ARM/gem5.debug -j`nproc`
```

For more information regarding the binary types, and other build information refer to the gem5 build documentation [here](http://learning.gem5.org/book/part1/building.html).

# Using gem5-SALAM

To use gem5-SALAM you need to define the computation model of you accelerator in your language of choice,and compile it to LLVM IR. Any control and dataflow graph optimization (eg. loop unrolling) should be handled by the compiler. You can construct accelerators by associating their LLVM IR with an LLVMInterface and connecting it to the desired CommInterface in the gem5 memory map.

Below are some resources in the gem5-SALAM directory that can be used when getting started:

- Examples for system-level configuration can be found in **configs/common/HWAcc.py**.
- Accelerator benchmarks and examples can be found in the **benchmarks** directory. 
- The **benchmarks/common** directory contains basic drivers and syscalls for baremetal simulation.
- **benchmarks/sys_validation** contains examples for configuring and using gem5-SALAM with different algorithms.

## System Validation Examples

The system validation examples under **benchmarks/sys_validation** are good examples for how you interface with the gem5-SALAM simulation objects.

In order to use the system validation benchmarks, it is required to have the ARM GCC cross-compiler installed. If you didn't already install it when you setup the dependencies, you can install it in Ubuntu by running the below command:

```bash
sudo apt-get install gcc-arm-none-eabi
```

**systemValidation.sh** requires an environment variable named **M5_PATH** to be set. You will want to point it to your gem5-SALAM path as shown below. 

```bash
export M5_PATH=/path/to/gem5-SALAM
```

Next, compile your desired example. 

```bash
cd $M5_PATH/benchmarks/sys_validation/[benchmark]
make
```

Finally, you can run any of the benchmarks you have compiled by running the system validation script.

```bash
./systemValidation.sh -b [benchmark]
```

If you would like to see the gem5-SALAM command created by the shell file you would just need to inspect the **RUN_SCRIPT** variable in the shell file. 

# Resources

## gem5 Documentation

https://www.gem5.org/documentation/

## gem5 Tutorial

The gem5 documentation has a [tutorial for working with gem5](http://learning.gem5.org/book/index.html#) that will help get you started with the basics of creating your own sim objects.

## Building and Integrating Accelerators in gem5-SALAM

We have written a guide on how to create the GEMM system validation example. This will help you get started with creating your own benchmarks and systems. It can be viewed [here](https://github.com/TeCSAR-UNCC/gem5-SALAM/blob/master/docs/Building_and_Integrating_Accelerators.md).

## SALAM Object Overview

The [SALAM Object Overview](https://github.com/TeCSAR-UNCC/gem5-SALAM/blob/master/docs/SALAM_Object_Overview.md) covers what various Sim Objects in gem5-SALAM are and their purpose.

## Full-system OS Simulation ##

Please download the latest version of the Linux Kernel for ARM from the [gem5 ARM kernel page](http://gem5.org/ARM_Kernel).
You will also need the [ARM disk images](http://www.gem5.org/dist/current/arm/) for full system simulation.
Devices operate in the physical memory address space.

## Create a Cluster Cache

1. Go to config/SALAM/generated/file name, to line 27. Add `cache_size=’1MB’` to `connect_caches()`
2. Add `clstr.top.acp = clstr.coherency_bus.cpu_side_ports` to top config.
3. Go to src/hwacc/AccCluster.py line 64 : make it: `if(cache_size!=0):`
4. Go to system_validation.sh file and comment out *`${M5_PATH}/SALAM-Configurator/systembuilder.py --sysName $BENCH --benchDir "benchmarks/test-cases/${BENCH}"`*
5. Run the system_validation file.

## How to run ad benchmarks?
run:
```
./ad.sh -b $benchmark
```
For example:
```
./ad.sh -b gravity
```