# Physics Simulations

Implementing physics I'm learning at school using SFML and C++

## Dependencies and how to test

Obviously, a C++ compiler is required.<br>

I, personally, use the Visual Studio compiler, you can download it with the [IDE](https://visualstudio.microsoft.com/it/)<br>

To test the simulations you **need to download** the following libraries:<br>

+ [SFML](https://www.sfml-dev.org/)
+ [ROOT](https://root.cern/)

Make sure you download the **right libraries for your compiler**<br>
Then, change the vars in the `CMakeLists.txt` file with **your paths**:
```cmake
# VARS
set(ROOT_BIN_DIR "C:/root_v6.30.02/bin")
set(ROOT_DIR "C:/root_v6.30.02/cmake")
set(SFML_DIR "C:/SFML-2.6.1/lib/cmake/SFML")
```