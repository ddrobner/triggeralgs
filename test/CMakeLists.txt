
add_executable(test_trivial test_trivial.cxx)
target_include_directories(test_trivial PRIVATE ${BOOST_INCLUDE_DIRS})
add_test(NAME trivial COMMAND test_trivial)

add_executable(test_supernova test_supernova.cxx)
target_include_directories(test_supernova PRIVATE ${BOOST_INCLUDE_DIRS})
add_test(NAME supernova COMMAND test_supernova)
