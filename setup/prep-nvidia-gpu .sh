#!/bin/bash
# @Author:   3sne ( Mukur Panchani )
# @File:     prep-nvidia-gpu.sh
# @Location: www.github.com/3sne/Parallel-Processing-Lab/setup/prep-nvidia-gpu.sh
echo; echo; echo
echo Start...
sudo apt update
sudo apt install nvidia-390
sudo apt install nvidia-opencl-icd-390
sudo apt install nvidia-cuda-toolkit
nvidia-smi
echo; echo; echo