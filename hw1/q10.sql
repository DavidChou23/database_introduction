-- Gragas : 79
with 
    enemytable as(
        select match_id,player_id as enemy_id
        from participant
        where champion_id=79 and position="TOP"
    ),
    selftable1 as (
        select match_id,champion_id,player_id as self_id
        from participant
        where participant.match_id in (select match_id from enemytable)
        and participant.position="TOP" and participant.champion_id!=79
    ),
    selftable as(
        select *
        from selftable1
        where selftable1.champion_id in (
            select champion_id
            from selftable1
            group by champion_id
            having count(*)>=100
        )
    ),
    bigtable as(
        select 
            champion_id,champion_name,self_win,
            self_deaths,self_assists,
            self_kills+ self_doublekills+ self_triplekills+
                self_quadrakills+ self_pentakills+ self_legendarykills as self_kill,
            self_goldearned,enemy_win, enemy_deaths,enemy_assists,
            enemy_kills+ enemy_doublekills+ enemy_triplekills+ enemy_quadrakills+
                enemy_pentakills+ enemy_legendarykills as enemy_kill,
            enemy_goldearned    
        from selftable
        join champ using(champion_id)
        join(
            select
                player_id as self_id,           
                win as self_win,
                deaths as self_deaths,
                assists as self_assists,
                kills as self_kills,
                doublekills as self_doublekills,
                triplekills as self_triplekills,
                quadrakills as self_quadrakills,
                pentakills as self_pentakills,
                legendarykills as self_legendarykills,
                goldearned as self_goldearned 
            from stat
            where stat.player_id in (select player_id from selftable)
        )as s using (self_id)
        join enemytable using (match_id)
        join (
            select
                player_id as enemy_id,
                win as enemy_win,
                deaths as enemy_deaths,
                assists as enemy_assists,
                kills as enemy_kills,
                doublekills as enemy_doublekills,
                triplekills as enemy_triplekills,
                quadrakills as enemy_quadrakills,
                pentakills as enemy_pentakills,
                legendarykills as enemy_legendarykills,
                goldearned as enemy_goldearned
            from stat
            where stat.player_id in (select player_id from enemytable)
        )as enemy using (enemy_id)
    ),
    result as(
        select 
            champion_name as self_champ_name,
            count(case when self_win=1 then 1 end)/count(*) as win_ratio,
            (sum(self_kill)+sum(self_assists))/sum(self_deaths) as self_kda,
            avg(self_goldearned) as self_avg_gold,
            (sum(enemy_kill)+sum(enemy_assists))/sum(enemy_deaths) as enemy_kda,
            avg(enemy_goldearned) as enemy_avg_gold,
            count(*) as battle_record
        from bigtable
        group by champion_name
    )
select
    self_champ_name, win_ratio, self_kda,
    self_avg_gold, "Garas" as enemy_champ_name,
    enemy_kda, enemy_avg_gold, battle_record
from 
    result
order by win_ratio desc
limit 5;