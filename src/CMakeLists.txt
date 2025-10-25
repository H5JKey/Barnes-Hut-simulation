add_executable(Barnes-Hut-simulation 
    main.cpp
    quad-tree.cpp
    particle.cpp
    physics-engine.cpp
)

target_link_libraries(Barnes-Hut-simulation PRIVATE 
  sfml-graphics
  sfml-window
  sfml-system
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

