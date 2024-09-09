extra-file list
  main_{0, 1, 2, 3, or 4}_test_dir
    6_byte.txt "abcde\n"
    empty_f
    read_only_abc "abc\n"

extra compiler args
cl_libs = user32.lib gdi32.lib winmm.lib shlwapi.lib shell32.lib
cc_libs = -lwinmm -lShLwApi -lShell32
