set(base_SRCS
  Condition.cc
  CountDownLatch.cc
  Exception.cc
  FileUtil.cc
  LogFile.cc
  Logging.cc
  LogStream.cc
  ProcessInfo.cc
  Thread.cc
  ThreadPool.cc
  Timestamp.cc
  )

add_library(base ${base_SRCS})
target_link_libraries(base pthread rt)

install(TARGETS base DESTINATION lib)
file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/base)

#if(NOT CMAKE_BUILD_NO_EXAMPLES)
#  add_subdirectory(tests)
#endif()
