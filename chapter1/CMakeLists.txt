add_executable(
	Chapter1
	main.cpp
)

target_compile_definitions(
	Chapter1 PRIVATE
	NOMINMAX
	_USE_MATH_DEFINES
)
