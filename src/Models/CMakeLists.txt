add_library(Models STATIC
        Beer.cpp
)

target_link_libraries(Models
        Qt6::Core
)