## Parallel Processing Lab

<br/>

Notes on executing OpenCL programs. <br/>

Simply said, OpenCL is only a standard. The implemented code comes along with drivers provided by specific vendors.<br/>
So, in order to let OpenCL APIs look at your gpu, you need to have relevant drivers(which allow for APIs to talk to device) and SDKs (which contain code-base for OpenCL).<br/>

### LINUX:<br/>
  * INTEL UHD:<br/>
    * Get Intel OpenCL RunTime Libraries.<br/>
    * Install correct drivers(most distros have this pre-installed).<br/>
  * NVIDIA:<br/>
    * Get NVIDIA Cuda ToolKit.<br/>
    * Install correct drivers for your GPU.<br/>
  * Compile:<br/>
    * To compile "oclFile.c":<br/>
       `$gcc -g oclFile.c -lOpenCL` <br/>
    * You can use `-I` flag to include "CL" folder containing OpenCL headers while compiling if they aren't already installed natively (Installing above SDKs do this for you).<br/>

### WINDOWS:<br/>
Since windows doesn't natively run gcc (needs precompiled binaries, like MinGW), compiling and running OpenCL code without an IDE can be a pain.<br/>

  * Compiling:<br/>
     * Simplest way is to get the NVIDIA CUDA ToolKit, which contains OpenCL headers, and also the CUDA stuff. However, Compiling via gcc (MinGW) still won't work because of missing windows CL.exe binary. <br/>
     * The easiest option is to get Visual Studio IDE and NVIDIA CUDA ToolKit (or INTEL OpenCL SDK for Windows), and load dependencies and build config in VS.<br/>
  * Runtime:<br/>
     * In order to make sure your GPU is identified during run time, make sure you have correct drivers installed, be it Intel UHD, Nvidia or AMD.<br/>

### MAC:<br/>
I don't got Mac.<br/>
