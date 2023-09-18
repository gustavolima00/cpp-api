CREATE EXTENSION pg_trgm;

CREATE TABLE people(
    id uuid default gen_random_uuid() primary key,
    nickname varchar(33) not null,
    name varchar(101) not null,
    birth_date varchar(11) not null,
    stack varchar(33)[],
    stack_as_string text
);

CREATE UNIQUE INDEX people_name_uindex ON people (name);
CREATE INDEX people_search_index ON people USING gin(name gin_trgm_ops, stack_as_string gin_trgm_ops, nickname gin_trgm_ops);