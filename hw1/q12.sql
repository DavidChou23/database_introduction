with data as(
        select match_id,champion_name,player_id,win,ss1,ss2,player,
              position,duration,version,killn,deaths,assists
        from (
        select player_id,win,kills+doublekills+triplekills+
                quadrakills+pentakills+legendarykills as killn,
                deaths,assists
        from stat where legendarykills=1)as t1 
        join participant using(player_id) 
        join champ using(champion_id) join match_info using(match_id)
        )
select data.match_id as matchId,champion_name as selfName,win as w,ss1,ss2,killn,deaths,assists,
        enemy_name as enName,enemy_ss1 as ens1,enemy_ss2 as ens2,ekilln as ekill,edeaths as edeath,
        eassists as eassist,position,version
from data join
        (
                select match_id,champion_name as enemy_name,position as eposition,player,
                        ss1 as enemy_ss1,ss2 as enemy_ss2,kills+doublekills+triplekills+
                quadrakills+pentakills+legendarykills as ekilln,
                deaths as edeaths,assists as eassists
                from participant join champ using(champion_id) join stat using(player_id)
        ) as p on p.match_id = data.match_id and 
                p.eposition = data.position and 
                p.player <> data.player