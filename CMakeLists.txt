cmake_minimum_required(VERSION 3.5)
project(DeliveryApp)

# Specify the Python include and library directories
set(PYTHON_INCLUDE_DIR "C:/Users/Gyugyu/AppData/Local/Programs/Python/Python312/include")
set(PYTHON_LIBRARY "C:/Users/Gyugyu/AppData/Local/Programs/Python/Python312/libs/python312.lib")

# Add the executable for the C source file and name it DeliveryApp
add_executable(DeliveryApp "C:/Users/Gyugyu/Desktop/DeliveryApp/DeliveryApp.c")

# Specify the include directories and link the Python library
target_include_directories(DeliveryApp PRIVATE ${PYTHON_INCLUDE_DIR})
target_link_libraries(DeliveryApp ${PYTHON_LIBRARY})
