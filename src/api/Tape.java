package api;

import java.util.Arrays;
import java.util.concurrent.locks.Lock;

public class Tape {

  private int[] data;
  private int pointer = 0;

  public Tape() {
    data = new int[30000];
  }

  public void write(int data_byte) {
    data[pointer] = data_byte;
  }

  public int read() {
    return data[pointer];
  }

  public void inc_pointer() {
    pointer++;
    if (pointer >= size()) {
      data = Arrays.copyOf(data, size() * 2);
    }
  }

  public void dec_pointer() {
    pointer--;
    if (pointer < 0) {
      pointer = data.length - 1;
    }
  }

  public int size() {
    return data.length;
  }
}
