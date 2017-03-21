add_library(args STATIC args.cpp ${ARGS_INC_HEADERS})
target_link_libraries(args fmt)
