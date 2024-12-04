import java.io.FileReader;
import java.io.BufferedReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;

// read into two colums of integers
// sort both lists from smallest to largest
// calculate the sum of teh absolute value of the difference of each pair


class Day01App {

      public static void main(String[] args) throws IOException {

        BufferedReader inputStream = null;
	ArrayList<Integer> column1 = new ArrayList<>();
	ArrayList<Integer> column2 = new ArrayList<>();

        try {
            inputStream = new BufferedReader(new FileReader("day01input.txt"));

            String line;
	    
            while ((line = inputStream.readLine()) != null) {
		if (line.trim().isEmpty()) {
		    continue;
		}

		String[] values = line.trim().split("\\s+");

		if (values.length != 2) {
		    throw new IOException("Invalid line format:" + line);
		}

		try {
		    column1.add(Integer.parseInt(values[0]));
		    column2.add(Integer.parseInt(values[1]));
		} catch (NumberFormatException e) {
		    throw new IOException("Invalid number format in line:" + line);
		}
	    }
	    int[] array1 = column1.stream().mapToInt(Integer::intValue).toArray();
	    int[] array2 = column2.stream().mapToInt(Integer::intValue).toArray();

	    Arrays.sort(array1);
	    Arrays.sort(array2);

	    int sum = 0;
	    for (int i = 0; i < array1.length; i++) {
		sum += Math.abs(array1[i] - array2[i]);
	    }
	    System.out.println(sum);
        } finally {
            if (inputStream != null) {
                inputStream.close();
            }
        }
    }
}

