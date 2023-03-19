include_directories(include)

if (UNIX AND NOT APPLE)
    find_package(nlohmann_json 3.2.0 REQUIRED)
else()
    pkg_check_modules(JSON REQUIRED nlohmann_json)
endif()

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(test)

