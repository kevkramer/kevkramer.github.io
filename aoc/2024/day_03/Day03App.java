import java.io.FileReader;
import java.io.BufferedReader;
import java.io.IOException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

class Day03App {
        public static void main(String[] args) throws IOException {

        BufferedReader inputStream = null;

        try {
            inputStream = new BufferedReader(new FileReader("Day03.txt"));

            String line;

	    int sum = 0;
	    
            while ((line = inputStream.readLine()) != null) {
		if (line.trim().isEmpty()) {
		    continue;
		}

		Pattern pattern = Pattern.compile("mul\\((\\d+)\\,(\\d+)\\)");

		Matcher matcher = pattern.matcher(line);

		while (matcher.find()) {

  		   String match = matcher.group();
		   System.out.println(match);

		   int firstNumber = Integer.parseInt(matcher.group(1));
		   int secondNumber = Integer.parseInt(matcher.group(2));
		   System.out.println(firstNumber + " " + secondNumber);

		   sum += firstNumber*secondNumber;

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

