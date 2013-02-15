pywt
====

Python bindings for WtPyWt consists of:
- an introspection tool based on clang to extract API information from
  Wt header files. This introspection tool generates the API description files.
- a set of python scripts that use the API description files to generate
  C++ code, being the core implementation of the python bindings
- a set of C++ support files which are not generated, but are also required
  to complete the binding

Since a few functions are missing from clang, PyWt uses a modified version
of clang to generate the introspection tool.

Steps to build:
1. build llvm and a patched clang
   https://github.com/wdu/clang
2. build and run the python-dump tool on wt.cc (add -I for Wt and boost)
3. run descr-cleanup.sh
4. run pywt-gen.sh
5. mkdir build; cd build; cmake ..; make

Running the examples:
- put the .so files in the extensions directory searched by python
- cd examples/hello
- python hello.py --docroot . --http-address 0.0.0.0 --http-port 8080
- don't forget to create a link to the resources directory of Wt when
  your example requires files from the resources folder (most examples do).

State of the PyWt bindings:
The state of the current project is alpha. It is work-in-progress, and is
released in order to obtain feedback on the API, architecture, and
performance. Any comments or patches are welcomed and appreciated. The Wt
fora are the appropriate place to discuss PyWt.

See LICENSE file for license information.

