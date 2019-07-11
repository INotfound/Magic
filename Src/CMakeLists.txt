add_definitions(-g)
add_definitions(-std=c++11)
aux_source_directory(. ModelsSrc)
add_library(Models ${ModelsSrc})
