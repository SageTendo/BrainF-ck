import api.ProgramLoader;
import api.Tape;
import java.util.HashMap;
import java.util.Scanner;
import java.util.Stack;
import java.util.ArrayList;

public class Main {

  private static int programCounter = 0;
  private static Stack<Integer> stack;
  private static ProgramLoader program;
  private static HashMap<Integer, Integer> jumpsMap;

  public static void main(String[] args) {
    if (args.length == 0) {
      System.out.println("Please provide a FrainBuck program...");
      System.exit(0);
    }

    program = new ProgramLoader(args[0]);
    stack = new Stack<>();
    Tape tape = new Tape();
    ArrayList<Character> buffer = new ArrayList<>();
    jumpsMap = new HashMap<>();

    handleLoops();
    while (programCounter < program.size()) {
      char instruction = program.read(programCounter);
      switch (instruction) {
        case '+' -> {
          int data_byte = tape.read() + 1;
          if (data_byte == 256) {
            data_byte = 0;
          }
          tape.write(data_byte);
        }
        case '-' -> {
          int data_byte = tape.read() - 1;
          if (data_byte == -1) {
            data_byte = 255;
          }
          tape.write(data_byte);
        }
        case '>' -> tape.inc_pointer();
        case '<' -> tape.dec_pointer();
        case '.' -> System.out.print((char) tape.read());
        case ',' -> {
          int data_byte = getInput(buffer);
          tape.write(data_byte);
        }
        case '[' -> {
          if (tape.read() == 0) {
            programCounter = jumpsMap.get(programCounter) + 1;
            continue;
          }

          // Push start of loop to stack
          stack.push(programCounter);
        }
        case ']' -> {
          if (tape.read() != 0) {
            programCounter = stack.peek() + 1;
            continue;
          }

          // Pop start of loop
          stack.pop();
        }
      }
      programCounter++;
    }
    System.out.println();
  }

  private static void handleLoops() {
    // Scan loops
    while (programCounter < program.size()) {
      char instruction = program.read(programCounter);
      switch (instruction) {
        case '[' -> stack.push(programCounter);
        case ']' -> {
          int loopStart = stack.pop();
          jumpsMap.put(loopStart, programCounter);
        }
      }
      programCounter++;
    }
    programCounter = 0;
  }


  private static int getInput(ArrayList<Character> buffer) {
    if (buffer.size() > 0) {
      return buffer.remove(0);
    }

    Scanner sc = new Scanner(System.in);
    System.out.print("\nUser input: ");

    String line = sc.nextLine();
    if (line.length() == 0) {
      return '\n';
    }

    if (line.equalsIgnoreCase("quit")) {
      return '\0';
    }

    for (int i = 0; i < line.length(); i++) {
      buffer.add(line.charAt(i));
    }
    return (int) buffer.remove(0);
  }
}