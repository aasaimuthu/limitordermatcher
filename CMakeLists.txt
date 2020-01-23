cmake_minimum_required(VERSION 2.6.4)
project(LimitOrderMatcher)

if (Boost_NO_SYSTEM_PATHS)
  set(BOOST_ROOT "/fs02/home/thirdparty//boost/1.63.0/$PLATFORM/")
  set(BOOST_INCLUDE_DIRS  "${BOOST_ROOT}/include")
  set(BOOST_LIBRARY_DIRS  "${BOOST_ROOT}/lib")
endif (Boost_NO_SYSTEM_PATHS)


include_directories(${BOOST_INCLUDE_DIRS}
              ./include
)

add_definitions(
    -fPIC
    -Wreturn-type
    -Wno-deprecated
    -Wno-unused-local-typedefs
    -Wno-sign-compare
    -std=c++11
    -Wno-error=maybe-uninitialized
)

add_executable(order_book 
     src/main.cpp
     src/LimitOrderBook.cpp
     src/InputOrderProcessor.cpp
)

set(${ORDER_TEST} order_book_test)
add_executable(order_book_test
     test/TestLimitOrder.cpp
     src/LimitOrderBook.cpp
)
                         
target_link_libraries(order_book_test
                      ${Boost_SYSTEM_LIBRARY}
                      ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)

