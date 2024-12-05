import java.io.FileReader;
import java.io.BufferedReader;
import java.io.IOException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.ArrayList;
 
class Day04App {
        public static void main(String[] args) throws IOException {

        BufferedReader inputStream = null;

        try {
            inputStream = new BufferedReader(new FileReader("Day04.txt"));

            String line;

	    int nrows = 0;

	    ArrayList<String> rows = new ArrayList<>();
	    
            while ((line = inputStream.readLine()) != null) {
		if (line.trim().isEmpty()) {
		    continue;
		}
		rows.add(line);
		nrows++;
	    }
	    int ncols = ((String)rows.get(0)).length();
	    char grid[][];
	    grid = new char[nrows][ncols];
	    for (int i = 0; i < nrows; i++) {
		for (int j = 0; j < ncols; j++) {
		    grid[i][j] = ((String)rows.get(i)).charAt(j);
		}
	    }
	    int nfound = 0;
	    for (int i=1; i< nrows-1; i++) {
		for (int j=1; j < ncols-1; j++) {
		    if (grid[i][j] == 'A') {
			if (grid[i-1][j-1] == 'M' && grid[i-1][j+1] == 'M' && grid[i+1][j-1] == 'S' && grid[i+1][j+1] == 'S') {
			    nfound++;
			}
			if (grid[i-1][j-1] == 'M' && grid[i-1][j+1] == 'S' && grid[i+1][j-1] == 'M' && grid[i+1][j+1] == 'S') {
			    nfound++;
			}
			if (grid[i-1][j-1] == 'S' && grid[i-1][j+1] == 'S' && grid[i+1][j-1] == 'M' && grid[i+1][j+1] == 'M') {
			    nfound++;
			}
			if (grid[i-1][j-1] == 'S' && grid[i-1][j+1] == 'M' && grid[i+1][j-1] == 'S' && grid[i+1][j+1] == 'M') {
			    nfound++;
			}
		    }
		}
	    }
	    System.out.println(nfound);		    
	

        } finally {
            if (inputStream != null) {
                inputStream.close();
            }
        }
    }
}

