set(http_SRCS
  HttpServer.cc
  HttpResponse.cc
  )

add_library(http ${http_SRCS})
target_link_libraries(http net)

install(TARGETS http DESTINATION lib)
set(HEADERS
  HttpRequest.h
  HttpResponse.h
  HttpServer.h
  )
install(FILES ${HEADERS} DESTINATION include/http)

#if(NOT CMAKE_BUILD_NO_EXAMPLES)
#add_executable(httpserver_test tests/HttpServer_test.cc)
#target_link_libraries(httpserver_test muduo_http)
#
#if(BOOSTTEST_LIBRARY)
#add_executable(httprequest_unittest tests/HttpRequest_unittest.cc)
#target_link_libraries(httprequest_unittest muduo_http boost_unit_test_framework)
#endif()
#
#endif()

# add_subdirectory(tests)
