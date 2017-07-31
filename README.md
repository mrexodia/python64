# python64

Hack to have both python and python64 in your PATH environment variable.

## Setup

1. Put `python64.exe` and `pip64.exe` in your global python path (`C:\Python27` for example).
2. Set a global environment variable called `PYTHON27X64` to your 64-bit `PYTHONHOME` (`C:\Python27-64` for example).
3. You can use do `python64.exe test.py` and `pip64 install numpy` without conflicts!