cmake_minimum_required(VERSION 3.10)
project(MVPPoA)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)

add_executable(poa_node
    main.cpp
    ledger_integration.cpp
    pq_crypto.cpp
)

target_link_libraries(poa_node PRIVATE Threads::Threads)
