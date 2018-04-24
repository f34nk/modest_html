add_executable(test-cbench test-cbench.c)
target_link_libraries(test-cbench cbench)

add_test(test-cbench test-cbench)
