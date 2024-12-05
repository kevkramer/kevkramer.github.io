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
	    for (int i=0; i< nrows; i++) {
		for (int j=0; j < ncols; j++) {
		    if (j < ncols-3) {
			if (grid[i][j] == 'X' && grid[i][j+1] == 'M' && grid[i][j+2] == 'A' && grid[i][j+3] == 'S') {
			    nfound++;
			}
		    }
		    if (j > 2) {
			if (grid[i][j] == 'X' && grid[i][j-1] == 'M' && grid[i][j-2] == 'A' && grid[i][j-3] == 'S') {
			    nfound++;
			}
		    }
		    if (i < nrows-3) {
			if (grid[i][j] == 'X' && grid[i+1][j] == 'M' && grid[i+2][j] == 'A' && grid[i+3][j] == 'S') {
			    nfound++;
			}
		    }
		    if (i > 2) {
			if (grid[i][j] == 'X' && grid[i-1][j] == 'M' && grid[i-2][j] == 'A' && grid[i-3][j] == 'S') {
			    nfound++;
			}
		    }
		    if ((j < ncols-3) && (i < nrows - 3)) {
			if (grid[i][j] == 'X' && grid[i+1][j+1] == 'M' && grid[i+2][j+2] == 'A' && grid[i+3][j+3] == 'S') {
			    nfound++;
			}
		    }
		    if ((j < ncols-3) && (i > 2)) {
			if (grid[i][j] == 'X' && grid[i-1][j+1] == 'M' && grid[i-2][j+2] == 'A' && grid[i-3][j+3] == 'S') {
			    nfound++;
			}
		    }
		    if ((j > 2) && (i < nrows - 3)) {
			if (grid[i][j] == 'X' && grid[i+1][j-1] == 'M' && grid[i+2][j-2] == 'A' && grid[i+3][j-3] == 'S') {
			    nfound++;
			}
		    }
		    if ((j > 2) && (i > 2)) {
			if (grid[i][j] == 'X' && grid[i-1][j-1] == 'M' && grid[i-2][j-2] == 'A' && grid[i-3][j-3] == 'S') {
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

