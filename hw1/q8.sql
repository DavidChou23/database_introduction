select champion_name
from champ
where champion_id not in(
    select distinct champion_id
    from teamban
    where teamban.match_id in(
        select match_id
        from match_info
        where match_info.version like '7.7.%'
    )
)
order by champion_name;