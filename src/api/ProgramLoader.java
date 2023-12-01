package api;

import java.io.FileReader;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Scanner;

public class ProgramLoader {

  private FileReader fileReader;

  private final char[] data;

  public ProgramLoader(String fileName) {
    if (fileName == null) {
      System.out.println("Please enter a file name.");
      System.exit(0);
    }

    // Open the file
    try {
      fileReader = new FileReader(Paths.get(fileName).toFile());
    } catch (Exception e) {
      System.out.println("File not found.");
      System.exit(0);
    }

    Scanner sc = new Scanner(fileReader);
    String line;
    ArrayList<Character> buffer = new ArrayList<>();
    while (sc.hasNextLine()) {
      line = sc.nextLine();

      // Add bytes to buffer
      for (int i = 0; i < line.length(); i++) {
        switch (line.charAt(i)) {
          case '+' -> buffer.add('+');
          case '-' -> buffer.add('-');
          case '>' -> buffer.add('>');
          case '<' -> buffer.add('<');
          case '.' -> buffer.add('.');
          case ',' -> buffer.add(',');
          case '[' -> buffer.add('[');
          case ']' -> buffer.add(']');
        }
      }
    }
    // Move buffer to data
    data = new char[buffer.size()];
    for (int i = 0; i < buffer.size(); i++) {
      data[i] = buffer.get(i);
    }
  }

  public char read(int programCounter) {
    return data[programCounter];
  }

  public int size() {
    return data.length;
  }

  @Override
  public String toString() {
    return Arrays.toString(data);
  }
}
