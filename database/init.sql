CREATE TABLE pessoa (
    id uuid default gen_random_uuid() primary key,
    apelido varchar(33) not null,
    nome varchar(101) not null,
    nascimento varchar(11) not null,
    stack varchar(33)[],
);
