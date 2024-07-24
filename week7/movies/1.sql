/*
query to list the titles of all movies released in 2008.
Your query should output a table with a single column for the title of each movie.
Below, I think style50 thinks that year is a function and not the column name...
*/
SELECT
  title
FROM
  movies
WHERE
  YEAR = 2008;