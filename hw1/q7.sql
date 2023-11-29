with data as(
    select
        position,
        champion_id,
        (totalkills+assists)/deaths as kda,
        rank() over(partition by position order by (totalkills+assists)/deaths desc) as rnk
    from(
        select 
            position,
            champion_id,
            sum(kills)+sum(doublekills)+sum(triplekills)+
                sum(quadrakills)+sum(pentakills)+sum(legendarykills) as totalkills
                ,sum(deaths) as deaths
                ,sum(assists) as assists
        from(
            (
            select player_id,champion_id,position
            from participant
            where position in ("TOP","MID","JUNGLE","DUO_CARRY","DUO_SUPPORT")
            )as t1 join (
                select player_id,kills,deaths,assists,doublekills,triplekills,
                        quadrakills,pentakills,legendarykills
                from stat
            )as t2 using(player_id)
        )
        where deaths>0
        group by position,champion_id
    )as bigtable
    group by position,champion_id
)
select position,champion_name,kda
from 
    data join champ using(champion_id)
where rnk=1
order by position;