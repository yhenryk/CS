SELECT first, last from Movie, MovieActor, Actor WHERE title = 'Die Another Day' and MovieActor.mid = Movie.id AND MovieActor.aid = Actor.id; 
--gives the first and last names of actors in the movie Die Another Day
SELECT count(DISTINCT m1.aid) from MovieActor m1, MovieActor m2 WHERE m1.mid <> m2.mid AND m1.aid = m2.aid; 
--counts actors in multiple movies
SELECT A.first, A.last from Actor A, Director D WHERE A.id = D.id; 
--finds the actors that are also directors