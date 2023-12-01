# Makefile for Java program

# Compiler and compiler flags
JAVAC = javac
JFLAGS = -d bin -sourcepath src

# Source and target directories
SRCDIR = src
BINDIR = bin

# Java source files
SOURCES = $(wildcard $(SRCDIR)/*.java)

# Java class files
CLASSES = $(SOURCES:$(SRCDIR)/%.java=$(BINDIR)/%.class)

# Default target
all: $(CLASSES)

# Compile Java source files
$(BINDIR)/%.class: $(SRCDIR)/%.java
	@mkdir -p $(BINDIR)
	$(JAVAC) $(JFLAGS) $<

# Clean target
clean:
	rm -rf $(BINDIR)

# Run the Java program with arguments
run: all
	java -cp $(BINDIR) Main $(ARGS)

# Phony targets
.PHONY: all clean run

