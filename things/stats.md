# Statistics

## Descriptive Statistics

### Mean

$$
\bar x = \frac 1 n \sum_ {i=1}^n  x_i
$$

### Median

The median is the middle value in an ordered set of data. If there are an even number of values it is the average of the middle 2 values in the set.

### Mode

The mode is the most common value (or values) in a set of data.

### Dispersion

Dispersion refers to measures of how spread out our data is. The simplest measure of dispersion is the range which is the difference between the largest and smallest elements in a set of data.

### Variance

A more sophisticated measure of dispersion is the variance. You can think of this as the average squared deviation from the mean. However we modify the sum by dividing by $n-1$ because when dealing with a sample from a larger population, $\bar{x}$ is only an estimate of the actual mean. On average this means $x_i - \bar{x}$ is an underestimate of the true deviation which is why we divide by a smaller denominator.

[QUESTION: Why only subtract 1?]

[QUESTION: What if you actually do have the entire population?]

$$
S^2 = \frac {\sum {(x_i - \bar{x})}^2} {n - 1}
$$

$S^2$ = sample variance\
$x_i$ = value of the one observation\
$\bar{x}$ = mean value of all observations\
$n$ = number of observations

### Standard Deviation

The standard deviation is the square root of the variance which means it is in the same units as the range of the dataset.

$$
S = \sqrt {\frac {\sum {(x_i - \bar{x})}^2} {n - 1}}
$$

$S^2$ = sample variance\
$x_i$ = value of the one observation\
$\bar{x}$ = mean value of all observations\
$n$ = number of observations

### Interquartile Range

The interquartile range is the difference between the 75th and 25th percentile value. This is a simple measure but useful as it is not affected by outliers like the range and standard deviation are.

### Covariance

Whereas variance measures how a single variable deviates from its mean, covariance measures how two variables vary in tandem from their means.

$$
cov_{x,y} = \frac {\sum {(x_i - \bar{x})(y_i - \bar{y})}} {N - 1}
$$

$cov_{x,y}$ = covariance between variable x and y\
$x_i$ = data value of x\
$y_i$ = data value of y\
$\bar{x}$ = mean of x\
$\bar{y}$ = mean of y\
$N$ = number of data values

A "large" positive covariance means that $x$ tends to be large when $y$ is large and small when y is small. A “large” negative covariance means the opposite — that $x$ tends to be small when $y$ is large and vice versa. A covariance close to zero means that no such relationship exists.

Covariance can be hard to interpret because:

- Its units are the product of the inputs' units which can be hard to make sense of.
- If each every value of $x$ was doubled (with $y$ staying the same) the covariance would be twice as large. But the variables would be just as interrelated. So it’s hard to say what counts as a "large" covariance.

### Correlation

Pearson's correlation coefficient is the covariance of the two variables divided by the product of their standard deviations:

$$
\rho(X,Y) = \frac {cov(X,Y)}{\sigma(X)\sigma(Y)}
$$

$cov$ = the covariance\
$\sigma(X)$ = the standard deviation of X\
$\sigma(Y)$ = the standard deviation of Y\

Correlation shows us relationships in which knowing how $x_i$ compares to the mean of $x$ gives us information about how $y_i$ compares to the mean of $y$. Other types of relationship may not show up. It also doesn't tell us how large the relationship is. It is also the case that if $E$ and $F$ are independent if the probability of them both happening is equal to the product of the probability of each one happening.

#### Simpson's Paradox

## Probability

Notationally, $P(E)$ means the probability of the event $E$.

Events $E$ and $F$ are `dependent` if inforamation about whether $E$ happens gives us information about whether $F$ happens or vice versa. Otherwise the two events are independent of each other.

$$ P(E,F) = P(E)P(F) $$

If the two events are not necessarily independent (and the probability of $F$ is not 0) we define probability of $E$ given $F$ as:

$$ P(E|F) = \frac {P(E,F)}{P(F)} $$

This is often written as:

$$ P(E,F) = P(E|F)P(F) $$

When $E$ and $F$ are independent this means:

$$ P(E|F) = P(E) $$

Let's ask _what is the probability "both children are girls" conditional on the event "the older child is a girl"?_

We use the definition of conditional probability and the fact that the event $Both$ and $Girl$ ("both children are girls and the older child is a girl") is the same as the event $Both$.

$$
\begin{align}
P(Both|Girl) &= \frac {P(Both, Girl)}{P(Girl)} \notag \\
&= \frac {P(Both)}{P(Girl)} \notag \\
&= \frac {1}{2} \notag
\end{align}
$$

We can also ask about the probability of the event "both children are girls" conditional on the event "at least one of the children is a girl" ($At\ Least\ One$). Surprisingly, the answer is different from before. If all you know is that at least one of the children is a girl, then it is twice as likely that the family has one boy and one girl than that it has both girls.

$$
\begin{align}
P(Both | At\ Least\ One) &= \frac {P(Both, At\ Least\ One)} {P(At\ Least\ One)} \notag \\
&= \frac {P(Both)} {P(At\ Least\ One)} \notag \\
&= \frac {1}{3} \notag
\end{align}
$$

### Bayes' Theorem

Bayes’s theorem is a way of “reversing” conditional probabilities. If we need to know the probability of some event $E$ conditional on some other event $F$ occurring but we only have information about the probability of $F$ conditional on $E$ occurring, using the definition of conditional probability twice tells us that:

$$
\begin{align}
P(E | F) &= \frac {P(E, F)} {P(F)} \notag \\
&= \frac {P(F | E)P(E)} {P(F)} \notag \\
\end{align}
$$

The event $F$ can be split into the two mutually exclusive events "$F$ and $E$" and "$F$ and not $E$". We write $\neg$ for "not".

$$
\begin{align}
P(F) &= P(F, E) + P(F, \neg E) \notag \\
P(E | F) &= \frac {P(F | E)P(E)} {P(F, E)P(E) + P(F, \neg E)P(\neg E)} \notag \\
\end{align}
$$

If a disease has a 1 in 10000 chance of occurring ($P(Disease) = 0.0001$) and a test has a 99% chance of giving a positive result when someone has the disease ($P(Positive|Disease) = 0.99$) we would apply Bayes Theorum like so:

$$
\begin{align}
P(Disease | Positive) &= \frac {P(Positive | Disease)P(Disease)} {P(Positive | Disease)P(Disease) + P(Positive | \neg Disease)P(\neg Disease)} \notag \\
&= \frac {0.99 \times 0.0001}{0.99 \times 0.0001 + 0.01 \times 0.9999} \notag \\
&\approx 0.0098 \notag \\
&= 0.98\% \notag
\end{align}
$$

Note that this assumes people take the test roughly at random. If only people with certain symptoms take the test, we would have to condition on the event "positive result and symptoms" and the number would likely be a lot higher.

We can also view this problem using natural frequencies. In a population of 1 million people you’d expect 100 of them to have the disease, and 99 of those 100 to test positive. On the other hand, you’d expect 999,900 of them not to have the disease, and 9,999 of those to test positive. That means you’d expect only 99 out of 10098 (99 + 9999) positive testers to actually have the disease which is roughly 0.98%.

### Random Variables

A random variable is a variable whose possible values have an associated probability distribution. The expected value of a random variable is the average of its values weighted by their probabilities.

A very simple random variable equals 1 if a coin flip turns up heads and 0 if the flip turns up tails. The variable equals 0 with probability 0.5 and 1 with probability 0.5. It has an expected value of $\frac{1}{2} = (0 * \frac{1}{2} + 1 * \frac{1}{2})$

A more complicated random variable might measure the number of heads you observe when flipping a coin 10 times or a value picked from `range(10)` where each number is equally likely. The associated distribution gives the probabilities that the variable realizes each of its possible values. In this case the variable has a distribution that assigns probability 0.1 to each of the numbers from 0 to 9. Flipping a coin 10 times has an expected value of 5.5. The range(10) variable has an expected value of 4.5.

$$
E(X) = \sum {X_iP(X_i)}
$$

$X_i$ = The values that X can take\
$P(X_i)$ = The probability that X takes the value $X_i$

Random variables can be conditioned on events just as other events can. Going back to the two-child example, if $X$ is the random variable representing the number of girls, $X$ equals 0 with probability $\frac{1}{4}$, 1 with probability $\frac{1}{2}$, and 2 with probability $\frac{1}{4}$. We can define a new random variable $Y$ that gives the number of girls conditional on at least one of the children being a girl. Then $Y$ equals 1 with probability $\frac{2}{3}$ and 2 with probability $\frac{1}{3}$. And a variable $Z$ that's the number of girls conditional on the older child being a girl equals 1 with probability $\frac{1}{2}$ and 2 with probability $\frac{1}{2}$.

### Continuous Distributions

### Normal Distribution

## Inferential Statistics

### Hypothosis Testing

### T Tests/Distribution

### ANOVA

When looking at data, you might want to know if two groups are different. ANOVA (analysis of variance) calculates the **between group variance** and the **within group variance**

The ratio of the SS (between SS divided by within SS) results is known as the F-statistic

### Variance ratio (F) test

## Regression

## Noise reduction

## Baysian Data Analysis

Bayesian data analysis takes a question in the form of a model and uses logic to produce an answer in the form of probability distributions.

In Baysian data analysis:

- For each possible explanation of the data, count all the ways the data can happen
- Explanations with more ways to produce the data are more plausible

Imagine a bag that contains 4 marbles. Some are blue and some are white. We don't know in what proportion. A table of all the possible combinations looks like this. These possible combinations are our **conjectures**.

|     |     |     |     |
| :-: | :-: | :-: | :-: |
|  B  |  B  |  B  |  B  |
|  B  |  B  |  B  |  W  |
|  B  |  B  |  W  |  W  |
|  B  |  W  |  W  |  W  |
|  W  |  W  |  W  |  W  |

Now suppose that we draw 3 marbles from the bag with replacement. The results from that will be our **data**.

$$
BWB
$$

How could we think about what the 4th marble is likely to be?

Notice that the number of ways to produce the data, for each conjecture, can be computed by first counting the number of paths in each “ring” of the garden and then by multiplying these counts together.

| Conjecture | Number of ways to produce the $$BWB$$ | Plausibility |
| :--------: | :-----------------------------------: | :----------: |
|  $$WWWW$$  |      $$0 \times 4 \times 0 = 0$$      |      0       |
|  $$BWWW$$  |      $$1 \times 3 \times 1 = 3$$      |     0.15     |
|  $$BBWW$$  |      $$2 \times 2 \times 2 = 8$$      |     0.4      |
|  $$BBBW$$  |     $$3 \times 1 \times 3  = 9$$      |     0.45     |
|  $$BBBB$$  |     $$4 \times 0 \times 4  = 0$$      |      0       |

The rules for Baysian updating:

1. State a causal model for how observations arise, given each possible explanation
2. Count ways data could arise for each explanation
3. Relative plausibility is relative value from (2)

Let's suppose we draw another $$B$$ from the bag. We can update our previous (**prior**) counts and update them in light of the new observation becuase the new observation is logically independent of the previous observations. First count the numbers of ways each conjecture could produce the new observation. Then multiply each of these new counts by the prior numbers of ways for each conjecture. In table form:

| Conjecture | Ways to produce $$B$$ | Previous counts |      New count      | New plausibility |
| :--------: | :-------------------: | :-------------: | :-----------------: | :--------------: |
|  $$WWWW$$  |           0           |        0        | $$0 \times 0 = 0$$  |        0         |
|  $$BWWW$$  |           1           |        3        | $$3 \times 1 = 3$$  |     $$0.07$$     |
|  $$BBWW$$  |           2           |        8        | $$8 \times 2 = 16$$ |     $$0.35$$     |
|  $$BBBW$$  |           3           |        9        | $$9 \times 3 = 27$$ |     $$0.57$$     |
|  $$BBBB$$  |           4           |        0        | $$0 \times 4 = 0$$  |        0         |

Obviously you want to normalise these values which is why we have a plausibilty colum. You can can converts the counts to probabilities with the following formula:

$$
plausibility\ of\ p\ after\ D_{new} = \frac {ways\ p\ can\ produce\ D_{new} \times prior\ plausibility\ p} {sum\ of\ products}
$$

<div class="blog-image">
![](https://henry-dashwood-public-assets.s3.eu-west-2.amazonaws.com/post-imgs/2022-02-05-sr-bayeian-stats/prob_dense_plot.gif)
</div>
Every updated set of plausibilities becomes the initial plausibilities for the  next observation. Every conclusion is the starting point for future inference. This updating process works backwards as well as forwards. Given a final set of plausibilities and knowing the final observation $$W$$ it is possible to mathematically divide out the observation to infer the previous plausibility curve. So the data could be presented to your model in any order, or all at once even. In most cases you will present the data all at once for convenience. This represents an abbreviation of an iterated learning process.

Some points about Baysian Inference:

1. There is no such thing as a minimum sample size. The curve is likely to be flat and wide when you have little data but that's fine. It might even be enough information to be useful in some sense.
2. The shape of the curve embodies the sample size. As you get more data the curve gets narrower and taller.
3. Point estimates don't have a big role in Baysian data analyis because the the estimate is the curve/distribution. Always use the entire posterior distribution because to do otherwise is to throw away uncertainty and generate overconfident answers. Summary should always be the last step.
4. Intervals don't have a strong role in Baysian inference. How wide to make an interval or even where to centre it is an arbitrary descision.

What proportion of the Earth is covered by water? We aren't going to visit literally every point on Earth. So how can we work this out? We can take a sample. Suppose we visit ten points of earth at random and record whether it is covered by land or water. Suppose these are our results :

$$
W, W, W, L, W, W, W, W, W, L
$$

The counts of “water” $$W$$ and “land’ $$L$$ are distributed binomially, with probability p of “water” on each toss.

$$
Pr(W,L|p) = \frac {(W + L)!} {W!L!} p^W(1 − p)^L
$$

A short way of writing our model

W ~ Binomial(N, p)

Where N = W + L

p ~ Uniform(0,1)
