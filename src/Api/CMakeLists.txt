add_library(Api STATIC
        UntappdApiClient.cpp
)

target_link_libraries(Api
        Qt6::Core
        Qt6::Network
        Models
)