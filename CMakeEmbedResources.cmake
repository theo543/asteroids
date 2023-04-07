# For some reason, executing this file at configure-time so clang-tidy and cppcheck can find the generated files doesn't work.
# I don't know why. It "fails to process" even though launching this file at compile-time or via terminal works.
# But launching a subprocess on CMakeLists.txt works even at configure-time, so I moved the code there.
# TODO: Figure out why this doesn't work at configure-time so I can split the code back into two files.
