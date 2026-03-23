cmake_minimum_required(VERSION 3.22.1)

project(loader)

# libcurl
add_library(libcurl STATIC IMPORTED)
set_target_properties(libcurl PROPERTIES
    IMPORTED_LOCATION
    ${CMAKE_SOURCE_DIR}/sharedjni/curl/curl-android-${ANDROID_ABI}/lib/libcurl.a
)

# libssl
add_library(libssl STATIC IMPORTED)
set_target_properties(libssl PROPERTIES
    IMPORTED_LOCATION
    ${CMAKE_SOURCE_DIR}/sharedjni/curl/openssl-android-${ANDROID_ABI}/lib/libssl.a
)

# libcrypto
add_library(libcrypto STATIC IMPORTED)
set_target_properties(libcrypto PROPERTIES
    IMPORTED_LOCATION
    ${CMAKE_SOURCE_DIR}/sharedjni/curl/openssl-android-${ANDROID_ABI}/lib/libcrypto.a
)

file(GLOB anjay "client/*.cpp" "sharedjni/*.cpp")

add_library(yogs SHARED ${anjay})

target_include_directories(yogs PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_SOURCE_DIR}/sharedjni/curl/curl-android-${ANDROID_ABI}/include
            ${CMAKE_SOURCE_DIR}/sharedjni/curl/openssl-android-${ANDROID_ABI}/include)

target_link_libraries(yogs z log android libcurl libssl libcrypto)

file(GLOB anjayy "server/*.cpp")
add_executable(libsock64.so ${anjayy})
target_include_directories(libsock64.so PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(libsock64.so z log android)

add_custom_command(TARGET libsock64.so POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_SOURCE_DIR}/../assets"
		COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:libsock64.so> "${CMAKE_CURRENT_SOURCE_DIR}/../assets/fuck"
		COMMENT "Moving libsock64.so to assets folder"
)