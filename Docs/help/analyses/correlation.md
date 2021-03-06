Correlation Matrix
==========================

Correlation Matrix allows you to test the null hypothesis that the population correlation between pairs of variables equals 0.

Assumptions for null hypothesis significance testing using the Pearson product-moment correlation coefficient
-----------
- Numeric variables.
- The data are a random sample from the population for each variable.
- The pairs of variables follow a bivariate normal distribution in the population.

Assumptions for null hypothesis significance testing using the Spearman's rank-order correlation coefficient and the Kendall's tau B rank-order correlation coefficient
-----------
- Ordinal or numeric variables.
- The data are a random sample from the population for each variable.
- The relationship between the pairs of variables is monotonic.

Options
-------
### Correlation Coefficients: 
- Pearson: Pearson product-moment correlation coefficient to quantify the linear association between two variables.
- Kendall's tau-b: Kendall's tau B rank-order correlation coefficient to quantify the monotonic association between two variables.
- Spearman: Spearman's rank-order correlation coefficient to quantify the monotonic association between two variables.

### Hypothesis:
- Correlated: Two-sided alternative hypothesis that the population correlation does not equal 0. 
- Correlated positively: One-sided alternative hypothesis that the population correlation is larger than 0.
- Correlated negatively: One-sided alternative hypothesis that the population correlation is smaller than 0.

- Report significance: Report p-value for each significance test.
 
Output
-------
### Correlation Table (Note that this table is called "Pearson Correlations" if only the Pearson correlation coefficient is requested):
- Pearson's R: Pearson product-moment correlation coefficient. Alternatively:
  - Spearman's Rho: Spearman's rank-order correlation coefficient.
  - Kendall's tau B: Kendall's tau B rank-order correlation coefficient.
- p-value: p-value.

Example
-------

### Data set: 
- NBA Business Revenue and Team Value

### Input: 
- The null hypotheses that the pairwise population correlations between the business revenue ("Revenue"), team value ("Value"), debt ("Debt"), and income ("Income") of teams in the National Basketball Association (NBA) equals 0 are tested
  against the two-sided alternative hypotheses that the pairwise correlations does not equal 0.

### Output: 
- The Pearson correlation between "Value" and "Revenue" equals -0.323; the null hypothesis that the population correlation equals 0 cannot be rejected in favor of the two-sided alternative
  hypothesis that the population correlation does not equal 0, p = 0.082.
- The Pearson correlation between "Value" and "Debt" equals 0.986; the null hypothesis that the population correlation equals 0 is rejected in favor of the two-sided alternative
  hypothesis that the population correlation does not equal 0, p < .001.
- The Pearson correlation between "Value" and "Income" equals 0.718; the null hypothesis that the population correlation equals 0 is rejected in favor of the two-sided alternative
  hypothesis that the population correlation does not equal 0, p < .001.
- The Pearson correlation between "Revenue" and "Debt" equals -0.339; the null hypothesis that the population correlation equals 0 cannot be rejected in favor of the two-sided alternative
  hypothesis that the population correlation does not equal 0, p = 0.067.
- The Pearson correlation between "Revenue" and "Income" equals -0.301; the null hypothesis that the population correlation equals 0 cannot be rejected in favor of the two-sided alternative
  hypothesis that the population correlation does not equal 0, p = 0.106. 
- The Pearson correlation between "Debt" and "Income" equals 0.713; the null hypothesis that the population correlation equals 0 is rejected in favor of the two-sided alternative
  hypothesis that the population correlation does not equal 0, p < .001.

  
Additional Options
-------
- Flag significant correlations: Mark significant correlations in the "Correlation Table".

### Plots
  - Correlation Matrix: Displays a (matrix of) scatterplot(s) between the variables.
    - Densities for variables: Add a histogram and the corresponding density plot for each variable to the Correlation Matrix.
    - Statistics: Adds the correlation coefficient(s) and the corresponding 95% confidence interval(s) to the Correlation Matrix.

Additional Output
-------
### Correlation Table:
- Flag significant correlations: Significant correlations are marked with:
  - *p < .05 if the correlation is significant at alpha=.05 level.
  - **p < .01 if the correlation is significant at alpha=.01 level.
  - ***p < .001 if the correlation is significant at alpha=.001 level.

### Plots:
  - Correlation Matrix: Displays a (matrix of) scatterplot(s) between the variables (in the off-diagonal entries of the
    matrix). The solid black line in the scatterplot represents (TBA).
    - Densities for variables: Displays a histogram and the corresponding density plot for each variable in the diagonal entries of the matrix.
    - Statistics: Displays the correlation coefficient(s) and, in case only the Pearson's correlation is requested, also the corresponding 95% confidence interval(s) in the off-diagonal entries of the
    matrix.
      - r: Pearson product-moment correlation coefficient
      - rho: Spearman's rank-order correlation coefficient
      - tau: Kendall's tau B rank-order correlation coefficient
      
References
-------
 - Moore, D.S., McCabe, G.P., Craig, B.A. (2012). Introduction to the Practice of Statistics (7th ed.). New York, NY: W.H. Freeman and Company.
 - Whitlock, M.C. & Schluter, D. (2015). The Analysis of Biological Data (2nd ed.). Greenwood Village, Colorado: Roberts and Company Publishers.
