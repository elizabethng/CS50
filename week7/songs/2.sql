-- list the names of all songs in increasing order of tempo.
-- your query should output a table with a single column for the name of each song.
SELECT
  name
FROM
  songs
ORDER BY
  tempo;