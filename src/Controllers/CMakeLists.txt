add_library(Controllers STATIC
        BeerSearchController.cpp
        BeerSearchController.h
)

target_link_libraries(Controllers
        Qt6::Core
        Api
        Models
)