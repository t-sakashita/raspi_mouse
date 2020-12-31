# Creating libbcm2835.a
add_library(bcm2835 bcm2835.c)

# Build test program
add_executable(test_program test.c)
target_link_libraries(test_program bcm2835)
