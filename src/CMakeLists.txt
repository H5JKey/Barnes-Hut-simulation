add_executable(Barnes-Hut-simulation 
    main.cpp
    quad-tree.cpp
    particle-system.cpp
    physics-engine.cpp
    force-calculator.cpp
)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(MSVC)
        target_compile_options(Barnes-Hut-simulation PRIVATE 
        /O2
        /fp:fast
        /arch:AVX2
        /openmp:experimental
        /GL
        /Oi
        /LTCG
    )
    else()
        target_compile_options(Barnes-Hut-simulation PRIVATE 
        -O3 
        -ffast-math 
        -march=native
        -fno-trapping-math
        -fno-signed-zeros
        -fno-math-errno
        -fassociative-math
        -frename-registers
        -fomit-frame-pointer
        -ftree-vectorize 
        -funroll-loops
        -flto=auto
    )
    endif()
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Profile")
  if(NOT MSVC)
    find_package(PkgConfig REQUIRED)
      pkg_check_modules(GPERFTOOLS REQUIRED libprofiler)
      target_compile_options(Barnes-Hut-simulation PRIVATE 
        -O3 
        -ffast-math 
        -march=native
        -fno-trapping-math
        -fno-signed-zeros
        -fno-math-errno
        -fassociative-math
        -frename-registers
        -fno-omit-frame-pointer
        -ftree-vectorize 
        -funroll-loops
        -g
        -flto=auto
      )
      
      target_link_libraries(Barnes-Hut-simulation PRIVATE 
          ${GPERFTOOLS_LIBRARIES}
      )

      target_compile_definitions(Barnes-Hut-simulation PRIVATE 
          ENABLE_PROFILING=1
      )
  endif()
endif()


set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(OpenMP)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

target_link_libraries(Barnes-Hut-simulation PRIVATE 
  sfml-graphics
  sfml-window
  sfml-system
  OpenMP::OpenMP_CXX
)

if(WIN32)
  if (BUILD_SHARED_LIBS)
    add_custom_command(TARGET Barnes-Hut-simulation POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
          $<TARGET_FILE:sfml-graphics>
          $<TARGET_FILE:sfml-window>
          $<TARGET_FILE:sfml-system>
          $<TARGET_FILE_DIR:Barnes-Hut-simulation>)
  endif()
endif()



