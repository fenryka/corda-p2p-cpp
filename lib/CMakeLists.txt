include_directories(include)

pkg_check_modules(JSON REQUIRED nlohmann_json)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(test)

