with t as (
    select *
    from(
        select champion_name,ss1,ss2,win_ratio,turns,
            rank() over(partition by champion_name order by win_ratio desc) as rnk
        from(
            select champion_id,
                ss1,
                ss2,
                count(case when win=1 then 1 end)/count(*) as win_ratio,
                rank() over(partition by champion_id order by count(*) desc) as turns
            from(
                (
                select player_id,champion_id,ss1,ss2
                from participant
                where position="TOP") as t1
                join (
                    select player_id, win
                    from stat
                )as t2 using(player_id)
            )
            group by champion_id,ss1,ss2

            )as t3 join champ using(champion_id)
        where turns between 1 and 5
    ) as t4
    where rnk=1
)
select champion_name,ss1,ss2,win_ratio
from(
    select champion_name,ss1,ss2,win_ratio,
        rank() over(partition by champion_name order by rnk+turns) as rnk1
    from t
) as t5
where rnk1=1