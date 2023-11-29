-- Lee Sin: 64, Teemo: 17
with t1 as (
    select ver,win
    from(
        select win,match_id,substring_index(version,".",2) as ver
        from(
            participant 
            join stat using(player_id) 
            join match_info using(match_id)
        )
        where champion_id in (64,17)
    ) as t
    group by ver,match_id,win
    having count(*)>1
)
select 
    ver as version,
    count(case when win=1 then 1 end) as win_cnt,
    count(case when win=0 then 1 end) as lose_cnt,
    count(case when win=1 then 1 end)/count(*) as win_ratio
from t1
group by ver
order by ver;
