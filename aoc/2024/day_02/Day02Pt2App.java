import java.io.FileReader;
import java.io.BufferedReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;

// read into two colums of integers
// sort both lists from smallest to largest
// calculate the sum of teh absolute value of the difference of each pair


class Day02App {
       public static int checkforsafe(int[] intvals) {	   
		int safe = 1;
		int vallen = intvals.length;
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
                return safe;
        }
    
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
		int safe = checkforsafe(intvals);
		if (safe == 1) {
		    sum++;
		} else {
		    int intvals2[];
		    intvals2 = new int[vallen-1];
		    for (int k=0; k < vallen; k++) {
			int kk = 0;
			for (int j=0; j < vallen - 1; j++) {
			    if (j == k) {
				kk = kk + 1;
			    } 
			    intvals2[j] = intvals[j+kk];
			}
			System.out.println(intvals2[0]);
			int safe2 = checkforsafe(intvals2);
			if (safe2 == 1) {
			    safe = 1;
			}
		    }
		    if (safe == 1) {
			sum++;
		    }
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

