reducecache v0.02 (2013/04/29)
==============================

Summary
-------

A memory manager of Windows tends to allocate much memory to disk cache.
It is said that Windows 7 series fixes the behavior much but sometimes
diskcache holds most of physical memory.

This software limits disk cache to small size for a moment so that
memory allocated as disk cache is deallocated.

Contents of package
-------------------

- README.md       : This document
- reducecache.exe : Executable
- reducecache.txt : Document written in Japanese

Source code can be retrieved from https://github.com/yak1ex/reducecache

Environment
-----------

This software is woked on 64bit Windows.
Working is confirmed on Windows 7 64bit and 2003 Server R2.
Only memory-rich environment has visible effect, and needs,
You need to have Administrator privilege to run this software.

Build environment is mingw64-x86_64-gcc-core on Cygwin.

Usage
-----

Invoke this software in cmd.exe with Administrator rights.

- ```reducecache.exe -h```
Show help

- ```reducecache.exe ```
Run as if 67108864 (= 64 * 1024 * 1024 = 64MB) is specified.

- ```reducecache.exe -v 0x4000000```
You can specifiy limit as hexadecimal.
Status is shown if -v is specified.

Licence
-------

This software is distrubuted under the terms of NYSL 
http://www.kmonos.net/nysl/

Limitation and known bugs
-------------------------

- If a too small value is specified, error 87 occurs. This is OS behavior. In my environment, the smallest possible value is 0x1000000 = 16777216 = 16MB.

Contact
-------

Yak! yak_ex@mx.scn.tv, @yak_ex in twitter, http://8904.teacup.com/yak_ex/bbs or https://github.com/yak1ex/reducecache/issues

History
-------

- 2013/04/29 (v0.02)
  - Support command line arguments.

- 2013/03/06 (v0.01)
  - Initial release (not public).
