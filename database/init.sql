CREATE TABLE people(
    id uuid default gen_random_uuid() primary key,
    nickname varchar(33) not null,
    name varchar(101) not null,
    birth_date varchar(11) not null,
    stack varchar(33)[]
);

CREATE UNIQUE INDEX people_name_uindex ON people (nickname) USING(hash);