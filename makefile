# the compiler: gcc for C program, define as clang for C
  CC = clang

  # compiler flags:
  #  -O3 enables optimization to speedup code

  CFLAGS  = -03 

  # the build target executable:
  TARGET = myprog

  all: $(TARGET)

  $(TARGET): $(TARGET).c
  	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

  clean:
  	$(RM) $(TARGET)