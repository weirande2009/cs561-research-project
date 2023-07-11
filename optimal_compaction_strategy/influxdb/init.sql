/*
表设计：
*/
create table Enumeration(
    eid int primary key,
    insert_num bigint,
    update_num bigint,
    delete_num bigint,
    second_level_size bigint, /* bytes */
    exp_result_path varchar(128),
    rrcid int,
    morid int,
    ocid int,
);

create table RoundRobinChoice(
    rrcid int primary key,
    wa bigint,
);

create table MinOverlappingRatioChoice(
    morid int primary key,
    wa bigint,
);

create table OptimalChoice(
    ocid int primary key,
    ccid int,
);

create table CompactionChoice(
    ccid bigint primary key,
    file_num int,
    choice_index int,
    overlapping_ratio_rank int,
    fiid int,


);

create table FileInfomation(
    fiid bigint primary key,
    overlapping_ratio bigint,
    smallest_key varchar(32),
    largest_key varchar(32),
    entry_num int,
    deletion_num int,
    raw_key_size bigint,
    raw_value_size bigint,
);

