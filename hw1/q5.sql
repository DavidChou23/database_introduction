select
  case win
    when 1 then 'win'
    when 0 then 'lose'
  end as win_loss,
  count(*) as cnt
from
  (select match_id,win 
  from participant 
  inner join match_info using(match_id) 
  inner join stat using(player_id) 
  group by match_id, win
  having avg(longesttimespentliving) >= 1200
  ) as q
group by win
order by win asc;