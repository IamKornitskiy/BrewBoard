add_library(Models STATIC
        Beer.cpp Beer.h
        BeerListModel.cpp BeerListModel.h
)

target_link_libraries(Models
        Qt6::Core
)