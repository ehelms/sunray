PROJECT(SunRay)

ADD_LIBRARY(httpinterface  HttpPullData.cpp)
TARGET_LINK_LIBRARIES(httpinterface curl stdc++ ssl crypto z xml2 boost_filesystem)

ADD_EXECUTABLE(post post.cpp HttpPullData.cpp)
TARGET_LINK_LIBRARIES(post httpinterface)
