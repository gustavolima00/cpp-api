CREATE EXTENSION pg_trgm;

CREATE TABLE people(
    id uuid default gen_random_uuid() primary key,
    nickname varchar(300) not null,
    name varchar(300) not null,
    birth_date varchar(20) not null,
    stack_as_string text
);

CREATE UNIQUE INDEX people_name_uindex ON people (name);
CREATE INDEX people_search_index ON people USING gin(name gin_trgm_ops, stack_as_string gin_trgm_ops, nickname gin_trgm_ops);
ANALYSE people;
