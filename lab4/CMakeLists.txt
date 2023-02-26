project(lab4)

set(HEADERS
        tinyThreadPool/blocking_queue.hpp
        tinyThreadPool/task.hpp
        tinyThreadPool/WaitGroup.hpp
        tinyThreadPool/thread_pool.hpp
        Analyzer.hpp
        )

set(SOURCES
        tinyThreadPool/thread_pool.cpp
        main.cpp
        )

add_executable(lab4 ${HEADERS} ${SOURCES})