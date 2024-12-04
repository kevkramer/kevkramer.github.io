import java.io.FileReader;
import java.io.BufferedReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;

// read into two colums of integers
// sort both lists from smallest to largest
// calculate the sum of teh absolute value of the difference of each pair


class Day02App {

      public static void main(String[] args) throws IOException {

        BufferedReader inputStream = null;
	ArrayList<Integer> column1 = new ArrayList<>();

        try {
            inputStream = new BufferedReader(new FileReader("Day02.txt"));

            String line;

	    int sum = 0;
	    
            while ((line = inputStream.readLine()) != null) {
		if (line.trim().isEmpty()) {
		    continue;
		}

		String[] values = line.trim().split("\\s+");

		int intvals[];
		int vallen = values.length;
		intvals = new int[vallen];

		for (int i = 0; i< vallen; i++) {
		    intvals[i] = Integer.parseInt(values[i]);
		}
		int safe = 1;
		if (intvals[0] < intvals[1]) {
 		    int k = 0;
		    
		    while (k + 1 < vallen) {
			int diff = intvals[k+1] - intvals[k];
			if (diff < 1 || diff > 3) {
			    safe = 0;
			}
			k = k + 1;
		    }
		} else {
		    int k = 0;
		    
		    while (k + 1 < vallen) {
			int diff = intvals[k] - intvals[k+1];
			if (diff < 1 || diff > 3) {
			    safe = 0;
			}
			k = k + 1;
		    }
		}
		if (safe == 1) {
		    sum = sum + 1;
		}
	    }
            System.out.println(sum);
        } finally {
            if (inputStream != null) {
                inputStream.close();
            }
        }
    }
}

