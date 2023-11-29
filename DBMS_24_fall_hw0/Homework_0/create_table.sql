create table mask(
    inst_id varchar(15),
    adult_mask_num int default 0,
    child_mask_num int default 0,
    data_time datetime,
    primary key (inst_id, data_time)
);

load data local infile './maskdata.csv'
into table mask
fields terminated by ','
enclosed by '"'
lines terminated by '\n'
ignore 1 lines;

load data local infile './transfer.csv'
into table mask
fields terminated by ','
enclosed by '"'
lines terminated by '\n'
ignore 1 lines;
