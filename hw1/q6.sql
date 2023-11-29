with data as(
    select position,champion_name,
        rank() over(partition by position order by count(*) desc) as rnk
    from(
        select position,champion_name    
        from(
          participant as p
          join (
            select match_id
            from match_info
            where match_info.duration between 2400 and 3000
          ) as m using(match_id)
          join champ using(champion_id)
        )
        where position in ("DUO_CARRY","DUO_SUPPORT","JUNGLE","MID","TOP")
    )as pc
    group by position,champion_name
)

select position,champion_name
from data
where rnk=1;