# Advertencias del compilador comunes a todos los objetivos propios.
#
# Uso:  calc_set_warnings(<objetivo>)
#
# Las bibliotecas de terceros no pasan por aquí: se incluyen como SYSTEM para
# que sus advertencias no contaminen la salida.
function(calc_set_warnings target)
  if(MSVC)
    target_compile_options(${target} PRIVATE /W4 /permissive- /utf-8)
    if(CALC_WARNINGS_AS_ERRORS)
      target_compile_options(${target} PRIVATE /WX)
    endif()
  else()
    target_compile_options(${target} PRIVATE
      -Wall
      -Wextra
      -Wpedantic
      -Wshadow
      -Wconversion
      -Wsign-conversion
      -Wnon-virtual-dtor
      -Wold-style-cast)
    if(CALC_WARNINGS_AS_ERRORS)
      target_compile_options(${target} PRIVATE -Werror)
    endif()
  endif()
endfunction()
