select c.champion_name, r.cnt
from(
  champ as c,
  (
    select champion_id, count(*) as cnt
    from participant as p
    where p.position='JUNGLE'
    group by champion_id
    order by cnt desc
    limit 3
  ) as r
)
where c.champion_id=r.champion_id;